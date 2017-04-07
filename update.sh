#!/bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $DIR/common.inc
cd $HACKBERRY_DIR
pwd

# Ensure we are using sudo if required.
SUDO=""
if [ "$EUID" -ne 0 ]
then
	SUDO="sudo"
fi

hack_msg
hack_msg Installing the Hackberry toolset
hack_msg
hack_msg Install log is being written to $HACKBERRY_LOG
hack_msg

# Ensure .profile is up to date.
if grep "^export HACKBERRY_DIR=" $HOME/.profile &> /dev/null
then
	if ! grep "^export HACKBERRY_DIR=$HACKBERRY_DIR$" $HOME/.profile &> /dev/null
	then
		hack_msg "Updating HACKBERRY_DIR=$HACKBERRY_DIR in .profile"
		sed -i "s#^export HACKBERRY_DIR=.*#HACKBERRY_DIR=$HACKBERRY_DIR#" $HOME/.profile
		hack_msg
	fi
else
	hack_msg "Adding $HACKBERRY_DIR to .profile"
	echo >> $HOME/.profile
	echo "# Hackberry profile" >> $HOME/.profile
	echo "export HACKBERRY_DIR=$HACKBERRY_DIR" >> $HOME/.profile
	echo "source $HACKBERRY_DIR/profile" >> $HOME/.profile
	hack_msg
fi

deps=()

if [ "$APT_GET" != "0" ]
then

	# Basic tooling.
	deps+=( git cmake g++ pkg-config )

	# ttyd dependencies
	deps+=( libjson-c-dev libssl-dev )

	hack_msg ${GREEN}Ensuring the following packages are up to date:${CLEAR}
	hack_msg ${deps[@]}
	hack_msg

	$SUDO debconf-apt-progress -- apt-get -y install ${deps[@]}
fi

# Run the init scripts.
hack_msg Running init scripts...
hack_msg
hack_indent
for script in $HACKBERRY_DIR/init/*.sh
do
	$script update
	hack_msg
done
hack_unindent

# Ensure the hackberry conf.d is up to date.
echo HACKBERRY_DIR=$HACKBERRY_DIR | $SUDO tee /etc/default/hackberry > /dev/null

cp -rfT $HACKBERRY_DIR/services/units.templates $HACKBERRY_DIR/services/units.current
sed -i "s#\$HACKBERRY_DIR#$HACKBERRY_DIR#g" "$HACKBERRY_DIR"/services/units.current/*.service

hack_msg Adding services...
for service in $HACKBERRY_DIR/services/units.current/*.service
do
	sudo systemctl enable $service || exit $?
	sudo systemctl start $( basename $service )
done
