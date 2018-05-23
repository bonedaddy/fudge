#include <abi.h>
#include <fudge.h>
#include "base.h"

void event_read(unsigned int descriptor, struct event *event)
{

    file_readall(descriptor, &event->header, sizeof (struct event_header));
    file_readall(descriptor, event->data, event->header.length - sizeof (struct event_header));

}

void event_send(unsigned int descriptor, struct event *event, unsigned int destination, unsigned int type, unsigned int length)
{

    event->header.destination = destination;
    event->header.type = type;
    event->header.length = sizeof (struct event_header) + length;

    file_writeall(descriptor, event, event->header.length);

}

