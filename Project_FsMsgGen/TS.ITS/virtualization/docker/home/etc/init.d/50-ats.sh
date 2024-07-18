#!/bin/bash

#set -e
set -vx

echo -e "*****************************\n* Change user in cfg files\n*****************************\n"
. /home/etsi/devenv.bash
cd ${HOME}/dev/TS.ITS/scripts || exit 1
./update_user_name.sh || exit 1


echo -e "*****************************\n* Build test suites\n*****************************\n"
cd ${HOME}/dev/TS.ITS
ATS_LIST="AtsCAM AtsDENM AtsGeoNetworking AtsPki AtsSecurity AtsIS"
for i in ${ATS_LIST}
do
    export ATS=$i
    make
done

echo -e "*****************************\n* Change sudo in command line\n*****************************\n"
cd /home/etsi/dev/TS.ITS/scripts
sed --in-place 's/sudo/echo "etsi" \| sudo -S/' ./run_mtc.bash
sed --in-place 's/sudo/echo "etsi" \| sudo -S/' ./run_ptcs.bash

cd ${HOME}

exit 0
