#include <fudge.h>

void main()
{

    unsigned char date[FUDGE_BSIZE];
    unsigned char time[FUDGE_BSIZE];
    unsigned int woff = 0;

    if (!call_walk(CALL_L0, CALL_DR, 25, "system/clock/dev/rtc/date"))
        return;

    if (!call_walk(CALL_L1, CALL_DR, 25, "system/clock/dev/rtc/time"))
        return;

    call_open(CALL_O0, CALL_OPEN_WRITE);
    call_open(CALL_L0, CALL_OPEN_READ);

    woff += call_write(CALL_O0, woff, call_read(CALL_L0, 0, FUDGE_BSIZE, date), date);
    woff += call_write(CALL_O0, woff, 1, " ");

    call_close(CALL_L0);
    call_open(CALL_L1, CALL_OPEN_READ);

    woff += call_write(CALL_O0, woff, call_read(CALL_L1, 0, FUDGE_BSIZE, time), time);
    woff += call_write(CALL_O0, woff, 1, "\n");

    call_close(CALL_L1);
    call_close(CALL_O0);

}

