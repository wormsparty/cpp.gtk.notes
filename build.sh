#!/bin/sh

cd "`dirname $0`"
PREFIX=

if [ -d /mingw64 ]; then
	export PATH="$PATH:/mingw64/bin"
	export PKG_CONFIG="x86_64-w64-mingw32-pkg-config"
	PREFIX="--prefix /mingw64"
fi

rm -fr ./bin
mkdir -p ./bin

meson setup ./work $PREFIX
meson compile -C ./work
DESTDIR=./../bin meson install -C ./work
