#!/bin/sh
#
# Returns platform dependent variables that can be passed to 'make' via
# command line.

# CONSTANTS ###################################################################
MACOS_PLATFORM="Darwin"
CLANG_CC="clang"

HELP="\
Outputs platform dependent variables for 'make'\n\n\
usage: ..."


# START #######################################################################

## Get options

# If no options, print help
if [ ! "$1" ]; then
	echo $HELP
	exit 0
fi

debug=false
release=false
while getopts ":hdrso:" option; do
	case $option in
		h)
			echo $HELP
			exit 0
			;;
		d)
			debug=true
			;;
		r)
			release=true
			;;
		*)
			echo "Unknown option '-$OPTARG'" >&2
			exit 1
			;;
	esac
done

## Check for invalid argument combinations
if [ "$debug" = true -a "$release" = true ]; then
	echo "Both debug and release modes cannot be set" >&2
	exit 1
fi

## Default arguments
if [ "$debug" = false -a "$release" = false ]; then
	debug=true
fi

## Get platform
platform=$(uname)
cc=
static_cmd=
static_ext=
static_out=
shared_cmd=
shared_ext=
shared_flag=
if [ "$platform" = "$MACOS_PLATFORM" ]; then
	cc=$CLANG_CC
	static_cmd="ar rcs"
	static_ext="a"
	static_out=""
	shared_cmd="$cc -shared"
	shared_ext="dylib"
	shared_out="-o"
fi

## Get compiler specific flags
cflags=
cdebug_flags=
crealease_flags=
if [ "$cc" = "$CLANG_CC" ]; then
	cdebug_flags="-g -c"
	crelease_flags="-O3 -c"

	if [ "$debug" = true ]; then
		cflags=$cdebug_flags
	elif [ "$release" = true ]; then
		cflags=$crelease_flags
	fi
fi

## Echo all variables for 'make'
options="\
CC=\"$cc\" \
CFLAGS=\"$cflags\" \
STATIC_EXT=\"$static_ext\" \
SHARED_EXT=\"$shared_ext\" \
CDEBUG_FLAGS=\"$cdebug_flags\" \
CRELEASE_FLAGS=\"$crelease_flags\" \
SHARED_CMD=\"$shared_cmd\" \
STATIC_CMD=\"$static_cmd\" \
SHARED_OUT=\"$shared_out\" \
STATIC_OUT=\"$static_out\""

echo $options
