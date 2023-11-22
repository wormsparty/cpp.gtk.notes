#!/bin/sh

cd "`dirname $0`"
PREFIX=/usr
MINGW_FOLDER=mingw64

if [ -d /$MINGW_FOLDER ]; then
	echo "mingw64 detected"
	PREFIX="/$MINGW_FOLDER"
	PATH="$PATH:/$PREFIX/bin"
fi

rm -fr ./dist
mkdir -p ./dist

meson setup ./work --prefix $PREFIX
meson compile -C ./work
DESTDIR=./../dist meson install -C ./work

find ./dist -name \*.exe -exec mv {} ./main.exe \;

if [ -f ./main.exe ]; then
	rm -fr ./dist/*
	mv main.exe ./dist/
	echo "Copying DLL..."
	ldd ./dist/main.exe | grep -vi '/c/windows' | grep '/.*.dll' -o | xargs -I{} cp "{}" ./dist
	mkdir -p ./dist/share/glib-2.0
	cp -r "$PREFIX/share/glib-2.0/schemas" ./dist/share/glib-2.0/
	echo "-> Windows package with DLL has been deployed to '$PWD/dist/'"
else
	find ./dist -type f -exec mv {} ./main \;
	rm -fr ./dist/*
	mv ./main dist/main
	echo "-> Executable has been deployed to '$PWD/dist/main'"
fi
