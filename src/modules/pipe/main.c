#include <fudge.h>
#include <kernel.h>
#include <modules/system/system.h>
#include "pipe.h"

static struct system_node root;
static struct system_node clone;

static void wakeup(struct list *list)
{

    struct list_item *current;

    for (current = list->head; current; current = current->next)
    {

        struct task *task = current->data;

        task_setstatus(task, TASK_STATUS_ACTIVE);
        list_remove(list, &task->blockitem);

    }

}

static unsigned int read(struct buffer *b, struct list *selfread, struct list *targetwrite, unsigned int ref, unsigned int count, void *buffer)
{

    struct task *task = task_findactive();

    count = buffer_rcfifo(b, count, buffer);

    if (!count && ref)
    {

        list_add(selfread, &task->blockitem);
        task_setstatus(task, TASK_STATUS_BLOCKED);

    }

    wakeup(targetwrite);

    return count;

}

static unsigned int write(struct buffer *b, struct list *selfwrite, struct list *targetread, unsigned int count, void *buffer)
{

    struct task *task = task_findactive();

    count = buffer_wcfifo(b, count, buffer);

    if (!count)
    {

        list_add(selfwrite, &task->blockitem);
        task_setstatus(task, TASK_STATUS_BLOCKED);

    }

    wakeup(targetread);

    return count;

}

static unsigned int end0_open(struct system_node *self)
{

    struct pipe *pipe = (struct pipe *)self->parent;

    pipe->end0.ref++;

    return (unsigned int)self;

}

static unsigned int end0_close(struct system_node *self)
{

    struct pipe *pipe = (struct pipe *)self->parent;

    pipe->end0.ref--;

    return (unsigned int)self;

}

static unsigned int end0_read(struct system_node *self, unsigned int offset, unsigned int count, void *buffer)
{

    struct pipe *pipe = (struct pipe *)self->parent;

    return read(&pipe->end0.buffer, &pipe->end0.readlist, &pipe->end1.writelist, pipe->end1.ref, count, buffer);

}

static unsigned int end0_write(struct system_node *self, unsigned int offset, unsigned int count, void *buffer)
{

    struct pipe *pipe = (struct pipe *)self->parent;

    return write(&pipe->end1.buffer, &pipe->end0.writelist, &pipe->end1.readlist, count, buffer);

}

static unsigned int end1_open(struct system_node *self)
{

    struct pipe *pipe = (struct pipe *)self->parent;

    pipe->end1.ref++;

    return (unsigned int)self;

}

static unsigned int end1_close(struct system_node *self)
{

    struct pipe *pipe = (struct pipe *)self->parent;

    pipe->end1.ref--;

    return (unsigned int)self;

}

static unsigned int end1_read(struct system_node *self, unsigned int offset, unsigned int count, void *buffer)
{

    struct pipe *pipe = (struct pipe *)self->parent;

    return read(&pipe->end1.buffer, &pipe->end1.readlist, &pipe->end0.writelist, pipe->end0.ref, count, buffer);

}

static unsigned int end1_write(struct system_node *self, unsigned int offset, unsigned int count, void *buffer)
{

    struct pipe *pipe = (struct pipe *)self->parent;

    return write(&pipe->end0.buffer, &pipe->end1.writelist, &pipe->end0.readlist, count, buffer);

}

static unsigned int clone_child(struct system_node *self, unsigned int count, char *path)
{

    struct list_item *current;

    for (current = root.children.head; current; current = current->next)
    {

        struct system_node *node = current->data;
        struct pipe *pipe = current->data;

        if (node == self)
            continue;

        if (pipe->end0.ref || pipe->end1.ref)
            continue;

        return node->child(node, count, path);

    }

    return 0;

}

void pipe_init(struct pipe *pipe)
{

    buffer_init(&pipe->end0.buffer, 4096, pipe->end0.data);
    buffer_init(&pipe->end1.buffer, 4096, pipe->end1.data);
    system_initnode(&pipe->end0.node, SYSTEM_NODETYPE_NORMAL, "0");
    system_initnode(&pipe->end1.node, SYSTEM_NODETYPE_NORMAL, "1");

    pipe->end0.node.open = end0_open;
    pipe->end0.node.close = end0_close;
    pipe->end0.node.read = end0_read;
    pipe->end0.node.write = end0_write;
    pipe->end1.node.open = end1_open;
    pipe->end1.node.close = end1_close;
    pipe->end1.node.read = end1_read;
    pipe->end1.node.write = end1_write;

    system_initnode(&pipe->root, SYSTEM_NODETYPE_GROUP | SYSTEM_NODETYPE_MULTI, "pipe");
    system_addchild(&pipe->root, &pipe->end0.node);
    system_addchild(&pipe->root, &pipe->end1.node);

    pipe->end0.ref = 0;
    pipe->end1.ref = 0;

}

void pipe_register(struct pipe *pipe)
{

    system_addchild(&root, &pipe->root);

}

void pipe_unregister(struct pipe *pipe)
{

    system_removechild(&root, &pipe->root);

}

void module_init(void)
{

    system_initnode(&root, SYSTEM_NODETYPE_GROUP, "pipe");
    system_initnode(&clone, SYSTEM_NODETYPE_GROUP, "clone");

    clone.child = clone_child;

    system_addchild(&root, &clone);

}

void module_register(void)
{

    system_registernode(&root);

}

void module_unregister(void)
{

    system_unregisternode(&root);

}

