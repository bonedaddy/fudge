#include <fudge.h>
#include <abi.h>

static unsigned int bytes;
static unsigned int words;
static unsigned int lines;
static unsigned int whitespace = 1;

static void sum(unsigned int count, void *buffer)
{

    char *data = buffer;
    unsigned int i;

    for (i = 0; i < count; i++)
    {

        switch (data[i])
        {

        case '\n':
            whitespace = 1;
            lines++;

            break;

        case ' ':
            whitespace = 1;

            break;

        default:
            if (whitespace)
                words++;

            whitespace = 0;

            break;

        }

        bytes++;

    }

}

static unsigned int ondata(struct event_channel *channel)
{

    sum(event_getdatasize(channel), event_getdata(channel));

    return 0;

}

static unsigned int onfile(struct event_channel *channel)
{

    struct event_file *file = event_getdata(channel);
    char buffer[FUDGE_BSIZE];
    unsigned int count;

    file_open(file->descriptor);

    while ((count = file_read(file->descriptor, buffer, FUDGE_BSIZE)))
        sum(count, buffer);

    file_close(file->descriptor);

    return 0;

}

static unsigned int onstop(struct event_channel *channel)
{

    char num[FUDGE_NSIZE];

    event_reply(channel, EVENT_DATA);
    event_append(&channel->o, ascii_wvalue(num, FUDGE_BSIZE, lines, 10), num);
    event_append(&channel->o, 1, "\n");
    event_append(&channel->o, ascii_wvalue(num, FUDGE_BSIZE, words, 10), num);
    event_append(&channel->o, 1, "\n");
    event_append(&channel->o, ascii_wvalue(num, FUDGE_BSIZE, bytes, 10), num);
    event_append(&channel->o, 1, "\n");
    event_place(channel->o.header.target, &channel->o);

    return 1;

}

static unsigned int onkill(struct event_channel *channel)
{

    return 1;

}

void main(void)
{

    unsigned int status = 0;
    struct event_channel channel;

    while (!status)
    {

        switch (event_pick(&channel))
        {

        case EVENT_DATA:
            status = ondata(&channel);

            break;

        case EVENT_FILE:
            status = onfile(&channel);

            break;

        case EVENT_STOP:
            status = onstop(&channel);

            break;

        case EVENT_KILL:
            status = onkill(&channel);

            break;

        }

    }

}

