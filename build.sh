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
	# Fix the paths
	rm -fr ./dist
	mkdir -p ./dist/win32
	mv main.exe ./dist/win32/
	cp ./win32/run.lnk ./dist/

	# Add the DLL for distribution
	echo "Copying DLL..."
	ldd ./dist/win32/main.exe | grep -vi '/c/windows' | grep '/.*.dll' -o | xargs -I{} cp "{}" ./dist/win32
	
	# Add the schemas, otherwise it crashed when opening some dialogs
	mkdir -p ./dist/win32/share/glib-2.0
	cp -r "$PREFIX/share/glib-2.0/schemas" ./dist/win32/share/glib-2.0/
	glib-compile-schemas ./dist/win32/share/glib-2.0/schemas
	find ./dist/win32/share/glib-2.0/schemas -not -name gschemas.compiled -exec rm {} \;
	
	mkdir -p ./dist/win32/etc/gtk-4.0
	cat << EOT > ./dist/win32/etc/gtk-4.0/settings.ini
[Settings]
gtk-theme-name=win32
gtk-font-name=Segoe UI 9
EOT
	
	cp -r "$PREFIX/share/icons" ./dist/win32/share/

	echo "-> Windows package with DLL has been deployed to '$PWD/dist/'"
else
	find ./dist -type f -exec mv {} ./main \;
	rm -fr ./dist/*
	mv ./main dist/main
	echo "-> Executable has been deployed to '$PWD/dist/main'"
fi
