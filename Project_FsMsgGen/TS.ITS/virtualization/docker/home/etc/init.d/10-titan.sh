#!/bin/bash

#set -e
set -vx

echo -e "*****************************\n* Install titan\n*****************************\n"
SRC_DIR=${HOME}/frameworks/titan/titan.core
export TTCN3_DIR=$HOME/frameworks/titan/Install

[ -f /usr/bin/java ] &&  export JAVA_HOME=`readlink -f /usr/bin/java | sed "s:bin/java::"`
TITAN_REPO=`cat ${HOME}/etc/titan_repos.txt | grep 'titan\.core\.git' | head -n 1`
export PATH=$PATH:$TTCN3_DIR/bin

mkdir -p "$SRC_DIR"
cd "$SRC_DIR/.." || exit 1

# Install titan core
git clone --progress "$TITAN_REPO" || exit 1

cd titan.core || exit 1
cat >Makefile.personal <<EOF
TTCN3_DIR=$TTCN3_DIR
JNI=no
GUI=no
DEBUG=no
GEN_PDF=no
JDKDIR=$JAVA_HOME
EOF

make && make install || exit 1

# To be compiant with build_ttcn3.bash
# TODO Refactor build_ttcn3.bash & devenv.bash
mv ${TTCN3_DIR} ${SRC_DIR}
export TTCN3_DIR=${SRC_DIR}/Install

echo "export TTCN3_DIR=$TTCN3_DIR" >> $HOME/.bashrc
echo "export PATH=\$PATH:\$TTCN3_DIR/bin" >> $HOME/.bashrc

# Install other repos
cd $SRC_DIR/.. || exit 1
cat ${HOME}/etc/titan_repos.txt | grep -v -e '^\s*#' -e 'titan\.core' | while read REPO; do
  [ -z $ $REPO ] && continue
  WS=`echo $REPO | sed -e 's|.*/||g' -e 's|\.git||g'`
  if git clone --progress "${REPO}" "${WS}"; then
    [ -f "$WS/Makefile" ] && make -C "$WS"
  fi
done

cd $HOME
