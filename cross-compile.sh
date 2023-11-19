#!/bin/sh

cd "`dirname $0`"

if ! which "x86_64-w64-mingw32-g++"; then
	sudo apt-get install mingw-w64 python3-pip python3-venv wine bison flex doxygen xsltproc
	./debian-setup.sh
fi


if [ ! -d meson ]; then
	python3 -m venv meson
	meson/bin/pip3 install meson
fi

meson/bin/meson setup --cross-file cross.meson build-mingw
meson/bin/meson compile -C build-mingw

rm -fr ./dist
mkdir -p ./dist
DESTDIR=./../dist meson/bin/meson install -C build-mingw
	
mv ./dist/usr/local/bin ./tmp
rm -fr ./dist/
mv ./tmp ./dist
find ./dist -name \*.exe -not -name 'gnometest.exe' -exec rm {} \;

cp /usr/lib/gcc/x86_64-w64-mingw32/12-win32/libssp-0.dll ./dist/
cp /usr/lib/gcc/x86_64-w64-mingw32/12-win32/libstdc++-6.dll ./dist/
cp /usr/lib/gcc/x86_64-w64-mingw32/12-win32/libgcc_s_seh-1.dll ./dist/

echo "-> Windows package with DLL has been deployed to '$PWD/dist/'"
