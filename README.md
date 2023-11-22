# GTK hello world

To compile on Debian:
- ./debian-setup.sh
- ./build.sh

To compile from MSYS2 on Windows:
- ./windows-setup.sh
- ./build.sh

## Theming

To bundle a theme:
- Put it inside dist/win32/share/themes/${theme}
- Open dist/win32/etc/gtk-4.0/settings.ini and set gtk-theme-name=${theme}

