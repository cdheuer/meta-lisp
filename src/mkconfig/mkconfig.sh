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

## Parse options
debug=false
release=false
while getopts ":hdr" option; do
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
if [ "$platform" = "$MACOS_PLATFORM" ]; then
	cc=$CLANG_CC
fi

## Get compiler specific flags
cflags=
outflag=
if [ "$cc" = "$CLANG_CC" ]; then
	if [ "$debug" = true ]; then
		cflags="-g -c"
	elif [ "$release" = true ]; then
		cflags="-O3 -c"
	fi

	outflag="-o"
fi

## Output variables as options for 'make'
options="\
\"CC=$cc\" \
\"CFLAGS=$cflags\"
\"OUTFLAG=$outflag\""

echo $options

