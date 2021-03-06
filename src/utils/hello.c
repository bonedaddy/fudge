#include <fudge.h>
#include <abi.h>

static void onmain(struct channel *channel, unsigned int source, void *mdata, unsigned int msize)
{

    channel_request(channel, EVENT_DATA);
    channel_appendstring(channel, "Hello world!\n");
    channel_place(channel, source);
    channel_close(channel);

}

static void onredirect(struct channel *channel, unsigned int source, void *mdata, unsigned int msize)
{

    struct event_redirect *redirect = mdata;

    channel_setredirect(channel, redirect->type, redirect->mode, redirect->id, source);

}

void main(void)
{

    struct channel channel;

    channel_init(&channel);
    channel_setsignal(&channel, EVENT_MAIN, onmain);
    channel_setsignal(&channel, EVENT_REDIRECT, onredirect);
    channel_listen(&channel);

}

