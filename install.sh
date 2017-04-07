#!/bin/bash

: ${HACKBERRY_DIR:="$HOME/.hackberry"}
: ${HACKBERRY_GIT:="https://github.com/Rantanen/hackberry"}

echo $HACKBERRY_DIR

if cd $HACKBERRY_DIR
then
	pwd
	git pull
else
	pwd
	git clone $HACKBERRY_GIT $HACKBERRY_DIR
	cd $HACKBERRY_DIR
fi

./update.sh
