#include <fudge.h>
#include <abi.h>

static void ondone(struct channel *channel, unsigned int source, void *mdata, unsigned int msize)
{

    channel_close(channel);

}

static void ondata(struct channel *channel, unsigned int source, void *mdata, unsigned int msize)
{

    channel_request(channel, EVENT_DATA);
    channel_append(channel, msize, mdata);
    channel_place(channel, source);

}

static void onfile(struct channel *channel, unsigned int source, void *mdata, unsigned int msize)
{

    char buffer[FUDGE_BSIZE];
    unsigned int count;

    if (!file_walk2(FILE_L0, mdata))
        return;

    file_open(FILE_L0);

    while ((count = file_read(FILE_L0, buffer, FUDGE_BSIZE)))
    {

        channel_request(channel, EVENT_DATA);
        channel_append(channel, count, buffer);
        channel_place(channel, source);

    }

    file_close(FILE_L0);

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
    channel_setsignal(&channel, EVENT_DONE, ondone);
    channel_setsignal(&channel, EVENT_DATA, ondata);
    channel_setsignal(&channel, EVENT_FILE, onfile);
    channel_setsignal(&channel, EVENT_REDIRECT, onredirect);
    channel_listen(&channel);

}

