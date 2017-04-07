#!/bin/bash

: ${HACKBERRY_DIR:="$HOME/.hackberry"}
: ${HACKBERRY_GIT:="https://github.com/Rantanen/hackberry"}

if cd $HACKBERRY_DIR
then
	git pull
else
	git clone $HACKBERRY_GIT $HACKBERRY_DIR
	cd $HACKBERRY_DIR
fi

./update.sh
