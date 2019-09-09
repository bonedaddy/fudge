#include <fudge.h>
#include <kernel.h>
#include <kernel/x86/cpu.h>
#include <kernel/x86/gdt.h>
#include <kernel/x86/idt.h>
#include <kernel/x86/tss.h>
#include <kernel/x86/arch.h>
#include <mboot/mboot.h>
#include <modules/base/base.h>
#include <modules/system/system.h>
#include <modules/video/video.h>

static struct video_interface videointerface;
static unsigned int *lfb;

static unsigned int videointerface_readctrl(struct system_node *self, struct system_node *current, struct service_state *state, void *buffer, unsigned int count, unsigned int offset)
{

    return memory_read(buffer, count, &videointerface.settings, sizeof (struct ctrl_videosettings), offset);

}

static unsigned int videointerface_writectrl(struct system_node *self, struct system_node *current, struct service_state *state, void *buffer, unsigned int count, unsigned int offset)
{

    video_notifymode(&videointerface, videointerface.settings.w, videointerface.settings.h, videointerface.settings.bpp);

    return count;

}

static unsigned int videointerface_readdata(struct system_node *self, struct system_node *current, struct service_state *state, void *buffer, unsigned int count, unsigned int offset)
{

    return memory_read(buffer, count, lfb, videointerface.settings.w * videointerface.settings.h * videointerface.settings.bpp, offset);

}

static unsigned int videointerface_writedata(struct system_node *self, struct system_node *current, struct service_state *state, void *buffer, unsigned int count, unsigned int offset)
{

    return memory_write(lfb, videointerface.settings.w * videointerface.settings.h * videointerface.settings.bpp, buffer, count, offset);

}

static unsigned int videointerface_readcolormap(struct system_node *self, struct system_node *current, struct service_state *state, void *buffer, unsigned int count, unsigned int offset)
{

    return 0;

}

static unsigned int videointerface_writecolormap(struct system_node *self, struct system_node *current, struct service_state *state, void *buffer, unsigned int count, unsigned int offset)
{

    return count;

}

void module_init(void)
{

    struct mboot_header *header = (void *)0x5000;
    unsigned int i;

    lfb = (unsigned int *)header->framebuffer.loaddr;
    arch_setmap(4, (unsigned int)lfb, (unsigned int)lfb, 0x00400000);
    arch_setmap(5, (unsigned int)lfb + 0x00400000, (unsigned int)lfb + 0x00400000, 0x00400000);

    video_initinterface(&videointerface, 0);
    ctrl_setvideosettings(&videointerface.settings, header->framebuffer.width, header->framebuffer.height, header->framebuffer.bpp / 8);

    videointerface.ctrl.operations.read = videointerface_readctrl;
    videointerface.ctrl.operations.write = videointerface_writectrl;
    videointerface.data.operations.read = videointerface_readdata;
    videointerface.data.operations.write = videointerface_writedata;
    videointerface.colormap.operations.read = videointerface_readcolormap;
    videointerface.colormap.operations.write = videointerface_writecolormap;

    for (i = 0; i < header->framebuffer.width * header->framebuffer.height; i++)
        lfb[i] = 0xFFFFFFFF;

}

void module_register(void)
{

    video_registerinterface(&videointerface);

}

void module_unregister(void)
{

    video_unregisterinterface(&videointerface);

}

