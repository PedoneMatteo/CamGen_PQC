#!/bin/bash

#set -e
set -vx

echo -e "*****************************\n* Build ETSI ITS certificate generation tools \n*****************************\n"
cd ${HOME}/dev || exit 1

git clone --recurse-submodules https://forge.etsi.org/rep/ITS/itscertgen.git ./itscertgen
cd ./itscertgen
cd cshared && make && cd -
cd certgen/ && make && cd -

echo -e "*****************************\n* Generate ETSI ITS certificates \n*****************************\n"
cd ${HOME}/dev/TS.ITS/data/certificates
CERTGEN=${HOME}/dev/itscertgen make

cd ${HOME}

exit 0

