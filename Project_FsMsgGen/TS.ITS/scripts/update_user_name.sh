#!/bin/bash

# Debug mode
#set -e
#set -vx

# Usage: sudo ./update_user_name.bash
# TODO Use git clone in temporary directory

USER_NAME='s/etsi/vagrant/g'
for i in `find /home/vagrant/dev/$1 -name "*.cf*"`
do
  sed --in-place ${USER_NAME} $i
done

exit 0
