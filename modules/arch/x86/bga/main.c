#include <lib/memory.h>
#include <kernel/modules.h>
#include <modules/bga/bga.h>
#include <modules/nodefs/nodefs.h>

static struct bga_driver driver;
static struct nodefs_node xres;
static struct nodefs_node yres;
static struct nodefs_node bpp;
static struct nodefs_node enable;
static struct nodefs_node lfb;

static unsigned int xres_read(struct nodefs_node *self, unsigned int offset, unsigned int count, void *buffer)
{

    memory_copy(buffer, &driver.xres, 4);

    return 4;

}

static unsigned int xres_write(struct nodefs_node *self, unsigned int offset, unsigned int count, void *buffer)
{

    memory_copy(&driver.xres, buffer, 4);

    return 4;

}

static unsigned int yres_read(struct nodefs_node *self, unsigned int offset,  unsigned int count, void *buffer)
{

    memory_copy(buffer, &driver.yres, 4);

    return 4;

}

static unsigned int yres_write(struct nodefs_node *self, unsigned int offset, unsigned int count, void *buffer)
{

    memory_copy(&driver.yres, buffer, 4);

    return 4;

}

static unsigned int bpp_read(struct nodefs_node *self, unsigned int offset, unsigned int count, void *buffer)
{

    memory_copy(buffer, &driver.bpp, 4);

    return 4;

}

static unsigned int bpp_write(struct nodefs_node *self, unsigned int offset, unsigned int count, void *buffer)
{

    memory_copy(&driver.bpp, buffer, 4);

    return 4;

}

static unsigned int enable_write(struct nodefs_node *self, unsigned int offset, unsigned int count, void *buffer)
{

    driver.set_mode(&driver);

    return 0;

}

static unsigned int lfb_write(struct nodefs_node *self, unsigned int offset, unsigned int count, void *buffer)
{

    memory_copy((char *)driver.lfb + offset, buffer, count);

    return count;

}

void init()
{

    struct nodefs_driver *nodefsDriver = (struct nodefs_driver *)modules_get_driver(NODEFS_DRIVER_TYPE);

    bga_driver_init(&driver);
    modules_register_driver(&driver.base);

    if (!nodefsDriver)
        return;

    nodefsDriver->register_node(nodefsDriver, &xres, "bga/xres", &driver.base.base, xres_read, xres_write);
    nodefsDriver->register_node(nodefsDriver, &yres, "bga/yres", &driver.base.base, yres_read, yres_write);
    nodefsDriver->register_node(nodefsDriver, &bpp, "bga/bpp", &driver.base.base, bpp_read, bpp_write);
    nodefsDriver->register_node(nodefsDriver, &enable, "bga/enable", &driver.base.base, 0, enable_write);
    nodefsDriver->register_node(nodefsDriver, &lfb, "bga/lfb", &driver.base.base, 0, lfb_write);

}

void destroy()
{

    modules_unregister_driver(&driver.base);

}

