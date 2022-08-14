#!/bin/bash
# Build script for BPI-M2U-BSP

BOARD=$1
mode=$2

usage() {
	cat <<-EOT >&2
	Usage: $0 <board>
	EOT
	./configure
}

if [ $# -eq 0 ]; then
	usage
	exit 1
fi

./configure $BOARD

if [ -f env.sh ] ; then
	. env.sh
fi

make && 
make pack
