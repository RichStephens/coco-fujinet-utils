#include <cmoc.h>
#include <fujinet.h>

void fuji_ready(void)
{
    struct _readycmd
    {
        byte opcode;
        byte command;
    } rc;

    byte z=0, r=0;
    
    rc.opcode = OP_FUJI;
    rc.command = CMD_READY;
    
    while (!z)
    {
        dwwrite((byte *)&rc,sizeof(rc));
        z = dwread((byte *)&r,sizeof(r));
    }
}