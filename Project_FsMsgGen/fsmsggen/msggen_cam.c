#include "msggen.h"
#include "cmem.h"
#include "copts.h"

#include "payload/CAM.h"
#include "gn_types.h"
#include "../uppertester/uppertester.h"

static void cam_process (MsgGenApp * app, FitSec * e);
static int cam_options  (MsgGenApp* app, int argc, char* argv[]);
static size_t cam_fill  (MsgGenApp* app, FitSec * e, FSMessageInfo* m);
static void cam_onEvent (MsgGenApp * app, FitSec* e, void* user, FSEventId event, const FSEventParam* params);

static int   cam_ut_handler(FSUT* ut, void* ptr, FSUT_Message* m, int * psize);

static MsgGenApp _cam = {
    "cam", 0, cam_process, cam_options, cam_fill, cam_onEvent, cam_ut_handler
};

__INITIALIZER__(initializer_cam) {
     MsgGenApp_Register(&_cam);
}

static const char* _o_stationTypes[] = {
    "",
    "unknown",      // 0
    "pedestrian",   // 1
    "cyclist",      // 2
    "moped",        // 3
    "motorcycle",   // 4
    "passengerCar", // 5
    "bus",          // 6
    "lightTruck",   // 7
    "heavyTruck",   // 8
    "trailer",         // 9
    "special", // 10
    "tram",            // 11
    "",                // 12
    "",                // 13
    "",                // 14
    "rsu",            // 15
    NULL
};


static int _o_secured = 1;
static int _o_btpA = 0;
static int _activated = 1;

static copt_t options[] = {
    { "T",  "cam-station-type",  COPT_STRENUM ,  _o_stationTypes, "Station Type [unknown]" },
    { "B",  "cam-btpA",          COPT_BOOL ,    &_o_btpA, "Use BTP A [use btpB by default]" },
    { "C",  "cam-stop",          COPT_IBOOL ,   &_o_secured, "Send non-secured cam" },
    { NULL, "cam-no-sec",        COPT_IBOOL ,   &_activated, "Do not start CAM by default" },
    { NULL, NULL, COPT_END, NULL, NULL }
};

static CAM_t* cam = NULL;

static int cam_options(MsgGenApp* app, int argc, char* argv[])
{
    printf("\n  CAM_OPTIONS TO INIT THE CAM MESSAGE (msggen_cam.c)\n");
    // init CAM
    if (cam == NULL) {
        // register uppertester

        cam = cnew0(CAM_t);
        cam->header.messageID = messageID_cam;
        cam->header.protocolVersion = 2;
        cam->cam.generationDeltaTime = 0;

        cam->cam.camParameters.basicContainer.stationType = StationType_passengerCar;
        cam->cam.camParameters.basicContainer.referencePosition.altitude.altitudeValue = AltitudeValue_unavailable;
        cam->cam.camParameters.basicContainer.referencePosition.altitude.altitudeConfidence = AltitudeConfidence_unavailable;
        cam->cam.camParameters.basicContainer.referencePosition.positionConfidenceEllipse.semiMajorConfidence = SemiAxisLength_unavailable;
        cam->cam.camParameters.basicContainer.referencePosition.positionConfidenceEllipse.semiMinorConfidence = SemiAxisLength_unavailable;
        cam->cam.camParameters.basicContainer.referencePosition.positionConfidenceEllipse.semiMajorOrientation = HeadingValue_unavailable;

        cam->cam.camParameters.highFrequencyContainer.present = HighFrequencyContainer_PR_basicVehicleContainerHighFrequency;
        cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency.heading.headingValue = HeadingValue_unavailable;
        cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency.heading.headingConfidence = HeadingConfidence_unavailable;
        cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency.speed.speedValue = SpeedValue_standstill;
        cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency.speed.speedConfidence = SpeedConfidence_unavailable;
        cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency.driveDirection = DriveDirection_unavailable;
        cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency.vehicleLength.vehicleLengthValue = VehicleLengthValue_unavailable;
        cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency.vehicleLength.vehicleLengthConfidenceIndication = VehicleLengthConfidenceIndication_unavailable;
        cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency.vehicleWidth = VehicleWidth_unavailable;
        cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency.longitudinalAcceleration.longitudinalAccelerationValue = LongitudinalAccelerationValue_unavailable;
        cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency.longitudinalAcceleration.longitudinalAccelerationConfidence = AccelerationConfidence_unavailable;
        cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency.curvature.curvatureValue = CurvatureValue_unavailable;
        cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency.curvature.curvatureConfidence = CurvatureConfidence_unavailable;
        cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency.curvatureCalculationMode = CurvatureCalculationMode_unavailable;
        cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency.yawRate.yawRateValue = YawRateValue_unavailable;
        cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency.yawRate.yawRateConfidence = YawRateConfidence_unavailable;
    }

    int rc = 0;
    if (argc == 0) {
        coptions_help(stderr, "CAM", 0, options, "");
    }
    else {
        rc = coptions(argc, argv, COPT_NOREORDER | COPT_NOAUTOHELP | COPT_NOERR_UNKNOWN | COPT_NOERR_MSG, options);
        if (rc >= 0) {
            if (options[0].vptr != _o_stationTypes) {
                cam->cam.camParameters.basicContainer.stationType = copts_enum_value(options, 0, _o_stationTypes)-1;
            }
        }
    }
    return rc;
}
static void cam_onEvent (MsgGenApp * app, FitSec* e, void* user, FSEventId event, const FSEventParam* params)
{}

