#!/bin/bash

#BTOR_VERSION=ee44ed2eadd1eef51549ac7236e6a4f02f24f12a
# version: Wed May 20
BTOR_VERSION=59c9ade58bdcef51e07b2239972d35647b40cadb

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
DEPS=$DIR/../deps

mkdir -p $DEPS

if [ ! -d "$DEPS/boolector" ]; then
    cd $DEPS
    git clone https://github.com/Boolector/boolector.git
    chmod -R 777 boolector
    cd boolector
    git checkout -f $BTOR_VERSION
    CFLAGS=""
    ./contrib/setup-btor2tools.sh
    ./contrib/setup-cadical.sh
# compile with debug info
    ./configure.sh -g --only-cadical -fPIC
    cd build
    make -j$(nproc)
    cd $DIR
else
    echo "$DEPS/boolector already exists. If you want to rebuild, please remove it manually."
fi

if [ -f $DEPS/boolector/build/lib/libboolector.a ] && [ -f $DEPS/boolector/deps/cadical/build/libcadical.a ] && [ -f $DEPS/boolector/deps/btor2tools/build/btor2parser.o ] ; then \
    echo "It appears boolector was setup successfully into $DEPS/boolector."
    echo "You may now install it with make ./configure.sh --btor && cd build && make"
else
    echo "Building boolector failed."
    echo "You might be missing some dependencies."
    echo "Please see their github page for installation instructions: https://github.com/Boolector/boolector"
    exit 1
fi
