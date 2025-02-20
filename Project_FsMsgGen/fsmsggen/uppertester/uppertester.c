#include <csocket.h>
#include <cmem.h>
#include <cstr.h>
#include <cring.h>
#include <time.h>
#include <stdio.h>
#include "uppertester.h"

typedef struct FSUT_Handler {
    cring_t ring;
    FSUT_Handler_fn handler;
    void* ptr;
}FSUT_Handler;

struct FSUT
{
    csocket_t s;
    struct sockaddr_in local;
    struct sockaddr_in from;
    cring_t handlers;
    unsigned char buf[FSUT_MAX_MSG_SIZE];
    cring_t out;
};

typedef struct FSUT_Msg {
    cring_t ring;
    size_t  size;
    FSUT_Message m;
}FSUT_Msg;

FSUT_Msg * FSUT_Msg_New(size_t l){
    FSUT_Msg * m = (FSUT_Msg *)malloc(l + sizeof(FSUT_Msg) - sizeof(FSUT_Message));
    cring_init(&m->ring);
    m->size = l;
    return m;
}

FSUT* FSUT_New(const char* bind_host, int bind_port)
{
    FSUT* ut = cnew0(FSUT);
    ut->local.sin_family = PF_INET;
    ut->local.sin_addr.s_addr = INADDR_ANY;
    if (bind_host) {
        if (0 > inet_pton(AF_INET, bind_host, &ut->local.sin_addr)) {
            perror(bind_host);
            ut->local.sin_addr.s_addr = INADDR_ANY;
        }
    }
    ut->local.sin_port = cint16_hton(bind_port);

    cring_init(&ut->handlers);
    cring_init(&ut->out);
    return ut;
}

void  FSUT_Free(FSUT* ut)
{
    if (ut) {
        FSUT_Stop(ut);
        cring_cleanup(&ut->handlers, free);
        cring_cleanup(&ut->out, free);
        free(ut);
    }
}

void  FSUT_RegisterHandler(FSUT* ut, FSUT_Handler_fn handler, void* ptr)
{
    FSUT_Handler* h = cnew(FSUT_Handler);
    cring_init(&h->ring);
    h->handler = handler;
    h->ptr = ptr;
    cring_enqueue(&ut->handlers, h);
}

int FSUT_Start(FSUT* ut)
{
printf("\nIM in FSUT_START (in uppertester.c)\n");
#ifdef _MSC_VER
    WSADATA wd;
    WSAStartup(MAKEWORD(2, 2), &wd);
#endif
    csocket_t s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s > 0) {
        int flags = 1;
        const char* fn = "setsockopt";
        if (0 <= setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*)&flags, sizeof(flags))) {
            fn = "bind";
            if (0 <= bind(s, (struct sockaddr*)&ut->local, sizeof(ut->local))) {
                ut->s = s;
                return 0;
            }
        }
        perror(fn);
        csocket_close(s);
    }
    return -1;
}

void  FSUT_Stop(FSUT* ut)
{
    if (ut->s) {
        csocket_t s = ut->s;
        ut->s = 0;
        csocket_close(s);
    }
}

static int _FSUT_Proceed(FSUT* ut, struct timeval * tv)
{  
    if (!ut) return -1;
    int nfds = 1, len = 0;
    fd_set rs = { 0 }, es = { 0 };
    csocket_t s = ut->s;
    FD_SET(ut->s, &rs);
    FD_SET(ut->s, &es);
#ifndef _MSC_VER
    nfds = s + 1;
#endif
    int n = select(nfds, &rs, NULL, &es, tv); // wait event on socket with a timeout of tv
    if (n < 0){
        perror("select");
        return -1;
    }
    if(ut->out.next != &ut->out){ // check if there is a message to send
        FSUT_Msg * m = (FSUT_Msg *)ut->out.next;
        cring_erase(&m->ring);
        sendto(ut->s, (const char*)(&m->m), m->size, 0, (const struct sockaddr*)&ut->from, sizeof(struct sockaddr_in));
        free(m);
    }
    if (n > 0){ // if the select function return a value that indicate the presence of data to read
        if (FD_ISSET(s, &es)) {
            return -1;
        }
        if (FD_ISSET(s, &rs)) {
            socklen_t salen = sizeof(struct sockaddr_in);
            struct sockaddr_in from = { 0 };
            //read data from socket
            len = recvfrom(ut->s, ut->buf, FSUT_MAX_MSG_SIZE, 0, (struct sockaddr*)&from, &salen); 
            if (0 > len) { //if there is errors during reading
                return len;
            }
            //process the received message and send it to the receiver
            int outlen = FSUT_onUTMessage(ut, (const char*)(ut->buf), len);
            if (outlen > 0) {
                sendto(ut->s, (const char*)(ut->buf), outlen, 0, (const struct sockaddr*)&from, sizeof(struct sockaddr_in));
            }
            FSUT_Message* m = (FSUT_Message*)&ut->buf[0];
            if (m->code <= 1) { // utInitialize or Result
                ut->from = from;
            }
        }
    }
    return len;
}

