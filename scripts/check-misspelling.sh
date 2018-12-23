#!/usr/bin/env bash
#?
# check-misspelling - Checks to see if any common misspellings are present in 
#                     the source
#
# USAGE
#	check-misspelling
#
# BEHAVIOR
#
#	Looks for the common misspelling of "vgpio", "vpgio"
#?
if grep --color=always -rn vpgio $(dirname $0)/../src; then
	echo "Found \"vpgio\" in source" >&2
	exit 1
fi
