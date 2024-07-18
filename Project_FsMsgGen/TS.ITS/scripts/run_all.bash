#!/bin/bash
#set -e
#set -vx

clear

if [ -z $ATS ]
then
    echo "ATS shall be defined"
    exit 1
fi

if ! [[ $1 =~ "^[0-9]+$" ]]
then
    COUNTER=$1
else
    COUNTER=1
fi

if [ $COUNTER == 0 ]
then
   COUNTER=1
   cd .. && make && cd -
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

rm ../logs/$ATS/*.log

for i in $(seq 1 1 $COUNTER)
do
    ./run_mtc.bash &
    ./run_ptcs.bash

    dup=$(ps -ef | grep "$0" | grep -v grep | wc -l)
    while [ ${dup} -eq 3 ]
    do
        sleep 1
        dup=$(ps -ef | grep "$0" | grep -v grep | wc -l)
    done
    sleep 1

    mv ../logs/$ATS/merged.log ../logs/$ATS/merged.`date +'%Y%m%d%S'`.log
done

exit 0


