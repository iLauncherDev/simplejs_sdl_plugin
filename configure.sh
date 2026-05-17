#!/bin/bash
set -e

# Use: ./configure.sh [-toolchain ARCH]
TOOLCHAIN="mingw-amd64"  # Default toolchain
OUTDIR=""

while [[ $# -gt 0 ]]; do
    case "$1" in
        -toolchain)
            TOOLCHAIN="$2"
            shift 2
            ;;
        *)
            shift
            ;;
    esac
done

OUTDIR="output-${TOOLCHAIN}"
SDKDIR="../simplejs_src/sdk/toolchain"

mkdir -p "$OUTDIR"
cd "$OUTDIR"

cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_TOOLCHAIN_FILE=${SDKDIR}/${TOOLCHAIN}.cmake ..
cd ..
