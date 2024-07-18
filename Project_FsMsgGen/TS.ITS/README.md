# ETSI ITS protocols project


## General Information

This repositories contains the test specifications and test adapter code for ETSI ITS protocols testing.
ETSI ITS protocols project supports:
- ETSI EN 302 637-2: "Intelligent Transport Systems (ITS); Vehicular Communications; Basic Set of Applications; Part 2: Specification of Cooperative Awareness Basic Service"
- ETSI EN 102 637-3: "Intelligent Transport Systems (ITS); Vehicular Communications; Basic Set of Applications; Part 3: Specifications of Decentralized Environmental Notification Basic Service" 
- ETSI TS 103 301: "Intelligent Transport Systems (ITS); Vehicular Communications; Basic Set of Applications; Facilities layer protocols and communication requirements for infrastructure services"
- EN 302 636-5-1: "Intelligent Transport Systems (ITS); Vehicular Communications; GeoNetworking; Part 5: Transport Protocols; Sub-part 1: Basic Transport Protocol"
- EN 302 636-4-1: "Intelligent Transport Systems (ITS); Vehicular Communications; GeoNetworking; Part 4: Geographical addressing and forwarding for point-to-point and point-to-multipoint communications; Sub-part 1: Media-Independent Functionality"

In addition, it also support ITS Security as define by:
- ETSI TS 103 940: "Intelligent Transport Systems (ITS); Security; Security Architecture and Management".
- ETSI TS 103 097: "Intelligent Transport Systems (ITS); Security; Security header and certificate formats".
- ETSI TS 102 941: "Intelligent Transport Systems (ITS); Security; Trust and Privacy Management technical specification"
- IEEE Std 1609.2™-2016: "IEEE Standard for Wireless Access in Vehicular Environments –Security Services for Applications and Management Messages"
- IEEE Std 1609.2a™-2017: "Standard for Wireless Access In Vehicular Environments – Security Services for Applications and Management Messages Amendment 1".

Contact information
Email at cti_support at etsi dot org

License
Unless specified otherwise, the content of this repository and the files contained are released under the ETSI Software License.
See the attached LICENSE file or visit
https://forge.etsi.org/etsi-software-license

## STFs

The following STFs were or are currently involved in the evolutions of the ETSI ITS protocols project:
- STF 405, STF 422, STF 424, STF 455, STF 462, STF 481, STF 484, STF 507, STF 517, STF 525, STF 538, STF 545, STF 594, TTF T002, TTF T011


## Installation

