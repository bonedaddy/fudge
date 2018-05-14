#include "ctrl.h"

void ctrl_setconsettings(struct ctrl_consettings *settings, unsigned int interface, unsigned int protocol, unsigned int port)
{

    settings->interface = interface;
    settings->port = port;

}

void ctrl_setpartsettings(struct ctrl_partsettings *settings, unsigned int interface, unsigned int start, unsigned int end)
{

    settings->interface = interface;
    settings->start = start;
    settings->end = end;

}

void ctrl_setvideosettings(struct ctrl_videosettings *settings, unsigned int w, unsigned int h, unsigned int bpp)
{

    settings->w = w;
    settings->h = h;
    settings->bpp = bpp;

}

