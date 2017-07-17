#include <abi.h>
#include <fudge.h>
#include "event.h"

static void onkeypress(struct event_handlers *handlers, unsigned int descriptor, struct event_header *header)
{

    struct event_keypress keypress;

    file_readall(descriptor, &keypress, sizeof (struct event_keypress));

    if (handlers->keypress)
        handlers->keypress(header, &keypress);

}

static void onkeyrelease(struct event_handlers *handlers, unsigned int descriptor, struct event_header *header)
{

    struct event_keyrelease keyrelease;

    file_readall(descriptor, &keyrelease, sizeof (struct event_keyrelease));

    if (handlers->keyrelease)
        handlers->keyrelease(header, &keyrelease);

}

static void onmousemove(struct event_handlers *handlers, unsigned int descriptor, struct event_header *header)
{

    struct event_mousemove mousemove;

    file_readall(descriptor, &mousemove, sizeof (struct event_mousemove));

    if (handlers->mousemove)
        handlers->mousemove(header, &mousemove);

}

static void onmousepress(struct event_handlers *handlers, unsigned int descriptor, struct event_header *header)
{

    struct event_mousepress mousepress;

    file_readall(descriptor, &mousepress, sizeof (struct event_mousepress));

    if (handlers->mousepress)
        handlers->mousepress(header, &mousepress);

}

static void onmouserelease(struct event_handlers *handlers, unsigned int descriptor, struct event_header *header)
{

    struct event_mouserelease mouserelease;

    file_readall(descriptor, &mouserelease, sizeof (struct event_mouserelease));

    if (handlers->mouserelease)
        handlers->mouserelease(header, &mouserelease);

}

static void ontimertick(struct event_handlers *handlers, unsigned int descriptor, struct event_header *header)
{

    struct event_timertick timertick;

    file_readall(descriptor, &timertick, sizeof (struct event_timertick));

    if (handlers->timertick)
        handlers->timertick(header, &timertick);

}

static void onvideomode(struct event_handlers *handlers, unsigned int descriptor, struct event_header *header)
{

    struct event_videomode videomode;

    file_readall(descriptor, &videomode, sizeof (struct event_videomode));

    if (handlers->videomode)
        handlers->videomode(header, &videomode);

}

static void onwmmap(struct event_handlers *handlers, unsigned int descriptor, struct event_header *header)
{

    if (handlers->wmmap)
        handlers->wmmap(header);

}

static void onwminit(struct event_handlers *handlers, unsigned int descriptor, struct event_header *header)
{

    if (handlers->wminit)
        handlers->wminit(header);

}

static void onwmexit(struct event_handlers *handlers, unsigned int descriptor, struct event_header *header)
{

    if (handlers->wmexit)
        handlers->wmexit(header);

}

static void onwmresize(struct event_handlers *handlers, unsigned int descriptor, struct event_header *header)
{

    struct event_wmresize wmresize;

    file_readall(descriptor, &wmresize, sizeof (struct event_wmresize));

    if (handlers->wmresize)
        handlers->wmresize(header, &wmresize);

}

static void onwmshow(struct event_handlers *handlers, unsigned int descriptor, struct event_header *header)
{

    if (handlers->wmshow)
        handlers->wmshow(header);

}

static void onwmhide(struct event_handlers *handlers, unsigned int descriptor, struct event_header *header)
{

    if (handlers->wmhide)
        handlers->wmhide(header);

}

static void onwmflush(struct event_handlers *handlers, unsigned int descriptor, struct event_header *header)
{

    if (handlers->wmflush)
        handlers->wmflush(header);

}

static void (*funcs[EVENTS])(struct event_handlers *handlers, unsigned int descriptor, struct event_header *header) = {
    0,
    onkeypress,
    onkeyrelease,
    onmousemove,
    onmousepress,
    onmouserelease,
    ontimertick,
    onvideomode,
    onwmmap,
    onwminit,
    onwmexit,
    onwmresize,
    onwmshow,
    onwmhide,
    onwmflush
};

unsigned int event_read(struct event_handlers *handlers, unsigned int descriptor)
{

    struct event_header header;
    unsigned int count = file_readall(descriptor, &header, sizeof (struct event_header));

    if (!count)
        return 0;

    if (funcs[header.type])
        funcs[header.type](handlers, descriptor, &header);

    return count;

}

static void send(unsigned int descriptor, unsigned int destination, unsigned int type, unsigned int datacount, void *databuffer)
{

    unsigned char buffer[512];
    struct event_header header;

    header.destination = destination;
    header.type = type;

    memory_write(buffer, 512, &header, sizeof (struct event_header), 0);
    memory_write(buffer, 512, databuffer, datacount, sizeof (struct event_header));
    file_writeall(descriptor, buffer, sizeof (struct event_header) + datacount);

}

void event_sendkeypress(unsigned int descriptor, unsigned int destination, unsigned char scancode)
{

    struct event_keypress keypress;

    keypress.scancode = scancode;

    send(descriptor, destination, EVENT_KEYPRESS, sizeof (struct event_keypress), &keypress);

}

void event_sendkeyrelease(unsigned int descriptor, unsigned int destination, unsigned char scancode)
{

    struct event_keyrelease keyrelease;

    keyrelease.scancode = scancode;

    send(descriptor, destination, EVENT_KEYRELEASE, sizeof (struct event_keyrelease), &keyrelease);

}

void event_sendmousemove(unsigned int descriptor, unsigned int destination, char relx, char rely)
{

    struct event_mousemove mousemove;

    mousemove.relx = relx;
    mousemove.rely = rely;

    send(descriptor, destination, EVENT_MOUSEMOVE, sizeof (struct event_mousemove), &mousemove);

}

void event_sendmousepress(unsigned int descriptor, unsigned int destination, unsigned int button)
{

    struct event_mousepress mousepress;

    mousepress.button = button;

    send(descriptor, destination, EVENT_MOUSEPRESS, sizeof (struct event_mousepress), &mousepress);

}

void event_sendmouserelease(unsigned int descriptor, unsigned int destination, unsigned int button)
{

    struct event_mouserelease mouserelease;

    mouserelease.button = button;

    send(descriptor, destination, EVENT_MOUSERELEASE, sizeof (struct event_mouserelease), &mouserelease);

}

void event_sendwmmap(unsigned int descriptor, unsigned int destination)
{

    send(descriptor, destination, EVENT_WMMAP, 0, 0);

}

void event_sendwminit(unsigned int descriptor, unsigned int destination)
{

    send(descriptor, destination, EVENT_WMINIT, 0, 0);

}

void event_sendwmexit(unsigned int descriptor, unsigned int destination)
{

    send(descriptor, destination, EVENT_WMEXIT, 0, 0);

}

void event_sendwmresize(unsigned int descriptor, unsigned int destination, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int padding, unsigned int lineheight)
{

    struct event_wmresize wmresize;

    wmresize.x = x;
    wmresize.y = y;
    wmresize.w = w;
    wmresize.h = h;
    wmresize.padding = padding;
    wmresize.lineheight = lineheight;

    send(descriptor, destination, EVENT_WMRESIZE, sizeof (struct event_wmresize), &wmresize);

}

void event_sendwmshow(unsigned int descriptor, unsigned int destination)
{

    send(descriptor, destination, EVENT_WMSHOW, 0, 0);

}

void event_sendwmhide(unsigned int descriptor, unsigned int destination)
{

    send(descriptor, destination, EVENT_WMHIDE, 0, 0);

}

void event_sendwmflush(unsigned int descriptor, unsigned int destination)
{

    send(descriptor, destination, EVENT_WMFLUSH, 0, 0);

}