int   FSUT_Run(FSUT* ut)
{
    if (ut && ut->s) {
        struct timeval tv = {
            0, 100000
        };
        do {
            int len = _FSUT_Proceed(ut, &tv);
            if(len < 0){
                if (! ut->s ) {
                    // stop;
                    break;
                }
                // error
                FSUT_Stop(ut);
                FSUT_Start(ut);
                continue;
            }
        } while (ut->s > 0);
    }
    return 0;
}

int   FSUT_Proceed(FSUT* ut, FSUT_Message * m)
{
    struct timeval tv = { 0,0 };
    if(m){ //non entra
    //usefull to process the messagen and calling the handlers associated to ut
      FSUT_onUTMessage(ut, (const char*) m, 0);
    }
    return _FSUT_Proceed(ut, &tv);
}

unsigned long long unix2itstime64(time_t t);

int   FSUT_onUTMessage(FSUT* ut, const char* buf, size_t size)
{
    if (ut) {
        FSUT_Message* m = (FSUT_Message*)buf;
        FSUT_Handler* h = cring_first_cast(ut->handlers, FSUT_Handler);
        for (; &h->ring != &ut->handlers; h = cring_next_cast(h, FSUT_Handler)) {
            int s = (int)size;
            int rc = h->handler(ut, h->ptr, m, &s);
            if (rc > 0) {
                // we have to send this result
                return (size_t)s;
            }
        }
    }
    return 0;
}

int FSUT_SendIndication(FSUT* ut, uint8_t code, const char* buf, size_t size)
{
    FSUT_Message * m;
    char _tmp[32];
    size_t len;
    if(ut) {
        switch(code){
        case FS_UtPkiTriggerInd:
            m = (FSUT_Message *)_tmp;
            m->pkiState.state = buf[0];
            len = sizeof(m->pkiState);
            break;
        default:
            m = (FSUT_Message *)struct_from_member(struct FSUTMsg_Indication, buf, pdu);
            m->indication.pduLength = cint16_hton((uint16_t)size);
            len = size + 3;
            break;
        }
        m->code = code; 
        sendto(ut->s, (const char*)(m), len, 0, (const struct sockaddr*)&ut->from, sizeof(struct sockaddr_in));
        return 0;
    }
    return -1;
}

void FSUT_EnqueueIndication(FSUT* ut, uint8_t code, const char* buf, size_t size)
{
    if(ut) {
        FSUT_Msg * m; 
        switch(code){
        case FS_UtPkiTriggerInd:
            m = FSUT_Msg_New(sizeof(m->m.pkiState));
            m->m.pkiState.state = buf[0];
            m->size = sizeof(m->m.pkiState);
            break;
        default:
            m = FSUT_Msg_New(3+size);
            memcpy(m->m.indication.pdu, buf, size);
            m->m.indication.pduLength = cint16_hton((uint16_t)size);
            break;
        }
        m->m.code = code;
        cring_enqueue(&ut->out, &m->ring); 
    }
}

static int _cmd_UtInitialize(FSUT_Message ** pmsg, int argc, char ** argv);
static int _cmd_UtChangePosition(FSUT_Message ** pmsg, int argc, char ** argv);
static int _cmd_UtChangePseudonym(FSUT_Message ** pmsg, int argc, char ** argv);
static int _cmd_UtCam(FSUT_Message ** pmsg, int argc, char ** argv);
static int _cmd_UtDenmTrigger(FSUT_Message ** pmsg, int argc, char ** argv);
static int _cmd_UtDenmUpdate(FSUT_Message ** pmsg, int argc, char ** argv);
static int _cmd_UtDenmTerminate(FSUT_Message ** pmsg, int argc, char ** argv);
static int _cmd_UtDenm(FSUT_Message ** pmsg, int argc, char ** argv);
static int _cmd_UtGnTrigger(FSUT_Message ** pmsg, int argc, char ** argv);
static int _cmd_UtEnroll(FSUT_Message ** pmsg, int argc, char ** argv);
static int _cmd_UtAuth(FSUT_Message ** pmsg, int argc, char ** argv);

typedef struct UTHandlerRecord{ //struct that connect a name to a function
    const char * name;
    int (*create)(FSUT_Message ** pmsg, int argc, char ** argv);
} UTHandlerRecord;

static const UTHandlerRecord _msgnames[] = {
    {"initialize", _cmd_UtInitialize},
    {"position", _cmd_UtChangePosition},
    {"pseudonym", _cmd_UtChangePseudonym},
    {"cam", _cmd_UtCam },
    {"denm", _cmd_UtDenm },
    {"gn", _cmd_UtGnTrigger },
    {"enrol" , _cmd_UtEnroll},
    {"auth" , _cmd_UtAuth },
};

int _FSUT_ExecCommand(FSUT_Message ** pmsg, const UTHandlerRecord * cmds, size_t ccnt, int argc, char ** argv)
{
    for(int i=0; i<ccnt; i++){
        if(0 == strcmp(argv[0], cmds[i].name)){
            return cmds[i].create(pmsg, argc, argv);
        }
    }
    return 0;
}

