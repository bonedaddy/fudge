.code32

.extern mboot_setup

.set INIT_MBOOT_HEADER_MAGIC,           0x1BADB002
.set INIT_MBOOT_HEADER_FLAGS,           0x00000007
.set INIT_MBOOT_CHECKSUM,               0x00000000 - (INIT_MBOOT_HEADER_MAGIC + INIT_MBOOT_HEADER_FLAGS)

.section .data

.int INIT_MBOOT_HEADER_MAGIC
.int INIT_MBOOT_HEADER_FLAGS
.int INIT_MBOOT_CHECKSUM
.int 0
.int 0
.int 0
.int 0
.int 0
.int 0
.int 1024
.int 768
.int 32

.section .text

.global mboot_init
mboot_init:
    pushl %eax
    pushl %ebx
    call mboot_setup
    hlt

