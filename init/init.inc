
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
: ${HACKBERRY_DIR:=$DIR/..}
: ${INDENT:=0}
source $DIR/../vars.inc

hack_msg() {
	echo >> $HACKBERRY_LOG
	echo -n "MSG: " >> $HACKBERRY_LOG
	printf ' %.0s' {1..$INDENT} |& tee -a $HACKBERRY_LOG
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

update_git() {

	# $1 - Git repository
	# $2 - Target directory

	if [ -d "$2" ]
	then
		hack_pushd $2
		hack_run git fetch origin

		# Try to resolve the upstream revision.
		remote_ref=$( git rev-parse @{u} 2>/dev/null )
		if [ $? -eq 128 ]
		then
			# Failed to find upstream revision. No automatic update.
			hack_info $WARN $2 is not on a branch. Will not update.
			hack_popd
			return 0
		fi

		if [ $( git rev-parse HEAD ) = $( git rev-parse @{u} ) ]
		then
			hack_popd
			return 0
		fi

		hack_msg "  Pulling new changes"
		hack_run git pull
		hack_popd
	else
		hack_msg "  Cloning repository $1"
		hack_run git clone $1 $2
	fi

	return 1
}

rebuild_cmake () {

	# $1 - Force
	# $2 - Project root
	# $3... - cmake options

	hack_pushd $2

	# Clean the build directory if forced.
	if [ $1 -ne 0 ] && [ -d "build" ]
	then
		hack_msg "  Cleaning previous build"
		hack_run rm -rf build

	fi

	# Rebuild
	hack_run mkdir -p build
	hack_pushd build

	hack_msg "  Running cmake..."
	hack_run cmake .. ${@:3}
	hack_msg "  Running make..."
	hack_run make

	# Build and project root
	hack_popd
	hack_popd
}

echo -e "Updating module: ${GREEN}${HACK_MODULE}${CLEAR}"
echo
hack_indent
hack_update
hack_unindent
echo
echo -e "    ${GREEN}Ok!${CLEAR}"
echo
