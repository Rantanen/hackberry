
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
CLEAR='\033[0m'

ERROR=${RED}ERROR:${CLEAR}
WARN=${YELLOW}WARNING:${CLEAR}

: ${INDENT:=0}

# We know the vars.inc is in the hackberry root directory.
HACKBERRY_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

: ${HACKBERRY_LOG:="$HACKBERRY_DIR/hackberry.log"}

HACKBERRY_LOG=$( realpath $HACKBERRY_LOG )

hack_msg() {
	echo >> $HACKBERRY_LOG
	echo -n "MSG: " >> $HACKBERRY_LOG

	# Add the indent.
	if [ $INDENT -ne 0 ]
	then
		printf "%-${INDENT}s" | tee -a $HACKBERRY_LOG
	fi

	echo -e "$@" |& tee -a $HACKBERRY_LOG
}

hack_indent() {
	let INDENT+=2
}
hack_unindent() {
	let INDENT-=2
}

hack_run() {
	echo >> $HACKBERRY_LOG
	echo "CMD: $(pwd) $@" >> $HACKBERRY_LOG

	if [ $VERBOSE ]
	then
		$@ |& tee -a $HACKBERRY_LOG
	else
		$@ >> $HACKBERRY_LOG 2>&1
	fi

	# Use pipestatus as 'tee' might modify $?
	if [ ${PIPESTATUS[0]} -ne 0 ]
	then
		CODE=${PIPESTATUS[0]}
		INDENT=0
		hack_msg
		hack_msg "$ERROR Command failed:"
		hack_msg "       $@"
		hack_msg "       See $HACKBERRY_LOG for more information"
		exit $CODE
	fi
}

hack_pushd() {

	echo >> $HACKBERRY_LOG
	echo "PUSHD: $1" >> $HACKBERRY_LOG
	pushd $1 > /dev/null

	if [ $VERBOSE ]
	then
		echo
		echo "PUSHD: $1"
		echo
	fi
}

hack_popd() {

	popd > /dev/null
	echo >> $HACKBERRY_LOG
	echo "POPD: $(pwd)" >> $HACKBERRY_LOG

	if [ $VERBOSE ]
	then
		echo
		echo "POPD: $1"
		echo
	fi
}
