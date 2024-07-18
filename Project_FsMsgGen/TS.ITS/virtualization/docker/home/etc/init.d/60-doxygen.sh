#!/bin/bash

#set -e
set -vx

echo -e "*****************************\n* Build docs \n*****************************\n"
. /home/etsi/devenv.bash
cd ${HOME}/dev/TS.ITS/docs || exit 1
doxygen ./o2.cfg || exit 1

# Generate PDF file
cd ${HOME}/dev/TS.ITS/docs/AtsDocs/latex
make && mv refman.pdf ${HOME}/docs/etsi_its_help.pdf

cd ${HOME}

exit 0
