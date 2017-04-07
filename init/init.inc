
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
HACKBERRY_DIR=$( realpath $DIR/.. )
source $HACKBERRY_DIR/common.inc

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

		hack_msg Pulling new changes
		hack_run git pull
		hack_popd
	else
		hack_msg Cloning repository $1
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
		hack_msg "Cleaning previous build"
		hack_run rm -rf build
	fi

	# Ensure the build directory exists.
	hack_run mkdir -p build
	hack_pushd build

	# If we don't have the expected cmake output, run cmake.
	#
	# Either this is a new build or the previous cmake failed.
	if [ -z "$CMAKE_OUTPUT" ] || [ ! -f $CMAKE_OUTPUT ]
	then
		hack_msg Running cmake...
		hack_run cmake .. ${@:3}

		MAKE_MSG="Build updated. Running make..."
	else
		MAKE_MSG="Running make to ensure everything is up to date..."
	fi

	hack_msg $MAKE_MSG
	hack_run make

	# Build and project root
	hack_popd
	hack_popd
}

hack_msg Updating module: ${GREEN}${HACK_MODULE}${CLEAR}
hack_msg

hack_indent

hack_update

hack_msg
hack_msg ${GREEN}Ok!${CLEAR}
hack_msg

hack_unindent