static GNCommonHeader _def_ch = {
    0x20, // nextHeader BTP-B
    0x50, // .headerType = SHB
    0x02, // .trafficClass
    0x80, // .flags
    0,    // .payload length
    1,    // .maxHopLimit
    0,    // .reserved2
};

static GNExtendedHeader _def_eh = {
    .shb = {
        .srcPosVector = {
            .gnAddr = {0},
            .timestamp = 0,
            .latitude = 0,
            .longitude = 0,
            .accAndSpeed = 0,
            .heading = 0
        },
        .reserved = 0
    }
};

static void cam_process (MsgGenApp * app, FitSec * e)
{
    printf("\n CAM PROCESS (mssgen_cam.c)\n");
    if(_activated){
        MsgGenApp_Send(e, app); 
    }
}

unsigned int printBufs (uint8_t *bufs)
{
   unsigned int i = 0;
   int num_row = -10;
   for(i=0; i<373; i++){  

        if(i%16==0){
            num_row+=10;
            printf("%04d)   ", num_row);   
        } 

        printf("%02x  ", bufs[i]);

        if ((i + 1) % 8 == 0) {
            printf("  "); // newline every 16 bytes for better readability
        }
        if ((i + 1) % 16 == 0) {
            printf("\n"); // newline every 16 bytes for better readability
        }
   }
      
   return i+1;
}

static size_t cam_fill(MsgGenApp* app, FitSec * e, FSMessageInfo* m)
{
    if(m)
        printf("\n CAM FILL (mssgen_cam.c)\n");
    size_t len;
    m->status = 0;
   
    if (_o_secured) {
        m->payloadType = FS_PAYLOAD_SIGNED;
        m->sign.ssp.aid = 36;
        memset(m->sign.ssp.sspData.opaque, 0, sizeof(m->sign.ssp.sspData.opaque));
        m->sign.ssp.sspLen = 3;
        m->sign.ssp.sspData.bits.version = 1;
        //m->sign.cert = extendedCert;
        //m->encryption.cert = extendedCert ;
        len = FitSec_PrepareSignedMessage(e, m); // Prepare the buffer for the signed ITS message envelop
        if (len <= 0) {
            fprintf(stderr, "%-2s PREP %s:\t ERROR: 0x%08X %s\n", FitSec_Name(e), "PrepareSignedMessage", m->status, FitSec_ErrorMessage(m->status));
            return len;
        }
    }
    else {
        m->payloadType = FS_PAYLOAD_UNSECURED;
        m->payload = m->message;
    }
    //printBufs(m->payload);
    GNCommonHeader* ch = (GNCommonHeader*)m->payload;
    GNExtendedHeader* eh = (GNExtendedHeader*)(ch + 1);
 
    *ch = _def_ch;
    *eh = _def_eh;
    uint32_t * bh = (uint32_t*)&((&eh->shb)[1]);
    bh[0] = 0x0000d107; // port 2001
    len = ((char*)&bh[1]) - m->payload;
    if (_o_btpA) {
        ch->nextHeader = 0x10;
    }

    cam->header.stationID = 0x10101010;//(unsigned long)FSCertificate_Digest(m->cert);
    cam->cam.camParameters.basicContainer.referencePosition.latitude = m->position.latitude;
    cam->cam.camParameters.basicContainer.referencePosition.longitude =m->position.longitude;

    eh->shb.srcPosVector.latitude = m->position.latitude;
    eh->shb.srcPosVector.longitude = m->position.longitude;
    eh->shb.srcPosVector.timestamp = (uint32_t)(m->generationTime / 1000);

    asn_enc_rval_t rc = asn_encode_to_buffer(NULL, ATS_UNALIGNED_CANONICAL_PER, &asn_DEF_CAM, cam, m->payload + len, m->payloadSize - len);
    if (rc.encoded < 0) {
        fprintf(stderr, "%-2s SEND %s:\t ERROR: %zu at %s\n", FitSec_Name(e), "asn_encode", rc.encoded, rc.failed_type->name);
        len = 0;
    }
    else {
        char* p = m->payload + len + rc.encoded;
        m->payloadSize = p - m->payload;
        ch->plLength = cint16_hton((unsigned short)(rc.encoded + 4)); // plus BTP
        if (_o_secured) {
          
            len = FitSec_FinalizeSignedMessage(e, m);
           // printBufs(m->message);
            if (len == 0) {
                fprintf(stderr, "%-2s SEND %s:\t ERROR: 0x%08X %s\n", FitSec_Name(e), "FinalizeSignedMessage", m->status, FitSec_ErrorMessage(m->status));
            }
        }
        else {
            m->messageSize = p - m->message;
        }
    }
    return len;
}

static int  cam_ut_handler(FSUT* ut, void* ptr, FSUT_Message* m, int * psize)
{
    printf("\n IM in CAM UT HANDLER (mssgen_cam.c)\n");
    switch (m->code){
        case FS_UtCamTrigger:
            _activated = m->camState.state;
            break;

        case FS_UtCamTrigger_changeSpeed:
        {
            if(cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency.speed.speedValue == SpeedValue_unavailable){
                cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency.speed.speedValue = 0;
            }
            cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency.speed.speedConfidence = 20; // 20cm/sec
            cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency.speed.speedValue += m->changeSpeed.speed;
            if(cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency.speed.speedValue < 0)
                cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency.speed.speedValue = 0;
            else if (cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency.speed.speedValue > 16382)
                cam->cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency.speed.speedValue = 16382;
            break;
        }
    }
    return 0;
}

