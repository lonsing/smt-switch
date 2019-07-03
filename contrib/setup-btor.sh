#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
BTOR_HOME=$DIR/../btor

if [ ! -d "$BTOR_HOME/boolector" ]; then
    cd $BTOR_HOME
    git clone https://github.com/Boolector/boolector.git
    chmod -R 777 boolector
    cd boolector
    ./contrib/setup-btor2tools.sh
    ./contrib/setup-lingeling.sh
    ./configure.sh --only-lingeling -fPIC
    cd build
    make -j$(nproc)
    cd $DIR
fi

if [ -f $BTOR_HOME/boolector/build/lib/libboolector.a ] && [ -f $BTOR_HOME/boolector/deps/lingeling/liblgl.a ] && [ -f $BTOR_HOME/boolector/deps/btor2tools/build/btor2parser.o ] ; then \
    echo "It appears boolector was setup successfully into $BTOR_HOME/boolector."
    echo "You may now install it with make install-btor"
else
    echo "Building boolector failed."
    echo "You might be missing some dependencies."
    echo "Please see their github page for installation instructions: https://github.com/Boolector/boolector"
fi
