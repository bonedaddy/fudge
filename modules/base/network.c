#include <fudge/module.h>
#include <system/system.h>
#include "base.h"
#include "network.h"

static struct system_group root;
static struct system_stream data;
static struct system_stream mac;

static unsigned int data_open(struct system_node *self)
{

    return (unsigned int)self;

}

static unsigned int data_close(struct system_node *self)
{

    return (unsigned int)self;

}

unsigned int data_read(struct system_node *self, unsigned int offset, unsigned int count, void *buffer)
{

    struct base_network *interface = (struct base_network *)self->parent;
    struct base_device *device = (struct base_device *)self->parent->parent;
 
    if (offset)
        return 0;

    return interface->receive(device, count, buffer);

}

unsigned int data_write(struct system_node *self, unsigned int offset, unsigned int count, void *buffer)
{

    struct base_network *interface = (struct base_network *)self->parent;
    struct base_device *device = (struct base_device *)self->parent->parent;
 
    if (offset)
        return 0;

    return interface->send(device, count, buffer);

}

static unsigned int mac_open(struct system_node *self)
{

    return (unsigned int)self;

}

static unsigned int mac_close(struct system_node *self)
{

    return (unsigned int)self;

}

unsigned int mac_read(struct system_node *self, unsigned int offset, unsigned int count, void *buffer)
{

    struct base_network *interface = (struct base_network *)self->parent;
    char *mac = "00:00:00:00:00:00";
    unsigned int i;

    for (i = 0; i < 6; i++)
        memory_write_paddednumber(mac, 17, interface->mac[i], 16, i * 3, 2);

    return memory_read(buffer, count, mac, 17, offset); 

}

unsigned int mac_write(struct system_node *self, unsigned int offset, unsigned int count, void *buffer)
{

    return 0;

}

void base_register_network(struct base_network *interface, struct base_device *device)
{

    system_group_add(&root, &device->module.base.node);
    system_group_add(&device->module.base, &interface->base.node);
    system_group_add(&interface->base, &data.node);
    system_group_add(&interface->base, &mac.node);

}

void base_init_network(struct base_network *interface, unsigned int (*receive)(struct base_device *device, unsigned int count, void *buffer), unsigned int (*send)(struct base_device *device, unsigned int count, void *buffer))
{

    memory_clear(interface, sizeof (struct base_network));
    system_init_group(&interface->base, "network");

    interface->receive = receive;
    interface->send = send;

}

void base_setup_network()
{

    system_init_group(&root, "network");
    system_init_stream(&data, "data", data_open, data_close, data_read, data_write);
    system_init_stream(&mac, "mac", mac_open, mac_close, mac_read, mac_write);
    system_register_node(&root.node);

}
