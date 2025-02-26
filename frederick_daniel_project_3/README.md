# How to compile for Linux

- I used Meson as the build system. To run you can either install Meson or use gcc.
- I provided executables for the assignment in case you just want to run that.

How I compiled my program:

I installed meson and ninja:
sudo apt install meson ninja-build

1. meson setup build

2. meson compile -C build

3. ./build/mypipe [files]

In xv6, I modified the Makefile in the user directory by adding my file xv6mypipe.c which I also added in the user directory.
