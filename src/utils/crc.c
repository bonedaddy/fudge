#include <fudge.h>
#include <abi.h>

static struct crc s;

static unsigned int ondata(union event_message *imessage, union event_message *omessage)
{

    struct event_data *data = event_getdata(imessage);

    crc_read(&s, data + 1, data->count);

    return 0;

}

static unsigned int onfile(union event_message *imessage, union event_message *omessage)
{

    struct event_file *file = event_getdata(imessage);
    unsigned char buffer[FUDGE_BSIZE];
    unsigned int count;

    if (!file->descriptor)
        return 0;

    file_open(file->descriptor);

    while ((count = file_read(file->descriptor, buffer, FUDGE_BSIZE)))
        crc_read(&s, buffer, count);

    file_close(file->descriptor);

    return 0;

}

static unsigned int onstop(union event_message *imessage, union event_message *omessage)
{

    unsigned char buffer[FUDGE_BSIZE];
    unsigned int result = crc_finalize(&s);

    event_reply(omessage, imessage, EVENT_DATA);
    event_adddata(omessage);
    event_appenddata(omessage, ascii_wvalue(buffer, 32, result, 10), buffer);
    event_appenddata(omessage, 1, "\n");
    event_send(omessage);

    return 1;

}

static unsigned int onkill(union event_message *imessage, union event_message *omessage)
{

    return 1;

}

void main(void)
{

    unsigned int status = 0;
    union event_message imessage;
    union event_message omessage;

    event_open();

    while (!status)
    {

        event_read(&imessage);

        switch (imessage.header.type)
        {

        case EVENT_DATA:
            status = ondata(&imessage, &omessage);

            break;

        case EVENT_FILE:
            status = onfile(&imessage, &omessage);

            break;

        case EVENT_STOP:
            status = onstop(&imessage, &omessage);

            break;

        case EVENT_KILL:
            status = onkill(&imessage, &omessage);

            break;

        }

    }

    event_close();

}

