#!/bin/sh

cd "`dirname $0`"
PREFIX=

if [ -d /mingw64 ]; then
	export PATH="$PATH:/mingw64/bin"
	export PKG_CONFIG="x86_64-w64-mingw32-pkg-config"
	PREFIX="--prefix /mingw64"
fi

rm -fr ./dist
mkdir -p ./dist

meson setup ./work $PREFIX
meson compile -C ./work
DESTDIR=./../dist meson install -C ./work

if [ `uname -o` = 'Msys' ]; then
	find ./dist -name \*.exe -exec mv {} ./main.exe \;
	rm -fr ./dist/*
	mv main.exe ./dist/
	echo "Copying DLL..."
	ldd ./dist/main.exe | grep -vi '/c/windows' | grep '/.*.dll' -o | xargs -I{} cp "{}" ./dist
	echo "-> Windows package with DLL has been deployed to '$PWD/dist/'"
else
	find ./dist -type f -exec mv {} ./main \;
	rm -fr ./dist/*
	mv ./main dist/main
	echo "-> Executable has been deployed to '$PWD/dist/main'"
fi
