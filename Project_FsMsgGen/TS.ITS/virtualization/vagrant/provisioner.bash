#!/bin/bash
# Prepare environment for the build
#set -e # Exit with non 0 if any command fails
#set -vx

# Update system
sudo DEBIAN_FRONTEND=noninteractive apt-get update
sudo DEBIAN_FRONTEND=noninteractive apt-get dist-upgrade -y
sudo DEBIAN_FRONTEND=noninteractive apt-get install software-properties-common -y
sudo DEBIAN_FRONTEND=noninteractive apt-get update
sudo DEBIAN_FRONTEND=noninteractive apt-get --allow-unauthenticated install emacs gcc-9 g++-9 git subversion lsof ntp gdb make cmake flex bison autoconf doxygen graphviz libtool libncurses5-dev expect libssl-dev libgcrypt-dev libxml2-dev libedit2 libedit-dev xutils-dev libjsoncpp-dev tcpdump libpcap-dev unzip sshpass kubuntu-desktop valgrind vim tzdata dos2unix xsltproc -y
#sudo DEBIAN_FRONTEND=noninteractive apt --fix-broken install -y
sudo DEBIAN_FRONTEND=noninteractive apt-get autoremove --purge -y
sudo DEBIAN_FRONTEND=noninteractive apt-get clean

gcc --version
g++ --version
valgrind --version

export HOME=/home/vagrant

export PATH_DEV=${HOME}/dev
export HOME_FRAMEWORKS=${HOME}/frameworks
export HOME_LIB=${HOME}/lib
export HOME_BIN=${HOME}/bin
export HOME_ETC=${HOME}/etc
export HOME_INC=${HOME}/include
export HOME_TMP=${HOME}/tmp
export HOME_DOCS=${HOME}/docs
export PATH=${HOME_BIN}:${PATH}
export LD_LIBRARY_PATH=${HOME_LIB}:/usr/local/lib:${LD_LIBRARY_PATH}

OLD_PWD=`pwd`
# Create directories
mkdir -p ${HOME_LIB}
if [ ! -d ${HOME_LIB} ]
then
    exit -1
fi
mkdir -p ${HOME_INC}
if [ ! -d ${HOME_INC} ]
then
    exit -1
fi
mkdir -p ${HOME_BIN}
if [ ! -d ${HOME_BIN} ]
then
    exit -1
fi
mkdir -p ${HOME_TMP}
if [ ! -d ${HOME_TMP} ]
then
    exit -1
fi
mkdir -p ${HOME_DOCS}
if [ ! -d ${HOME_DOCS} ]
then
    exit -1
fi
mkdir -p ${HOME_FRAMEWORKS}
if [ ! -d ${HOME_FRAMEWORKS} ]
then
    exit -1
fi
mkdir -p ${PATH_DEV}
if [ ! -d ${PATH_DEV} ]
then
    exit -1
fi

cd /home/vagrant/dev
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
cd /home/vagrant/dev/TS.ITS/ttcn/LibIts
git checkout TS.ITS

cd /home/vagrant/dev/TS.ITS/scripts
chmod 775 *.bash devenv.bash.*
./update_user_name.sh
cd /home/vagrant/dev/TS.ITS/docker
chmod 775 *.sh
cd /home/vagrant/dev/TS.ITS
chmod 775 ./.jenkins.sh
cd /home/vagrant
ln -sf /home/vagrant/dev/TS.ITS/scripts/devenv.bash.ubuntu /home/vagrant/devenv.bash
. /home/vagrant/devenv.bash

# Install all frameworks

# Install asn1c
cd ${HOME_FRAMEWORKS}
git clone https://github.com/fillabs/asn1c.git asn1c
cd ${HOME_FRAMEWORKS}/asn1c
test -f configure || autoreconf -iv
./configure
make && sudo make install
cd -

# Install GoogleTest
cd ${HOME_FRAMEWORKS}
git clone https://github.com/google/googletest.git googletest
cd ${HOME_FRAMEWORKS}/googletest/
cmake .
make CXX=g++
sudo make install

# Install latest LCOV
cd ${HOME_FRAMEWORKS}
mkdir -p ${HOME_FRAMEWORKS}/lcov
cd ${HOME_FRAMEWORKS}/lcov
wget http://ftp.de.debian.org/debian/pool/main/l/lcov/lcov_1.13.orig.tar.gz
tar xf lcov_1.13.orig.tar.gz
sudo make -C lcov-1.13/ install

# Install lcov to coveralls conversion
sudo gem install coveralls-lcov

lcov --version
coveralls-lcov -h

cd /home/vagrant
echo "" >> /home/vagrant/.bashrc 
echo "export LD_LIBRARY_PATH=/home/vagrant/dev/TS.ITS/lib:$LD_LIBRARY_PATH" >> /home/vagrant/.bashrc 
echo "export PATH=/home/vagrant/bin:$PATH" >> /home/vagrant/.bashrc 
echo ". ~/devenv.bash" >> /home/vagrant/.bashrc

. /home/vagrant/.bashrc

# Build Titan
cd /home/vagrant/dev/TS.ITS/scripts
./build_titan.bash
. /home/vagrant/devenv.bash

# Build ATSs
cd /home/vagrant/dev/TS.ITS/
ATS_LIST="AtsCAM AtsDENM AtsGeoNetworking AtsPki AtsSecurity AtsIS"
for i in ${ATS_LIST}
do
    export ATS=$i
    make
done

# Change user in cfg files
cd ${HOME}/dev/TS.ITS/scripts
./update_user_name.sh TS.ITS

# Build ETSI ITS certificate generation tools
cd /home/vagrant/dev
git clone --recurse-submodules https://forge.etsi.org/rep/ITS/itscertgen.git ./itscertgen
cd ./itscertgen
cd cshared && make && cd -
cd certgen/ && make && cd -
# Generate ETSI ITS certificates
cd /home/vagrant/dev/TS.ITS/data/certificates
CERTGEN=${HOME}/dev/itscertgen make

cd ${OLD_PWD}

sudo init 6

exit 0
