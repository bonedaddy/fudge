#include <kernel.h>
#include "resource.h"
#include "vfs.h"
#include "task.h"
#include "scheduler.h"
#include "rendezvous.h"

unsigned int rendezvous_islocked(struct rendezvous *rendezvous)
{

    return rendezvous->task != 0;

}

unsigned int rendezvous_lock(struct rendezvous *rendezvous)
{

    if (rendezvous_islocked(rendezvous))
        return 0;

    rendezvous->task = scheduler_find_used_task();

    return 1;

}

unsigned int rendezvous_unlock(struct rendezvous *rendezvous)
{

    if (!rendezvous_islocked(rendezvous))
        return 0;

    rendezvous->task = 0;

    return 1;

}

unsigned int rendezvous_sleep(struct rendezvous *rendezvous, unsigned int condition)
{

    if (!condition)
        return 0;

    if (!rendezvous_islocked(rendezvous))
        return 0;

    if (!task_sleep(rendezvous->task))
        return 0;

    scheduler_block(rendezvous->task);

    return condition;

}

unsigned int rendezvous_unsleep(struct rendezvous *rendezvous, unsigned int condition)
{

    if (!condition)
        return 0;

    if (!rendezvous_islocked(rendezvous))
        return 0;

    if (!task_unsleep(rendezvous->task))
        return 0;

    scheduler_unblock(rendezvous->task);

    rendezvous->task->registers.ip -= 7;

    return condition;

}

void rendezvous_setup()
{

}

