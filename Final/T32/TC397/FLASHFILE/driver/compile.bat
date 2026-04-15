@echo off
set PATH=C:\sw\TOOLS\compilers\mingw64\bin;C:\sw\TOOLS\compilers\tricore-gcc-13.4-20250801-win32\bin;%PATH%

tricore-elf-gcc -O3 -c main.c -o main.o
tricore-elf-gcc -T link.ld -nostdlib -o nor_spicomdrv.elf main.o
py elf2bin.py
pause
