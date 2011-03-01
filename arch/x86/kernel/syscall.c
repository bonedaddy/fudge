#include <lib/call.h>
#include <lib/elf.h>
#include <lib/file.h>
#include <lib/vfs.h>
#include <kernel/kernel.h>
#include <kernel/vfs.h>
#include <modules/elf/elf.h>
#include <arch/x86/modules/io/io.h>
#include <arch/x86/kernel/arch.h>
#include <arch/x86/kernel/isr.h>
#include <arch/x86/kernel/mmu.h>
#include <arch/x86/kernel/syscall.h>

void *syscallRoutines[SYSCALL_ROUTINES_SIZE];

void syscall_register_handler(unsigned char index, void (*handler)(struct syscall_registers *registers))
{

    syscallRoutines[index] = handler;

}

void syscall_unregister_handler(unsigned char index)
{

    syscallRoutines[index] = 0;

}

static void syscall_open(struct syscall_registers *registers)
{

    registers->eax = (unsigned int)file_find(vfs_get_root(), (char *)registers->esi + 1);

}

static void syscall_map(struct syscall_registers *registers)
{

    unsigned int address = registers->ebx;

    if (!elf_check(address))
    {

        registers->eax = 0;

        return;

    }

    struct elf_header *header = (struct elf_header *)address;
    struct elf_program_header *programHeader = (struct elf_program_header *)(address + header->programHeaderOffset);

    unsigned int index = (programHeader->virtualAddress / MMU_PAGE_SIZE) / MMU_DIRECTORY_SIZE;

    struct mmu_directory *kernelDirectory = mmu_get_kernel_directory();
    struct mmu_table *programTable = mmu_get_program_table();

    mmu_clear_table(programTable);

    mmu_add_table(kernelDirectory, index, programTable, MMU_TABLE_FLAG_PRESENT | MMU_TABLE_FLAG_WRITEABLE | MMU_TABLE_FLAG_USERMODE);
    mmu_map(kernelDirectory, programHeader->virtualAddress, address, programHeader->memorySize, MMU_PAGE_FLAG_PRESENT | MMU_PAGE_FLAG_WRITEABLE | MMU_PAGE_FLAG_USERMODE);

    registers->eax = header->entry;

}

static void syscall_reboot(struct syscall_registers *registers)
{

    arch_reboot();

}


void syscall_handler(struct syscall_registers *registers)
{

    void (*handler)(struct syscall_registers *registers) = syscallRoutines[registers->eax];

    if (handler)
        handler(registers);

}

void syscall_init()
{

    syscall_register_handler(SYSCALL_ROUTINE_OPEN, syscall_open);
    syscall_register_handler(SYSCALL_ROUTINE_MAP, syscall_map);
    syscall_register_handler(SYSCALL_ROUTINE_REBOOT, syscall_reboot);

}

