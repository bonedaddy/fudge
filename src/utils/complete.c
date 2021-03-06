#include <fudge.h>
#include <abi.h>

static void complete(struct channel *channel, unsigned int source, unsigned int descriptor, void *name, unsigned int length)
{

    struct record record;

    file_open(descriptor);

    while (file_readall(descriptor, &record, sizeof (struct record)))
    {

        if (record.length >= length && memory_match(record.name, name, length))
        {

            channel_request(channel, EVENT_DATA);
            channel_append(channel, record.length, record.name);
            channel_appendstring(channel, "\n");
            channel_place(channel, source);

        }

        if (!file_step(descriptor))
            break;

    }

    file_close(descriptor);

}

static void onmain(struct channel *channel, unsigned int source, void *mdata, unsigned int msize)
{

    channel_close(channel);

}

static void ondata(struct channel *channel, unsigned int source, void *mdata, unsigned int msize)
{

    complete(channel, source, FILE_PW, mdata, msize);

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
    channel_setsignal(&channel, EVENT_DATA, ondata);
    channel_setsignal(&channel, EVENT_REDIRECT, onredirect);
    channel_listen(&channel);

}

