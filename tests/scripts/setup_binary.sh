#!/usr/bin/env bash
set -e

################################################################
#                   Replace the variables                      #
################################################################
NAME=tests
VAULT_DIR=$(cd `dirname $0` && pwd)
BUILD_COMMAND=make
OUTPUT_DIR=$(pwd)
TEST_DIR=$CMAKE_SOURCE_DIR/test_binary/tests
EYRIE_DIR=$CMAKE_BINARY_DIR/eyrie
EYRIE_PLUGINS="freemem paging"

# Download Eyrie Runtime
rm -rf $EYRIE_DIR
if [ ! -d $EYRIE_DIR ]
then
  #git clone https://github.com/iaclab-hpme/keystone-runtime $EYRIE_DIR
  cp -r /home/yintianyu/code/iaclab-hpme/keystone-runtime $EYRIE_DIR
fi
cd $EYRIE_DIR; cd ..

################################################################
#                       Build Enclave                          #
################################################################
# create a build directory
OUTPUT_FILES_DIR=$OUTPUT_DIR
mkdir -p $OUTPUT_FILES_DIR

# build eyrie runtime
$EYRIE_DIR/build.sh $EYRIE_PLUGINS

make -C $TEST_DIR

cp $TEST_DIR/stack/stack.eapp_riscv $OUTPUT_FILES_DIR
cp $EYRIE_DIR/eyrie-rt $OUTPUT_FILES_DIR

