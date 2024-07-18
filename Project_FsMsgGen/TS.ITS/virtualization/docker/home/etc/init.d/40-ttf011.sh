#!/bin/bash

#set -e
set -vx

echo -e "*****************************\n* Checkout TS.ITS sources\n*****************************\n"
cd ${HOME}/dev || exit 1

git clone --recurse-submodules --single-branch https://forge.etsi.org/rep/ITS/TS.ITS.git TS.ITS
cd ./TS.ITS
for i in `find ./ttcn -type d -name "Ats*"`
do
    cd $i
    git checkout devel
    cd -
done
for i in `find ./ttcn/Ats* -type d -name "asn1"`
do
    cd $i
    git checkout testing
    SUB_DIRS=`find . -type d`
    if [ "SUB_DIRS" != "" ]
    then
        for j in $SUB_DIRS
        do
            cd $j
            git checkout testing
            cd -
        done
    fi
    cd -
done
cd ${HOME}/dev/TS.ITS/ttcn/LibIts
git checkout TS.ITS
cd ${HOME}/dev/TS.ITS/scripts
chmod 775 *.bash devenv.bash.*
cd ${HOME}
ln -sf ${HOME}/dev/TS.ITS/scripts/devenv.bash.ubuntu ${HOME}/devenv.bash
ls -ltr ${HOME}

cd ${HOME}

exit 0
