#include <abi.h>
#include <fudge.h>
#include <event/base.h>
#include <event/device.h>

static unsigned int quit;
static unsigned char inputbuffer[FUDGE_BSIZE];
static struct ring input;

static unsigned int interpretbuiltin(unsigned int count, char *command)
{

    if (memory_match(command, "cd ", 3))
    {

        command[count - 1] = '\0';

        if (file_walk(FILE_L8, command + 3))
        {

            file_duplicate(FILE_PW, FILE_L8);
            file_duplicate(FILE_CW, FILE_L8);

        }

        return 1;

    }

    return 0;

}

static void interpretslang(unsigned int count, char *command)
{

    unsigned int id;

    if (!file_walk(FILE_CP, "/bin/slang2"))
        return;

    id = call_spawn();

    event_sendinit(FILE_L0, id);
    event_senddata(FILE_L0, id, count, command);
    event_sendexit(FILE_L0, id);

}

static void interpret(struct ring *ring)
{

    char buffer[FUDGE_BSIZE];
    unsigned int count = ring_read(ring, buffer, FUDGE_BSIZE);

    if (interpretbuiltin(count, buffer))
        return;

    interpretslang(count, buffer);

}

static void oninit(struct event_header *header, void *data)
{

    ring_init(&input, FUDGE_BSIZE, inputbuffer);
    file_writeall(FILE_PO, "$ ", 2);

}

static void onkill(struct event_header *header, void *data)
{

    quit = 1;

}

static void onconsoledata(struct event_header *header, void *data)
{

    struct event_consoledata *consoledata = data;

    switch (consoledata->data)
    {

    case '\0':
        break;

    case '\t':
        /* Call complete */

        break;

    case '\b':
    case 0x7F:
        if (!ring_skipreverse(&input, 1))
            break;

        file_writeall(FILE_PO, "\b \b", 3);

        break;

    case '\r':
        consoledata->data = '\n';

    case '\n':
        file_writeall(FILE_PO, &consoledata->data, 1);
        ring_write(&input, &consoledata->data, 1);
        interpret(&input);
        file_writeall(FILE_PO, "$ ", 2);

        break;

    default:
        ring_write(&input, &consoledata->data, 1);
        file_writeall(FILE_PO, &consoledata->data, 1);

        break;

    }

}

void main(void)
{

    if (!file_walk(FILE_L0, "/system/event"))
        return;

    if (!file_walk(FILE_L1, "/system/console/event"))
        return;

    file_open(FILE_L0);
    file_open(FILE_L1);

    while (!quit)
    {

        struct event event;

        event_read(FILE_L0, &event);

        switch (event.header.type)
        {

        case EVENT_INIT:
            oninit(&event.header, event.data);

            break;

        case EVENT_KILL:
            onkill(&event.header, event.data);

            break;

        case EVENT_CONSOLEDATA:
            onconsoledata(&event.header, event.data);

            break;

        }

    }

    file_close(FILE_L1);
    file_close(FILE_L0);

}

