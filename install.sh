#!/bin/bash

# Defaults.
: ${HACKBERRY_DIR:="$HOME/.hackberry"}
: ${HACKBERRY_GIT:="https://github.com/Rantanen/hackberry"}

# Ensure HACKBERRY_DIR is absolute just in case.
HACKBERRY_DIR=$(realpath $HACKBERRY_DIR)

# Update the local repository.
if [ -d $HACKBERRY_DIR ]
then
	cd $HACKBERRY_DIR
	git pull
	cd ..
else
	git clone $HACKBERRY_GIT $HACKBERRY_DIR
fi

echo

# Bootstrap done. Delegate to the normal update.
./update.sh