The ETSI ITS protocols project builds and tests regularly on the following platforms:

  - Linux (Ubuntu)
  - Windows ([Cygwin x64](https://cygwin.com/install.html), [Npcap SDK x64](https://nmap.org/npcap/#download) and [OpenSSL-Windows x64](https://www.openssl.org) are required)

Note: The [OpenSSL](https://www.openssl.org) version >= 1.1.1 is also required.

### How to do it?

They are three different methods:
- Using [Vagrant](https://www.vagrantup.com/)
- Using [Docker](https://www.docker.com/)
- Using [Eclipse TITAN on Windows or Linux](https://projects.eclipse.org/projects/tools.titan/downloads)

How to choose one of these methods is depending of your host system.

NOTE: In all case, if you want to setup an continuous integration process (e.g. Jenkins), Docker is the best choice. 


#### The host system is Windows
The both methods require a virtual machine. You can use either VirtualBox or WMware.
In this case, the easiest way is to use Vagrant.


#### The host system is Linux
Vagrant requires a virtual machine. You can use either VirtualBox or WMware.
Docker does not need a virtual machine, so it is the more efficant way.


#### Using Vagrant

Pre-requisites on your host machine:
- Install [Virtualbox](https://www.virtualbox.org/manual/ch01.html)
- Install [Vagrant](https://www.vagrantup.com/intro/getting-started/)
- Install Vagrant plugin vagrant-vbguest
- Credentials to access [ETSI forge](https://forge.etsi.org/gitlab/users/sign_in)
    - Set the environment variable USERNAME to your ETSI EOL account user name
    - Set the environment variable PASSWORD to your ETSI EOL account password

Procedure:
- On your host machine, open a command line session (PuTTY, DOS window...)
- From the ETSI ITS protocols project, clone the Vagrant folder
- In the file Vagrantfile, modify the tag config.vm.provision replacing <username> & <password> strings by your ETSI credentials
- In the Vagrant folder (./virtualization/vagrant), execute the following commands:

```sh
$ vagrant up --provider virtualbox --provision
...
```

NOTE The creation and the installations will take some time to achieve
- Stop vagrant virtual machine

```sh
$ vagrant halt
...
```

- Update the file 'Vagrantfile' to match with your networks configuration
- Re-start the vagrant virtual machine and log to to the machine

```sh
$ vagrant up
...
$ vagrant ssh
```

- Switch to the next clause (Usage)

NOTE The user password is vagrant.


### Using Docker

Pre-requisites on your host machine:
- Install Virtualbox (For Windows host only)
- Install Docker

Procedure for a Windows host machine:
- On your host machine, open a the Docker Quickstart Terminal and change to a working folder such as ./temp/docker_its

Procedure for a Linux host machine:
- On your host machine, open a terminal and change to a working folder such as $HOME/temp/docker_its

On your host machine, download the following items from ETSI ITS protocols project:
- The docker folder
- The .jenkins.sh script file (hidden file) and add the execution rights on it
- Check the rights of the script files and the folders

From the your current directory, execute the following commands:

```sh
$ ./.jenkins.sh
...
```

NOTE The creation and the installations will take some time to achieve
- Start the container

```sh
$ ./virtualization/docker/run-container.sh
...
```

- Switch to the next clause (Usage)


#### From scratch

Pre-requisites:
- Install Virtualbox

Procedure:
- Install a new Linux Virtual machine (Mint, Debian...)
- Update your system with the latest version of kernel and security packages
- Install the following packages (According to the Linux chosen, the package naming can be different)
    autoconf
    bison
    build-essential
    cmake
    curl
    dos2unix
    doxygen
    emacs
    expect
    flex
    g++:latest
    gcc:latest
    graphviz
    gdb 
    git-core
    gnutls-bin
    libglib2.0-dev
    libpcap-dev
    libgcrypt-dev
    libncurses5-dev
    libjsoncpp-dev
    libssl-dev
    libtool-bin
    libtool
    libwireshark-dev
    libxml2-dev
    lsof
    ntp
    pkg-config
    qt5-default
    qtmultimedia5-dev
    libqt5svg5-dev
    subversion
    sudo
    sshpass
    tcpdump
    texlive-font-utils
    tshark
    valgrind
    vim
    vsftpd
    xutils-dev 
    tree
    tzdata
    unzip
    wget
    xsltproc
- In your home directory, create the following folders: 
    - $HOME/frameworks, 
    - $HOME/dev
    - $HOME/lib
- In $HOME/frameworks, build the following package:
    - asn1c, according the procedure specified [here](https://github.com/vlm/asn1c.git)
    - Eclipse IDE for C/C++ Developers, according the procedure specified [here](https://www.eclipse.org/cdt/)
    - TITAN, according the procedure specified [here](https://github.com/eclipse/titan.core)
    - Import the TITAN plugin into your Eclipse IDE, according the procedure specified [here](https://github.com/eclipse/titan.core)
- Clone the ETSI ITS protocols project into $HOME/dev folder

```sh
$ git clone --recurse-submodules --single-branch https://forge.etsi.org/gitlab/ITS/TS.ITS.git ./TTF011_Its
```

- Update your default environment with the content of the script $HOME/dev/TTF011_Its/scripts/devenv.bash.ubuntu
- Switch to the next clause (Usage)


#### Using Eclipse TITAN

When cloning the ETSI ITS project, you can find two specific files:
- TTF011.tpd
- TTF011_Linux.tpd

These two files are used create the ETSI ITS project workspace on Eclipse TITAN. To do it, please follow the steps below:
- Start eclipse using a new workspace, (e.g. with the name workspace_titan)
- Select the menu option File/import
- In the Dialog box,select TITAN/Project from .tpd file
- Select the correct TTF011.tpd file and follow the instructions

NOTE: When the Eclipse TITAN workspace is created, you have to build manually the librairy 'libasn1c.so' following the commands below:
```sh
$ cd <eclipse workspace>/TTF011_Its
$ mkdir -p ./bin/asn1
$ cd ./bin/asn1
$ make CC=gcc -f ../../asn1/Makefile
$ rm *.cc # To prevent TITAN to import these .cc source files into our project
```


## Usage

This clause describes how to compile and execute an Abstract Test Suite.
The procedures below illustrate how to run the CAM test suite. The same procedures will apply for any other ETSI ITS test suite.


Pre-requisites:
- Your machine is installed following one of the installation method describes in the previous clause
- Refer to the ETSI TS 103 099 for the description of the Test System architecture and configuration
- Your are logged as 'etsi' or 'vagrant' user
- Procedure using TITAN command line (only):
    - Open several SSH session (PuTTY...)
    - Change to the directory ~/dev/TTF011_Its/
    - Modify the file config.mk according to your system:
        - On Linux, comment all the lines using the '#' character
        - On Windows, update the path accordingly
    - Build the test suite AtsCAM using the following command:

```sh
$ export ATS=AtsCAM # The Abstract Test Suite you want to build, such as AtsDENM, AtsSecurity...
$ make
...
```

- Edit the file ../etc/AtsCAM.cfg
- Update the following fields (see ETSI TS 103 099 for details):
    - system.camPort.params
    - system.utPort.params
- To run the test suitem, execute the following command:

```sh
$ cd ~/dev/TTF011_Its/scripts
$ ../run_all.bash
...
```

- The log files are located in ../logs/AtsCAM folder for this example. You can edit them using any editor or using the Eclipse TITAN log plugins

## How to generate ITS test certificates

The Test System requires the 'itscertgen' tool to generate ITS test certificates used for Conformance Testing.
This tool is located [here](https://forge.etsi.org/rep/ITS/itscertgen.git).

```sh
 cd ~/dev
$ git clone --recurse-submodules --single-branch https://forge.etsi.org/rep/ITS/itscertgen.git itscertgen
cd itscertgen
make
```

Note: These certificates can not be used in a true architectures, there are present only for testing and/or debug purposes.


### Build the tool 'itscertgen' to generate new certificates bundle

To build the tool, run the 'make':

```sh
$ cd ~/dev/itscertgen
$ make
```


### Generate the certificates

After applying the previous clause, change to the folder '~/dev/TTF011_Its/data/certificates' and execute 'make' command:
```
$ cd ~/dev/TTF011_Its/data/certificates
$ make
```
The certificates will be located in the folder '~/dev/TTF011_Its/data/certificates/certificates'.

To use this newly generated certificates, you shall update two parameters located in the TTCN-3 file LibItsSecurity_Pixits. These are:
- LibItsSecurity_Pixits.PX_CERTIFICATE_POOL_PATH, which is the path the certificates folder (e.g. LibItsSecurity_Pixits.PX_CERTIFICATE_POOL_PATH := "/home/<user>/dev/TTF011_Its/data/certificates")
- LibItsSecurity_Pixits.PX_IUT_SEC_CONFIG_NAME, which is the name of the certificates folder (e.g. LibItsSecurity_Pixits.PX_IUT_SEC_CONFIG_NAME := "certificates")


### Modify or create new certificates

The folder '~/dev/TTF011_Its/data/profiles' contains an XML file for each certificate to be generated.
This XML file describes the certificate content (e.g. CERT_IUT_A_RCA.xml describes the root certificate for all CERT_IUT_A certificates).

By modifying these files, you can change create new certificate with different geographical area, different validity periods or different SSPs.

To re-generates the certificates, refer to the previous clause.


## Security Test suites configurations

The security Test suites configurations depends of the Test suite to be executed.

### ITS Protocol Test suites for Facilities layer

These are the ITS Test suites such as AtsCAM (102 868-x) , AtsDENM (102 869-x) or IVI (103 191-x) Test suites. These Test suite can be executed againt an ITS device configured to use Security.
In this cases, the following parameters shall be modified to match the ITS device configuration:
- PICS_IS_IUT_SECURED=true which indicates that the ITS device under test is secured

In addition, the Test System shall be configured to support the security modifying the following settings as described below:
- device_mode=1
- secured_mode=1
- sec_db_path=/home/<user>/dev/TTF011_Its/data/certificates/certificates


### ITS Protocol Test suites for Transport layer

This is the AtsGeoNetworking Test suite (ETSI TS 102 871-x) which required more specific configuration (ETSI EN 302 636-4-1).
In this cases, the following parameters shall be modified to match the ITS device configuration:
- PICS_GN_SECURITY=false, which indicate the GeoNetworking Test Suite does not manage ITS Security protocol
- PICS_IS_IUT_SECURED=true, which indicates that the ITS device under test is secured

In addition, the Test System shall be configured to support the security modifying the following settings as described below:
- device_mode=1
- secured_mode=1
- sec_db_path=/home/<user>/dev/TTF011_Its/data/certificates/certificates


### ITS Protocol Test suites for Security

This clause is relative to the AtsSecurity (ETSI TS 103 096-x) Test suite which are focused on ITS Security protocol (ETSI TS 103 097).
In this cases, the following parameters shall be modified to match the ITS device configuration:
- PICS_GN_SECURITY=true, which indicate the GeoNetworking Test Suite manages ITS Security protocol
- PICS_IS_IUT_SECURED=false

In addition, the Test System shall be configured to support the security modifying the following settings as described below:
- device_mode=0
- secured_mode=0
- its_aid=36 for CAM or 37 for DENM


### ITS Protocol Test suites for Security/PKI

This clause is relative to the AtsPki (ETSI TR 103 525-x) Test suite which are focused on ITS PKI Security protocol (ETSI TS 102 941).
In this cases, the following parameters shall be modified according to the considered group of tests:
- For all tests focused on ITS device enrolment:
    - PICS_GN_SECURITY=true, which indicate the GeoNetworking Test Suite manages ITS Security protocol
    - PICS_IS_IUT_SECURED=false
    - HTTP_P_PL_ACT_AS_SERVER=true
    - PICS_TS_EA_CERTIFICATE_ID=CERT_TS_A_EA
    - PICS_MULTIPLE_END_POINT=false
    - PICS_IUT_ITS_S_ROLE=true
- For all tests focused on PKI:
    - HTTP_P_PL_ACT_AS_SERVER=true
    - PICS_TS_EA_CERTIFICATE_ID=CERT_pki certificate_EA
    - PICS_MULTIPLE_END_POINT=true


In addition, the Test System shall be configured to support the security modifying the following settings as described below:
- device_mode=0
- secured_mode=0


## Wireshark with support of ETSI ITS Protocols

The official version of Wireshark, supporting ETSI ITS Protocols, is available [here](https://www.wireshark.org/download.html).
Some sample capture files are available [here](https://wiki.wireshark.org/SampleCaptures).


## How to Report a Bug

The ETSI ITS protocols project is under constant development, so it is possible that you will
encounter a bug while using it. Please report bugs at cti_support at etsi dot org.
