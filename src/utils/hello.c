#include <abi.h>
#include <fudge.h>

static unsigned int quit;

static void onkill(struct event_header *header)
{

    char message[FUDGE_BSIZE];

    event_addresponse(message, header, EVENT_CHILD);
    event_send(message);

    quit = 1;

}

static void onfile(struct event_header *header)
{

    struct event_file *file = event_getdata(header);
    char message[FUDGE_BSIZE];

    if (file->descriptor)
        return;

    event_addresponse(message, header, EVENT_DATA);
    event_adddata(message, file->session, 13, "Hello world!\n");
    event_send(message);
    event_addresponse(message, header, EVENT_DATA);
    event_adddata(message, file->session, 0, 0);
    event_send(message);

}

void main(void)
{

    event_open();

    while (!quit)
    {

        char data[FUDGE_BSIZE];
        struct event_header *header = event_read(data);

        switch (header->type)
        {

        case EVENT_EXIT:
        case EVENT_KILL:
            onkill(header);

            break;

        case EVENT_FILE:
            onfile(header);

            break;

        }

    }

    event_close();

}

