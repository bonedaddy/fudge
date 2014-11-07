struct base_block_interface
{

    struct base_interface base;
    unsigned int (*rdata)(unsigned int offset, unsigned int count, void *buffer);
    unsigned int (*wdata)(unsigned int offset, unsigned int count, void *buffer);

};

struct base_block_interfacenode
{

    struct system_node base;
    struct system_node data;
    struct base_block_interface *interface;

};

void base_block_registerinterface(struct base_block_interface *interface);
void base_block_registerinterfacenode(struct base_block_interfacenode *node);
void base_block_unregisterinterface(struct base_block_interface *interface);
void base_block_unregisterinterfacenode(struct base_block_interfacenode *node);
void base_block_initinterface(struct base_block_interface *interface, struct base_bus *bus, unsigned int id, unsigned int (*rdata)(unsigned int offset, unsigned int count, void *buffer), unsigned int (*wdata)(unsigned int offset, unsigned int count, void *buffer));
void base_block_initinterfacenode(struct base_block_interfacenode *node, struct base_block_interface *interface);
void base_block_setup();
