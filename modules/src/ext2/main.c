#include <vfs.h>
#include <block/block.h>
#include <ext2/ext2.h>

static struct ext2_protocol protocol;

void init()
{

    ext2_protocol_init(&protocol);
    block_register_protocol(&protocol.base);

}

void destroy()
{

    block_unregister_protocol(&protocol.base);

}

