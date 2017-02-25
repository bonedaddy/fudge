#include <fudge.h>
#include <kernel.h>
#include <modules/system/system.h>
#include <modules/event/event.h>

static struct system_node root;
static struct system_node data;
static struct system_node event;
static struct list eventlinks;
static char databuffer[FUDGE_BSIZE];
static struct ring dataring;

static unsigned int data_read(struct system_node *self, struct service_descriptor *descriptor, void *buffer, unsigned int count)
{

    return ring_read(&dataring, buffer, count);

}

static unsigned int data_write(struct system_node *self, struct service_descriptor *descriptor, void *buffer, unsigned int count)
{

    return ring_write(&dataring, buffer, count);

}

static unsigned int event_open(struct system_node *self, struct service_descriptor *descriptor)
{

    list_add(&eventlinks, &descriptor->link);

    return descriptor->id;

}

static unsigned int event_close(struct system_node *self, struct service_descriptor *descriptor)
{

    list_remove(&eventlinks, &descriptor->link);

    return descriptor->id;

}

static unsigned int event_write(struct system_node *self, struct service_descriptor *descriptor, void *buffer, unsigned int count)
{

    return event_send(&eventlinks, descriptor, buffer, count);

}

void module_init(void)
{

    ring_init(&dataring, FUDGE_BSIZE, databuffer);
    system_initnode(&root, SYSTEM_NODETYPE_GROUP, "wm");
    system_initnode(&data, SYSTEM_NODETYPE_NORMAL, "data");
    system_initnode(&event, SYSTEM_NODETYPE_NORMAL, "event");

    data.read = data_read;
    data.write = data_write;
    event.open = event_open;
    event.close = event_close;
    event.read = system_readtask;
    event.write = event_write;

    system_addchild(&root, &data);
    system_addchild(&root, &event);

}

void module_register(void)
{

    system_registernode(&root);

}

void module_unregister(void)
{

    system_unregisternode(&root);

}

