#include <fudge.h>
#include <kernel.h>
#include <modules/base/base.h>
#include <modules/system/system.h>
#include <modules/timer/timer.h>
#include <modules/arch/x86/pic/pic.h>
#include <modules/arch/x86/io/io.h>
#include <modules/arch/x86/platform/platform.h>
#include "pit.h"

#define FREQUENCY                       1193182
#define REGISTERCHANNEL0                0x0000
#define REGISTERCHANNEL1                0x0001
#define REGISTERCHANNEL2                0x0002
#define REGISTERCOMMAND                 0x0003
#define COMMANDBINARY                   0x00
#define COMMANDBCD                      0x01
#define COMMANDMODE0                    0x00
#define COMMANDMODE1                    0x02
#define COMMANDMODE2                    0x04
#define COMMANDMODE3                    0x06
#define COMMANDMODE4                    0x08
#define COMMANDMODE5                    0x0A
#define COMMANDLATCH                    0x00
#define COMMANDLOW                      0x10
#define COMMANDHIGH                     0x20
#define COMMANDBOTH                     0x30
#define COMMANDCHANNEL0                 0x00
#define COMMANDCHANNEL1                 0x40
#define COMMANDCHANNEL2                 0x80
#define COMMANDREADBACK                 0xC0

static struct base_driver driver;
static struct timer_interface timerinterface;
static unsigned short io;
static unsigned short divisor;
static unsigned int jiffies;

void pit_wait(unsigned int ms)
{

    unsigned short d = ms * (FREQUENCY / 1000);

    io_outb(io + REGISTERCOMMAND, COMMANDCHANNEL0 | COMMANDBOTH | COMMANDMODE0);
    io_outb(io + REGISTERCHANNEL0, d >> 8);
    io_outb(io + REGISTERCHANNEL0, d);
    io_outb(io + REGISTERCOMMAND, COMMANDREADBACK | 0x22);

    while (!(io_inb(io + REGISTERCHANNEL0) & (1 << 7)));

    io_outb(io + REGISTERCOMMAND, COMMANDCHANNEL0 | COMMANDBOTH | COMMANDMODE3);
    io_outb(io + REGISTERCHANNEL0, divisor);
    io_outb(io + REGISTERCHANNEL0, divisor >> 8);

}

static void handleirq(unsigned int irq)
{

    jiffies++;

    if (jiffies >= 60)
    {

        timer_notifytick(&timerinterface, jiffies);

        jiffies = 0;

    }

}

static void driver_init(unsigned int id)
{

    jiffies = 0;
    divisor = FREQUENCY / 60;

    timer_initinterface(&timerinterface, id);

}

static unsigned int driver_match(unsigned int id)
{

    return id == PLATFORM_PIT;

}

static void driver_reset(unsigned int id)
{

    io = platform_getbase(id);

    io_outb(io + REGISTERCOMMAND, COMMANDCHANNEL0 | COMMANDBOTH | COMMANDMODE3);
    io_outb(io + REGISTERCHANNEL0, divisor);
    io_outb(io + REGISTERCHANNEL0, divisor >> 8);

}

static void driver_attach(unsigned int id)
{

    timer_registerinterface(&timerinterface);
    pic_setroutine(platform_getirq(id), handleirq);

}

static void driver_detach(unsigned int id)
{

    timer_unregisterinterface(&timerinterface);
    pic_unsetroutine(platform_getirq(id));

}

void module_init(void)
{

    base_initdriver(&driver, "pit", driver_init, driver_match, driver_reset, driver_attach, driver_detach);

}

void module_register(void)
{

    base_registerdriver(&driver, PLATFORM_BUS);

}

void module_unregister(void)
{

    base_unregisterdriver(&driver, PLATFORM_BUS);

}

