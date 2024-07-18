#!/bin/bash
# Copyright ETSI 2020-2021
# See: https://forge.etsi.org/etsi-forge-copyright-statement.txt

set -e
set -vx

# Force removing stfubuntu docker image
if [ "$1" == "--force-stfubuntu" ]; then
    if [ -n `docker images -q stfubuntu` ]; then
        docker rmi --force `docker images -q stfubuntu` || exit 1
    fi
fi
# Check and build stfubuntu image
if [ -z `docker images -q stfubuntu` ]; then
    docker build --no-cache --tag stfubuntu:20.04 -f Dockerfile.stfubuntu --force-rm  . || exit 1
fi

docker build --no-cache --tag etsiforge/etsi-its-ts --force-rm  . || ( echo "Docker build failed: $?"; exit 1 )

docker images
docker inspect etsiforge/etsi-its-ts:latest || ( echo "Docker inspect failed: $?"; exit 1 )

# That's all Floks
exit 0
