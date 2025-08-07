#include <fujinet.h>
#include <cmoc.h>

int main(void)
{
    DeviceSlot d[4];
    
    for (int i = 0; i < 4; i++) 
    {
        memset(&d[i], 0, sizeof(DeviceSlot)); 
    }

	struct _get_device_slots
    {
        byte opcode;
        byte cmd;
    } gdsc;

    gdsc.opcode = OP_FUJI;
    gdsc.cmd = 0xF2;

    fuji_ready();
    dwwrite((byte *)&gdsc, sizeof(gdsc));

    fuji_ready();
    fuji_get_response((byte *)&d[0], 152);    

    for (int i = 0; i < 4; i++) {
        if (d[i].hostSlot != 0xFF) 
        {
            printf("%d: %s\n", i, (char *)d[i].file);
        }
    }           

	return 0;
}