#include <fudge/kernel.h>
#include <kernel/error.h>
#include <kernel/vfs.h>
#include <kernel/task.h>
#include <kernel/container.h>
#include <kernel/kernel.h>
#include "arch.h"
#include "cpu.h"
#include "gdt.h"
#include "idt.h"
#include "multi.h"
#include "tss.h"

#define ARCH_STACK                      0x00400000
#define ARCH_GDT_DESCRIPTORS            6
#define ARCH_IDT_DESCRIPTORS            256
#define ARCH_TSS_DESCRIPTORS            1

static struct
{

    struct container *container;
    struct task *task;
    struct {struct gdt_pointer pointer; struct gdt_descriptor descriptors[ARCH_GDT_DESCRIPTORS];} gdt;
    struct {struct idt_pointer pointer; struct idt_descriptor descriptors[ARCH_IDT_DESCRIPTORS];} idt;
    struct {struct tss_pointer pointer; struct tss_descriptor descriptors[ARCH_TSS_DESCRIPTORS];} tss;
    struct {unsigned short code; unsigned short data;} kselector;
    struct {unsigned short code; unsigned short data;} uselector;
    struct {unsigned short info;} tselector;

} state;

unsigned short arch_generalfault(void *stack)
{

    struct {struct cpu_general general; unsigned int selector; struct cpu_interrupt interrupt;} *registers = stack;

    if (registers->interrupt.code == state.kselector.code)
        return state.kselector.data;

    return registers->interrupt.data;

}

unsigned short arch_pagefault(void *stack)
{

    struct {struct cpu_general general; unsigned int type; struct cpu_interrupt interrupt;} *registers = stack;

    state.container->map(state.container, state.task, cpu_get_cr2());
    state.task = state.container->schedule(state.container);

    if (registers->interrupt.code == state.kselector.code)
        return state.kselector.data;

    return registers->interrupt.data;

}

unsigned short arch_syscall(void *stack)
{

    struct {struct cpu_general general; struct cpu_interrupt interrupt;} *registers = stack;

    state.task->registers.ip = registers->interrupt.eip;
    state.task->registers.sp = registers->interrupt.esp;
    state.task->registers.fp = registers->general.ebp;
    state.task->registers.status = (state.container->calls[registers->general.eax]) ? state.container->calls[registers->general.eax](state.container, state.task, (void *)registers->interrupt.esp) : 0;
    state.task = state.container->schedule(state.container);

    if (state.task->state & TASK_STATE_USED)
    {

        registers->interrupt.code = state.uselector.code;
        registers->interrupt.eip = state.task->registers.ip;
        registers->interrupt.esp = state.task->registers.sp;
        registers->general.ebp = state.task->registers.fp;
        registers->general.eax = state.task->registers.status;

        return state.uselector.data;

    }

    registers->interrupt.code = state.kselector.code;
    registers->interrupt.eip = (unsigned int)arch_halt;
    registers->interrupt.esp = ARCH_STACK;
    registers->general.ebp = 0;
    registers->general.eax = 0;

    return state.kselector.data;

}

void arch_setup(unsigned int count, struct kernel_module *modules)
{

    gdt_init_pointer(&state.gdt.pointer, ARCH_GDT_DESCRIPTORS, state.gdt.descriptors);
    idt_init_pointer(&state.idt.pointer, ARCH_IDT_DESCRIPTORS, state.idt.descriptors);
    tss_init_pointer(&state.tss.pointer, ARCH_TSS_DESCRIPTORS, state.tss.descriptors);

    state.kselector.code = gdt_set_descriptor(&state.gdt.pointer, GDT_INDEX_KCODE, 0x00000000, 0xFFFFFFFF, GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_ALWAYS1 | GDT_ACCESS_RW | GDT_ACCESS_EXECUTE, GDT_FLAG_GRANULARITY | GDT_FLAG_32BIT);
    state.kselector.data = gdt_set_descriptor(&state.gdt.pointer, GDT_INDEX_KDATA, 0x00000000, 0xFFFFFFFF, GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_ALWAYS1 | GDT_ACCESS_RW, GDT_FLAG_GRANULARITY | GDT_FLAG_32BIT);
    state.uselector.code = gdt_set_descriptor(&state.gdt.pointer, GDT_INDEX_UCODE, 0x00000000, 0xFFFFFFFF, GDT_ACCESS_PRESENT | GDT_ACCESS_RING3 | GDT_ACCESS_ALWAYS1 | GDT_ACCESS_RW | GDT_ACCESS_EXECUTE, GDT_FLAG_GRANULARITY | GDT_FLAG_32BIT);
    state.uselector.data = gdt_set_descriptor(&state.gdt.pointer, GDT_INDEX_UDATA, 0x00000000, 0xFFFFFFFF, GDT_ACCESS_PRESENT | GDT_ACCESS_RING3 | GDT_ACCESS_ALWAYS1 | GDT_ACCESS_RW, GDT_FLAG_GRANULARITY | GDT_FLAG_32BIT);
    state.tselector.info = gdt_set_descriptor(&state.gdt.pointer, GDT_INDEX_TSS, (unsigned int)state.tss.pointer.descriptors, (unsigned int)state.tss.pointer.descriptors + state.tss.pointer.limit, GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_EXECUTE | GDT_ACCESS_ACCESSED, 0);

    idt_set_descriptor(&state.idt.pointer, IDT_INDEX_GF, arch_isr_generalfault, state.kselector.code, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_TYPE32INT);
    idt_set_descriptor(&state.idt.pointer, IDT_INDEX_PF, arch_isr_pagefault, state.kselector.code, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_TYPE32INT);
    idt_set_descriptor(&state.idt.pointer, IDT_INDEX_SYSCALL, arch_isr_syscall, state.kselector.code, IDT_FLAG_PRESENT | IDT_FLAG_RING3 | IDT_FLAG_TYPE32INT);
    tss_set_descriptor(&state.tss.pointer, TSS_INDEX_DEFAULT, state.kselector.data, ARCH_STACK);
    cpu_set_gdt(&state.gdt.pointer);
    cpu_set_idt(&state.idt.pointer);
    cpu_set_tss(state.tselector.info);

    state.container = kernel_setup();
    state.task = multi_setup(state.container);

    kernel_setup_modules(state.container, state.task, count, modules);
    arch_usermode(state.uselector.code, state.uselector.data, state.task->registers.ip, state.task->registers.sp);

}
