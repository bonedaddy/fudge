#include <abi.h>
#include <fudge.h>
#include "keycode.h"

static struct keyset map[256] = {
    {{{0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x31, 0x00, 0x00, 0x00}}, {1, {0x21, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x32, 0x00, 0x00, 0x00}}, {1, {0x40, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x33, 0x00, 0x00, 0x00}}, {1, {0x23, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x34, 0x00, 0x00, 0x00}}, {1, {0x24, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x35, 0x00, 0x00, 0x00}}, {1, {0x25, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x36, 0x00, 0x00, 0x00}}, {1, {0x5E, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x37, 0x00, 0x00, 0x00}}, {1, {0x26, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x38, 0x00, 0x00, 0x00}}, {1, {0x2A, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x39, 0x00, 0x00, 0x00}}, {1, {0x28, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x30, 0x00, 0x00, 0x00}}, {1, {0x29, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x2D, 0x00, 0x00, 0x00}}, {1, {0x5F, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x2B, 0x00, 0x00, 0x00}}, {1, {0x3D, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x08, 0x00, 0x00, 0x00}}, {1, {0x08, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x09, 0x00, 0x00, 0x00}}, {1, {0x09, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x71, 0x00, 0x00, 0x00}}, {1, {0x51, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x77, 0x00, 0x00, 0x00}}, {1, {0x57, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x65, 0x00, 0x00, 0x00}}, {1, {0x45, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x72, 0x00, 0x00, 0x00}}, {1, {0x52, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x74, 0x00, 0x00, 0x00}}, {1, {0x54, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x79, 0x00, 0x00, 0x00}}, {1, {0x59, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x75, 0x00, 0x00, 0x00}}, {1, {0x55, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x69, 0x00, 0x00, 0x00}}, {1, {0x49, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x6F, 0x00, 0x00, 0x00}}, {1, {0x4F, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x70, 0x00, 0x00, 0x00}}, {1, {0x50, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x5B, 0x00, 0x00, 0x00}}, {1, {0x7B, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x5D, 0x00, 0x00, 0x00}}, {1, {0x7D, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x0A, 0x00, 0x00, 0x00}}, {1, {0x0A, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x61, 0x00, 0x00, 0x00}}, {1, {0x41, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x73, 0x00, 0x00, 0x00}}, {1, {0x53, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x64, 0x00, 0x00, 0x00}}, {1, {0x44, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x66, 0x00, 0x00, 0x00}}, {1, {0x46, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x67, 0x00, 0x00, 0x00}}, {1, {0x47, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x68, 0x00, 0x00, 0x00}}, {1, {0x48, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x6A, 0x00, 0x00, 0x00}}, {1, {0x4A, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x6B, 0x00, 0x00, 0x00}}, {1, {0x4B, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x6C, 0x00, 0x00, 0x00}}, {1, {0x4C, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x3B, 0x00, 0x00, 0x00}}, {1, {0x3A, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x27, 0x00, 0x00, 0x00}}, {1, {0x22, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x60, 0x00, 0x00, 0x00}}, {1, {0x7E, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x5C, 0x00, 0x00, 0x00}}, {1, {0x7C, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x7A, 0x00, 0x00, 0x00}}, {1, {0x5A, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x78, 0x00, 0x00, 0x00}}, {1, {0x58, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x63, 0x00, 0x00, 0x00}}, {1, {0x43, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x76, 0x00, 0x00, 0x00}}, {1, {0x56, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x62, 0x00, 0x00, 0x00}}, {1, {0x42, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x6E, 0x00, 0x00, 0x00}}, {1, {0x4E, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x6D, 0x00, 0x00, 0x00}}, {1, {0x4D, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x2C, 0x00, 0x00, 0x00}}, {1, {0x3C, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x2E, 0x00, 0x00, 0x00}}, {1, {0x3E, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x2F, 0x00, 0x00, 0x00}}, {1, {0x3F, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
    {{{1, {0x20, 0x00, 0x00, 0x00}}, {1, {0x20, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}, {0, {0x00, 0x00, 0x00, 0x00}}}},
};

static unsigned int escaped;
static unsigned int modifier;

static struct keycode *scancode2keycode(unsigned char scancode)
{

    switch (scancode)
    {

    case 0xE0:
        escaped = 1;

        return 0;

    case 0x2A:
    case 0x36:
        modifier |= KEYMOD_SHIFT;

        return 0;

    case 0x38:
        modifier |= KEYMOD_ALT;

        return 0;

    case 0xAA:
    case 0xB6:
        modifier &= ~KEYMOD_SHIFT;

        return 0;

    case 0xB8:
        modifier &= ~KEYMOD_ALT;

        return 0;

    }

    if (scancode & 0x80)
        return 0;

    return &map[scancode].keycode[modifier];

}

void main(void)
{

    unsigned char buffer[FUDGE_BSIZE];
    unsigned int count;

    escaped = 0;
    modifier = 0;

    call_open(CALL_PO);
    call_open(CALL_PI);

    while ((count = call_read(CALL_PI, FUDGE_BSIZE, buffer)))
    {

        unsigned int i;

        for (i = 0; i < count; i++)
        {

            struct keycode *keycode = scancode2keycode(buffer[i]);

            if (!keycode)
                continue;

            if (keycode->length)
                call_write(CALL_PO, keycode->length, keycode->value);

        }

    }

    call_close(CALL_PI);
    call_close(CALL_PO);

}

