#!/bin/bash
# Copyright ETSI 2018-2020
# See: https://forge.etsi.org/etsi-forge-copyright-statement.txt

#set -e
set -vx

docker run --interactive --tty --rm --publish 2222:22 --env DISPLAY=$DISPLAY --volume /tmp/.X11-unix:/tmp/.X11-unix --cap-add=NET_RAW --cap-add=NET_ADMIN etsiforge/etsi-its-ts:latest

# That's all Floks
exit 0
