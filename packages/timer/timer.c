#include <arch.h>
#include <fudge.h>

void main()
{

    unsigned char buffer[FUDGE_BSIZE];

    if (!call_open(CALL_D0, CALL_DR, 25, "system/nodefs/pit_jiffies"))
        return;

    call_write(CALL_DO, 0, call_read(CALL_D0, 0, FUDGE_BSIZE, buffer), buffer);
    call_close(CALL_D0);

}

