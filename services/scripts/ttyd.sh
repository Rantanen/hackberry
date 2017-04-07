#!/bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $DIR/../../vars.inc

HOME=/home/pi
cd $HOME

$HACKBERRY_DIR/tools/ttyd/build/ttyd -p 80 -u $( id -u pi ) bash --login
