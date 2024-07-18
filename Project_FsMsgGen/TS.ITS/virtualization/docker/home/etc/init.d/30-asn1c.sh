#!/bin/bash

#set -e
set -vx

echo -e "*****************************\n* Install asn1c\n*****************************\n"
cd ${HOME}/frameworks || exit 1
git clone https://github.com/fillabs/asn1c.git ./asn1c || exit 1
cd ./asn1c
autoreconf -iv && ./configure --prefix=${HOME} && make install || exit 1

cd ${HOME}

exit 0
