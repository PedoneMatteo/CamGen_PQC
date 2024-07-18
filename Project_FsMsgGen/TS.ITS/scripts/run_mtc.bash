#!/bin/bash
#set -e
#set -vx

clear

if [ -z $ATS ]
then
    echo "ATS shall be defined"
    exit 1
fi

CURPWD=`pwd`
if [ ! "${PWD##*/}" == "scripts" ]
then
    cd ../scripts
    if [ ! $? == 0 ]
    then
        echo "Please move to PROJECT/scripts directory"
        exit 1
    fi
fi

TITAN_LOG_DIR=../logs/$ATS
if [ ! -d ${TITAN_LOG_DIR} ]
then
    mkdir ${TITAN_LOG_DIR}
else
    rm -f ${TITAN_LOG_DIR}/*.log
fi

CFG_FILES=`find ../etc/$ATS -name '*.cfg'`

echo "> cmtc: to create the MTC server"
echo "> smtc [module_name[[.control]|.testcase_name|.*]: when MyExample is connected, run the TCs in [EXECUTE] section"
echo "> emtc: Terminate MTC."
mctr ${CFG_FILES}

sudo chown -R `whoami` ${TITAN_LOG_DIR}
LOG_FILES=`find ${TITAN_LOG_DIR} -name '*.log'`
if [ "${TITAN_LOG_DIR}" != "" ]
then
    ttcn3_logmerge -o ${TITAN_LOG_DIR}/merged.log ${LOG_FILES}
    ttcn3_logformat -o ${TITAN_LOG_DIR}/merged_formated.log ${TITAN_LOG_DIR}/merged.log
    mv ${TITAN_LOG_DIR}/merged_formated.log ${TITAN_LOG_DIR}/merged.log
    echo "log files were merged into ${TITAN_LOG_DIR}/merged.log"
fi

cd ${CURPWD}
