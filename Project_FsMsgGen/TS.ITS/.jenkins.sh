#!/bin/bash
# Copyright ETSI 2018-2021
# See: https://forge.etsi.org/etsi-forge-copyright-statement.txt

set -vx
#set -e

cd $(dirname $0)

run_dir=`pwd`

# Docker version 2
cd ./virtualization/docker
./build.sh #--force-stfubuntu
ret_code=`echo $?`

if [ $ret_code != 0 ]
then
   cd $run_dir
   exit -1
fi

#./run.sh

if [ "$1" == "--remove" ]
then
    docker rmi --force etsiforge/etsi-its-ts:latest
fi

cd $run_dir

exit 0
