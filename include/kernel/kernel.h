#ifndef KERNEL_H
#define KERNEL_H

struct vfs_node;

extern void arch_init();
extern void arch_init_devices();
extern void arch_init_interrupts();
extern void arch_init_syscalls();
extern void arch_enable_interrupts();

extern vfs_node_t *fsRoot;

#endif

