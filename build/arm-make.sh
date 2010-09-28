#!/bin/sh

DIR_KERNEL="../kernel"
DIR_ARCH_KERNEL="../arch/arm/kernel"
DIR_LIB="../lib"

ASM="arm-elf-as"
ASMFLAGS=
GCC="arm-elf-gcc"
GCCFLAGS="-c -I../include -Wall -Wextra -ffreestanding -nostdlib -nostartfiles -std=gnu99 -mhard-float"
LD="arm-elf-ld"
LDFLAGS="-T./linker-arm.ld"

$GCC $GCCFLAGS $DIR_KERNEL/assert.c -o $DIR_KERNEL/assert.o
$GCC $GCCFLAGS $DIR_KERNEL/kernel.c -o $DIR_KERNEL/kernel.o
$GCC $GCCFLAGS $DIR_ARCH_KERNEL/arch.c -o $DIR_ARCH_KERNEL/arch.o
$ASM $ASMFLAGS $DIR_ARCH_KERNEL/init.s -o $DIR_ARCH_KERNEL/init.o
$GCC $GCCFLAGS $DIR_LIB/string.c -o $DIR_LIB/string.o
$GCC $GCCFLAGS $DIR_LIB/vfs.c -o $DIR_LIB/vfs.o

$LD $LDFLAGS \
    $DIR_KERNEL/assert.o \
    $DIR_KERNEL/kernel.o \
    $DIR_ARCH_KERNEL/arch.o \
    $DIR_ARCH_KERNEL/init.o \
    $DIR_LIB/string.o \
    $DIR_LIB/vfs.o \
    -o root/boot/kernel