int FSUT_CommandMessage(FSUT_Message ** pmsg, int argc, char ** argv)
{
    printf("\n\t(in uppertester.c) argv[0] = %s", argv[0]);
    return _FSUT_ExecCommand(pmsg, &_msgnames[0], arraysize(_msgnames), argc, argv);
}

static int _cmd_UtInitialize(FSUT_Message ** pmsg, int argc, char ** argv)
{
    if(argc > 1){
        FSUT_Message * m = malloc(sizeof(m->initialize));
        m->initialize.code = FS_UtInitialize;
        char * end = argv[1];
        m->initialize.digest = strtoull(argv[1], &end, 16);
        if(end > argv[1]){
            m->initialize.digest = cint64_hton(m->initialize.digest);
            *pmsg = m;
            return 2;
        }
    }
    return 0;
}

static int _cmd_UtChangePosition(FSUT_Message ** pmsg, int argc, char ** argv)
{
    if(argc > 2){
        char * end;
        int ret = 3;
        FSUT_Message * m = malloc(sizeof(m->changePosition));
        m->changePosition.code = FS_UtChangePosition;
        end = argv[1];
        m->changePosition.deltaLatitude = strtoul(argv[1], &end, 10);
        if(end  > argv[1]){
            end = argv[2];
            m->changePosition.deltaLongitude = strtoul(argv[2], &end, 10);
            if(end  > argv[2]){
                if(argc > 3){
                    end = argv[3];
                    m->changePosition.deltaAltitude = strtoul(argv[3], &end, 10);
                    if(end  <= argv[3])
                        goto err;
                    ret++;
                }
                *pmsg = m;
                return ret;
            }
        }
    }
err:
    return 0;
}

static int _cmd_UtChangePseudonym(FSUT_Message ** pmsg, int argc, char ** argv)
{
    if(argc > 1){
        FSUT_Message * m = malloc(sizeof(m->changePseudonym));
        m->changePseudonym.code = FS_UtChangePseudonym;
        *pmsg = m;
        return 1;
    }
    return 0;
}

static int _cmd_UtCamStart(FSUT_Message ** pmsg, int argc, char ** argv) {
    FSUT_Message * m = malloc(sizeof(m->camState));
    m->camState.code = FS_UtCamTrigger;
    m->camState.state = 1;
    *pmsg = m;
    return 1;
}
static int _cmd_UtCamStop(FSUT_Message ** pmsg, int argc, char ** argv) {
    FSUT_Message * m = malloc(sizeof(m->camState));
    m->camState.code = FS_UtCamTrigger;
    m->camState.state = 0;
    *pmsg = m;
    return 1;
}
static int _cmd_UtCamRate(FSUT_Message ** pmsg, int argc, char ** argv) {
    return 0;
}

static const UTHandlerRecord _cammsgnames[] = {
    {"start", _cmd_UtCamStart},
    {"stop",  _cmd_UtCamStop},
    {"rate",  _cmd_UtCamRate}
};

static int _cmd_UtCam(FSUT_Message ** pmsg, int argc, char ** argv)
{
    if(argc > 1){
        return _FSUT_ExecCommand(pmsg, _cammsgnames, arraysize(_cammsgnames), argc -1, argv + 1);
    }
    return 0;
}

static const UTHandlerRecord _denmmsgnames[] = {
    {"send",       _cmd_UtDenmTrigger},
    {"update",     _cmd_UtDenmUpdate},
    {"stop",       _cmd_UtDenmTerminate},
    {"terminate",  _cmd_UtDenmTerminate}
};

static int _cmd_UtDenm(FSUT_Message ** pmsg, int argc, char ** argv)
{
    if(argc > 1){
        return _FSUT_ExecCommand(pmsg, _denmmsgnames, arraysize(_denmmsgnames), argc -1, argv + 1);
    }
    return 0;
}
static int _cmd_UtDenmTrigger(FSUT_Message ** pmsg, int argc, char ** argv)
{
    return 0;
}
static int _cmd_UtDenmUpdate(FSUT_Message ** pmsg, int argc, char ** argv)
{
    return 0;
}
static int _cmd_UtDenmTerminate(FSUT_Message ** pmsg, int argc, char ** argv)
{
    return 0;
}
static int _cmd_UtGnTrigger(FSUT_Message ** pmsg, int argc, char ** argv)
{
    return 0;
}

static int _cmd_UtSimpleMessage(FSUT_Message ** pmsg, uint8_t code)
{
    FSUT_Message * m = malloc(sizeof(m->code));
    m->code = code;
    *pmsg = m;
    return 1;
}

static int _cmd_UtEnroll(FSUT_Message ** pmsg, int argc, char ** argv)
{
    if(argc >= 1){
        return _cmd_UtSimpleMessage(pmsg, FS_UtGenerateInnerEcRequest);
    }
    return 0;
}

static int _cmd_UtAuth(FSUT_Message ** pmsg, int argc, char ** argv)
{
    if(argc >= 1){
        return _cmd_UtSimpleMessage(pmsg, FS_UtGenerateInnerAtRequest);
    }
    return 0;
}
