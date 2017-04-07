#!/bin/bash
# Hackberry install script for ttyd

# libwebsockets dependency.

HACK_MODULE=ttyd

hack_update () {

	CMAKE_OUTPUT=Makefile

	hack_msg Updating dependency: libwebsockets
	hack_indent

	update_git https://github.com/warmcat/libwebsockets libs/libwebsockets
	rebuild_cmake $? libs/libwebsockets \
		-DLWS_WITHOUT_TESTAPPS=ON \
		-DLWS_WITHOUT_CLIENT=ON

	hack_unindent

	hack_msg Updating ttyd
	hack_indent

	update_git https://github.com/tsl0922/ttyd tools/ttyd
	rebuild_cmake $? tools/ttyd -DLibwebsockets_DIR=${HACKBERRY_DIR}/libs/libwebsockets/build

	hack_unindent
}

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $DIR/init.inc
