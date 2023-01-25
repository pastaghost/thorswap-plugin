#!/bin/bash

GIT_SERVER=https://github.com/LedgerHQ

if [[ "$OSTYPE" == "darwin"* ]]; then
    HOST_OS=macos
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    HOST_OS=linux
else
    echo "Host $OSTYPE not supported. "
    exit -1
fi

# Latest Nano S SDK
export NANOS_SDK=$(pwd)/dev/$HOST_OS/nanos-secure-sdk
if [ ! -e $(pwd)/dev/$HOST_OS/nanos-secure-sdk ]; then
    git clone --branch 2.1.0 --depth 1 "$GIT_SERVER/nanos-secure-sdk.git" "$NANOS_SDK"
fi

# Latest Nano X SDK
export NANOX_SDK=$(pwd)/dev/$HOST_OS/nanox-secure-sdk
if [ ! -e $(pwd)/dev/$HOST_OS/nanox-secure-sdk ]; then
    git clone --branch 2.0.2-2 --depth 1 "$GIT_SERVER/nanox-secure-sdk.git" "$NANOX_SDK"
fi

# Latest Nano S+ SDK
export NANOSP_SDK=$(pwd)/dev/$HOST_OS/nanosplus-secure-sdk
if [ ! -e $(pwd)/dev/$HOST_OS/nanosplus-secure-sdk ]; then
    git clone --branch 1.0.4 --depth 1 "$GIT_SERVER/nanosplus-secure-sdk.git" "$NANOSP_SDK"
fi

# Default SDK
export BOLOS_SDK=$NANOS_SDK

# Install ledger-app-builder Docker image and alias
alias ledger-app-builder="docker run --rm -ti -v "$(realpath .):/app" ledger-app-builder:latest"

# export BOLOS_ENV=$(pwd)/dev
# export ETHEREUM_PLUGIN_SDK=$(pwd)/ethereum-plugin-sdk
