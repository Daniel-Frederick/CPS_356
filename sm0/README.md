# How to run Program

install meson and ninja:
sudo apt install meson ninja-buld
Note: If you don't want to install meson and ninja, just use the countUD executable in the root directory

run cmds:
meson setup build

meson compiler -C build

./build/countUD [files]

