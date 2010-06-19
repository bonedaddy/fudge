ASM=nasm
ASMFLAGS=-f elf
GCC=gcc
GCCFLAGS=-c -O2 -I./include -Wall -Wextra -ffreestanding -nostdlib -nostartfiles -nodefaultlibs
LD=ld
LDFLAGS=-T linker.ld
ISO=genisoimage
ISOFLAGS=-R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table 
MKINITRD=./tools/mkinitrd

.PHONY: all cd clean i386 initrd kernel library

all: kernel initrd cd

cd:
	@echo "Creating iso..."
	@$(ISO) $(ISOFLAGS) -o fudge.iso root

clean:
	@echo "Cleaning..."
	@rm -f fudge.iso
	@rm -f kernel/*.o
	@rm -f kernel/arch/i386/*.o
	@rm -f lib/*.o
	@rm -f root/boot/kernel
	@rm -f root/boot/initrd
	@rm -f tools/mkinitrd

i386:
	@echo "Building i386..."
	@$(ASM) $(ASMFLAGS) kernel/arch/i386/cpu.s -o kernel/arch/i386/cpu.o
	@$(ASM) $(ASMFLAGS) kernel/arch/i386/gdt.s -o kernel/arch/i386/gdt.o
	@$(ASM) $(ASMFLAGS) kernel/arch/i386/idt.s -o kernel/arch/i386/idt.o
	@$(ASM) $(ASMFLAGS) kernel/arch/i386/irq.s -o kernel/arch/i386/irq.o
	@$(ASM) $(ASMFLAGS) kernel/arch/i386/isr.s -o kernel/arch/i386/isr.o
	@$(ASM) $(ASMFLAGS) kernel/arch/i386/loader.s -o kernel/arch/i386/loader.o
	@$(ASM) $(ASMFLAGS) kernel/arch/i386/mmu.s -o kernel/arch/i386/mmu.o
	@$(ASM) $(ASMFLAGS) kernel/arch/i386/vbe.s -o kernel/arch/i386/vbe.o

initrd: library
	@echo "Creating ramdisk..."
	@$(GCC) -O2 tools/mkinitrd.c -o tools/mkinitrd
	$(MKINITRD) ramdisk/about.txt about.txt ramdisk/help.txt help.txt ramdisk/hello.s hello.s ramdisk/hello hello ramdisk/hello2.c hello2.c ramdisk/hello2 hello2 ramdisk/cat cat

kernel: library i386
	@echo "Building kernel..."
	@$(GCC) $(GCCFLAGS) kernel/assert.c -o kernel/assert.o
	@$(GCC) $(GCCFLAGS) kernel/cpu.c -o kernel/cpu.o
	@$(GCC) $(GCCFLAGS) kernel/gdt.c -o kernel/gdt.o
	@$(GCC) $(GCCFLAGS) kernel/heap.c -o kernel/heap.o
	@$(GCC) $(GCCFLAGS) kernel/idt.c -o kernel/idt.o
	@$(GCC) $(GCCFLAGS) kernel/initrd.c -o kernel/initrd.o
	@$(GCC) $(GCCFLAGS) kernel/irq.c -o kernel/irq.o
	@$(GCC) $(GCCFLAGS) kernel/isr.c -o kernel/isr.o
	@$(GCC) $(GCCFLAGS) kernel/kbd.c -o kernel/kbd.o
	@$(GCC) $(GCCFLAGS) kernel/kernel.c -o kernel/kernel.o
	@$(GCC) $(GCCFLAGS) kernel/mmu.c -o kernel/mmu.o
	@$(GCC) $(GCCFLAGS) kernel/pit.c -o kernel/pit.o
	@$(GCC) $(GCCFLAGS) kernel/screen.c -o kernel/screen.o
	@$(GCC) $(GCCFLAGS) kernel/shell.c -o kernel/shell.o
	@$(GCC) $(GCCFLAGS) kernel/syscall.c -o kernel/syscall.o
	@$(GCC) $(GCCFLAGS) kernel/task.c -o kernel/task.o
	@$(GCC) $(GCCFLAGS) kernel/vbe.c -o kernel/vbe.o
	@$(GCC) $(GCCFLAGS) kernel/vfs.c -o kernel/vfs.o
	@$(LD) $(LDFLAGS) \
    lib/cbuffer.o \
    lib/io.o \
    lib/memory.o \
    lib/stack.o \
    lib/string.o \
    kernel/arch/i386/cpu.o \
    kernel/arch/i386/gdt.o \
    kernel/arch/i386/idt.o \
    kernel/arch/i386/irq.o \
    kernel/arch/i386/isr.o \
    kernel/arch/i386/loader.o \
    kernel/arch/i386/mmu.o \
    kernel/arch/i386/vbe.o \
    kernel/assert.o \
    kernel/cpu.o \
    kernel/gdt.o \
    kernel/heap.o \
    kernel/idt.o \
    kernel/initrd.o \
    kernel/irq.o \
    kernel/isr.o \
    kernel/kbd.o \
    kernel/kernel.o \
    kernel/mmu.o \
    kernel/pit.o \
    kernel/screen.o \
    kernel/shell.o \
    kernel/syscall.o \
    kernel/task.o \
    kernel/vbe.o \
    kernel/vfs.o \
    -o root/boot/kernel

library:
	@echo "Building library..."
	@$(GCC) $(GCCFLAGS) lib/stack.c -o lib/stack.o
	@$(GCC) $(GCCFLAGS) lib/cbuffer.c -o lib/cbuffer.o
	@$(GCC) $(GCCFLAGS) lib/string.c -o lib/string.o
	@$(GCC) $(GCCFLAGS) lib/memory.c -o lib/memory.o
	@$(GCC) $(GCCFLAGS) lib/io.c -o lib/io.o

