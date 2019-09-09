#include <fudge.h>
#include <kernel.h>
#include <kernel/x86/cpu.h>
#include <kernel/x86/gdt.h>
#include <kernel/x86/idt.h>
#include <kernel/x86/tss.h>
#include <kernel/x86/arch.h>
#include "mboot.h"

#define MBOOT_MAGIC                     0x2BADB002

static struct service_backend backend;
static unsigned int address;
static unsigned int limit;

static unsigned int read(struct service_state *state, void *buffer, unsigned int count, unsigned int offset)
{

    return memory_read(buffer, count, (void *)address, limit, offset);

}

static unsigned int write(struct service_state *state, void *buffer, unsigned int count, unsigned int offset)
{

    return memory_write((void *)address, limit, buffer, count, offset);

}

static unsigned int map(struct service_state *state, unsigned int offset, unsigned int count)
{

    return address + offset;

}

void mboot_setup(struct mboot_header *header, unsigned int magic)
{

    memory_copy((void *)0x5000, header, sizeof (struct mboot_header));

    if (header->flags & MBOOT_FLAG_LOADER)
    {

    }

    if (header->flags & MBOOT_FLAG_DEVICE)
    {

        unsigned int deviceNumber = header->device >> 24;

        switch (deviceNumber)
        {

        case 0xE0:
            break;

        case 0x00:
            break;

        case 0x80:
            break;

        default:
            break;

        }

    }

    if (header->flags & MBOOT_FLAG_CMDLINE)
    {

    }

    if (header->flags & MBOOT_FLAG_AOUT)
    {

    }

    if (header->flags & MBOOT_FLAG_ELF)
    {

    }

    if (header->flags & MBOOT_FLAG_MODULES)
    {

        struct mboot_module *modules = (struct mboot_module *)header->modules.address;

        address = modules[0].address;
        limit = modules[0].limit;

    }

    if (header->flags & MBOOT_FLAG_MEMORY)
    {

    }

    if (header->flags & MBOOT_FLAG_MMAP)
    {

    }

    if (header->flags & MBOOT_FLAG_VBE)
    {

        if (header->vbe.cinfo)
        {

        }

    }

    if (header->flags & MBOOT_FLAG_FRAMEBUFFER)
    {

    }

    service_initbackend(&backend, 1000, read, write, map);
    arch_setup(&backend);

}

