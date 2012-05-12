#include <lib/memory.h>
#include <kernel/error.h>
#include <kernel/kernel.h>
#include <kernel/modules.h>
#include <kernel/ramdisk.h>
#include <kernel/runtime.h>
#include <kernel/syscall.h>

static struct kernel_arch *kernelArch;

void kernel_disable_interrupts()
{

    kernelArch->disable_interrupts();

}

void kernel_enable_interrupts()
{

    kernelArch->enable_interrupts();

}

void kernel_halt()
{

    kernelArch->halt();

}

void kernel_arch_init(struct kernel_arch *arch, void (*setup)(struct kernel_arch *arch), void (*halt)(), void (*enable_interrupts)(), void (*disable_interrupts)(), void (*enter_usermode)(unsigned int ip, unsigned int sp), unsigned int ramdiskc, void **ramdiskv)
{

    memory_clear(arch, sizeof (struct kernel_arch));

    arch->setup = setup;
    arch->halt = halt;
    arch->enable_interrupts = enable_interrupts;
    arch->disable_interrupts = disable_interrupts;
    arch->enter_usermode = enter_usermode;
    arch->ramdiskc = ramdiskc;
    arch->ramdiskv = ramdiskv;

}

void kernel_init(struct kernel_arch *arch)
{

    error_assert(arch != 0, "Architecture not found", __FILE__, __LINE__);

    kernelArch = arch;
    kernelArch->setup(kernelArch);

    modules_init();
    syscall_init();
    runtime_init();
    ramdisk_init(kernelArch->ramdiskc, kernelArch->ramdiskv);

    struct runtime_task *task = syscall_execute("/ramdisk/bin/init");

    error_assert(task != 0, "Init not found", __FILE__, __LINE__);

    kernelArch->enter_usermode(task->registers.ip, task->registers.sp);

}

