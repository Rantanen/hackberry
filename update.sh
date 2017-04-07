#!/bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $DIR/vars.inc

# Ensure we are using sudo if required.
SUDO=""
if [ "$EUID" -ne 0 ]
then
	SUDO="sudo"
fi

echo
echo Installing the Hackberry toolset
echo
echo Install log is being written to $HACKBERRY_LOG
echo

deps=()

# Basic tooling.
deps+=( git cmake g++ pkg-config )

# ttyd dependencies
deps+=( libjson-c-dev libssl-dev )

echo -e ${GREEN}Ensuring the following packages are up to date:${CLEAR}
echo ${deps[@]}
echo

# $SUDO apt-get -y install ${deps[@]} | $SUDO debconf-apt-progress
$SUDO debconf-apt-progress -- apt-get -y install ${deps[@]}
# $SUDO apt-get -y install ${deps[@]} | cat

for script in ./init/*.sh
do
	$script update
done

# Install ttyd

# libwebsockets dependency.

# cd ~/.hackberry
# git clone https://github.com/warmcat/libwebsockets libs/libwebsockets
# cd libs/libwebsockets
# mkdir build
# cd build
# cmake ..
# make
# 
# cd ~/.hackberry
# git clone https://github.com/tsl0922/ttyd tools/ttyd
# cd tools/ttyd
# mkdir build
# cd build
# cmake .. -DLibwebsockets_DIR=~/.hackberry/libs/libwebsockets/build
# make
