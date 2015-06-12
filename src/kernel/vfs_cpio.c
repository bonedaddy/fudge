#include <fudge.h>
#include <cpio/cpio.h>
#include "resource.h"
#include "vfs.h"

static struct vfs_protocol protocol;

static unsigned int decode(unsigned int id)
{

    return id - 4096;

}

static unsigned int encode(unsigned int address)
{

    return address + 4096;

}

static unsigned int getheader(struct vfs_backend *backend, struct cpio_header *header, unsigned int address)
{

    return backend->read(address, sizeof (struct cpio_header), header) == sizeof (struct cpio_header);

}

static unsigned int getname(struct vfs_backend *backend, struct cpio_header *header, unsigned int address, unsigned int count, void *buffer)
{

    if (count < header->namesize)
        return 0;

    return backend->read(cpio_namedata(header, address), header->namesize, buffer) == header->namesize;

}

static unsigned int protocol_match(struct vfs_backend *backend)
{

    struct cpio_header header;

    if (!getheader(backend, &header, 0))
        return 0;

    return cpio_validate(&header);

}

static unsigned int protocol_root(struct vfs_backend *backend)
{

    struct cpio_header header;
    unsigned int address = 0;
    unsigned int last = address;

    do
    {

        if (!getheader(backend, &header, address))
            break;

        if (!cpio_validate(&header))
            break;

        if ((header.mode & 0xF000) != 0x4000)
            continue;

        last = address;

    } while ((address = cpio_next(&header, address)));

    return encode(last);

}

static unsigned int protocol_parent(struct vfs_backend *backend, unsigned int id)
{

    struct cpio_header header;
    unsigned char name[1024];
    unsigned int address = decode(id);
    unsigned int length;

    if (!getheader(backend, &header, address))
        return 0;

    if (!getname(backend, &header, address, 1024, name))
        return 0;

    length = header.namesize - 1;

    while (--length && name[length] != '/');

    do
    {

        if (!getheader(backend, &header, address))
            break;

        if (!cpio_validate(&header))
            break;

        if ((header.mode & 0xF000) != 0x4000)
            continue;

        if (header.namesize == length + 1)
        {

            unsigned char pname[1024];

            if (!getname(backend, &header, address, 1024, pname))
                break;

            if (memory_match(name, pname, length))
                return encode(address);

        }

    } while ((address = cpio_next(&header, address)));

    return 0;

}

static unsigned int protocol_child(struct vfs_backend *backend, unsigned int id, unsigned int count, const char *path)
{

    struct cpio_header header;
    unsigned int address = decode(id);
    unsigned int length;

    if (!count)
        return id;

    if (!getheader(backend, &header, address))
        return 0;

    length = header.namesize;
    address = 0;

    if (path[count - 1] == '/')
        count--;

    do
    {

        unsigned int cid = encode(address);

        if (cid == id)
            break;

        if (!getheader(backend, &header, address))
            break;

        if (header.namesize - length == count + 1)
        {

            unsigned char cname[1024];

            if (!getname(backend, &header, address, 1024, cname))
                break;

            if (memory_match(cname + length, path, count))
                return cid;

        }

    } while ((address = cpio_next(&header, address)));

    return 0;

}

static unsigned int protocol_create(struct vfs_backend *backend, unsigned int id, unsigned int count, const char *name)
{

    return 0;

}

static unsigned int protocol_destroy(struct vfs_backend *backend, unsigned int id, unsigned int count, const char *name)
{

    return 0;

}

static unsigned int protocol_open(struct vfs_backend *backend, unsigned int id)
{

    return id;

}

static unsigned int protocol_close(struct vfs_backend *backend, unsigned int id)
{

    return id;

}

static unsigned int protocol_read(struct vfs_backend *backend, unsigned int id, unsigned int offset, unsigned int size, unsigned int count, void *buffer)
{

    struct cpio_header header;
    unsigned int address = decode(id);

    if (!getheader(backend, &header, address))
        return 0;

    if ((header.mode & 0xF000) == 0x8000)
    {

        unsigned int s = cpio_filesize(&header);
        unsigned int d = cpio_filedata(&header, address);

        if (offset >= s)
            return 0;

        count = size * count;
        s -= offset;

        return backend->read(d + offset, (count > s) ? s : count, buffer);

    }

    if ((header.mode & 0xF000) == 0x4000)
    {

        struct record *records = buffer;
        unsigned int length = header.namesize;
        unsigned int i = 0;

        if (offset > 0)
            return 0;

        records[i].size = 0;
        records[i].length = memory_read(records[i].name, RECORD_NAMESIZE, "../", 3, 1, 0);

        i++;

        address = 0;

        do
        {

            unsigned int cid = encode(address);

            if (cid == id)
                break;

            if (!getheader(backend, &header, address))
                break;

            if (protocol_parent(backend, cid) == id)
            {

                unsigned char name[1024];

                if (!getname(backend, &header, address, 1024, name))
                    break;

                records[i].size = cpio_filesize(&header);
                records[i].length = memory_read(records[i].name, RECORD_NAMESIZE, name, header.namesize, 1, length);

                if ((header.mode & 0xF000) == 0x4000)
                    records[i].length += memory_write(records[i].name, RECORD_NAMESIZE, "/", 1, 1, records[i].length);

                i++;

            }

        } while ((address = cpio_next(&header, address)));

        return i;

    }

    return 0;

}

static unsigned int protocol_write(struct vfs_backend *backend, unsigned int id, unsigned int offset, unsigned int size, unsigned int count, void *buffer)
{

    struct cpio_header header;
    unsigned int address = decode(id);

    if (!getheader(backend, &header, address))
        return 0;

    if ((header.mode & 0xF000) == 0x8000)
    {

        unsigned int s = cpio_filesize(&header);
        unsigned int d = cpio_filedata(&header, address);

        if (offset >= s)
            return 0;

        count = size * count;
        s -= offset;

        return backend->write(d + offset, (count > s) ? s : count, buffer);

    }

    return 0;

}

static unsigned long protocol_getphysical(struct vfs_backend *backend, unsigned int id)
{

    /* TEMPORARY FIX */
    struct cpio_header header;
    unsigned int address = decode(id);

    if (!getheader(backend, &header, address))
        return 0;

    return backend->getphysical() + cpio_filedata(&header, address);

}

void vfs_setupcpio()
{

    vfs_initprotocol(&protocol, protocol_match, protocol_root, protocol_parent, protocol_child, protocol_create, protocol_destroy, protocol_open, protocol_close, protocol_read, protocol_write, protocol_getphysical);
    resource_register(&protocol.resource);

}
