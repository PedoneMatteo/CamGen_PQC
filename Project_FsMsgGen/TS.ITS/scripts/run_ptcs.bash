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

if [ -f ./core ]
then
    rm -f ./core
fi

if [ "$ATS" == "AtsRSUsSimulator" ]
then
    sudo LD_LIBRARY_PATH=$LD_LIBRARY_PATH ../bin/$ATS 127.0.0.1 12001
else
    sudo LD_LIBRARY_PATH=$LD_LIBRARY_PATH ../bin/$ATS 127.0.0.1 12000
fi

cd ${CURPWD}
