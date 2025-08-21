#include <coco.h>
#include <fujinet-fuji.h>

int main(void)
{
    DeviceSlot d[4];

    char rw;
    
    for (int i = 0; i < 4; i++) 
    {
        memset(&d[i], 0, sizeof(DeviceSlot)); 
    }

    fuji_get_device_slots(d,sizeof(d));
	
    for (int i = 0; i < 4; i++) 
    {
        if (d[i].hostSlot != 0xFF) 
        {
            switch (d[i].mode) 
            {
                case 0x01: rw = 'R'; break; // Read
                case 0x02: rw = 'W'; break; // Write
                default:   rw = '?'; break; // Unknown
            }
            printf("%d: (%d) (%c) %s\n", i, d[i].hostSlot+1, rw,  (char *)d[i].file);
        }
    }           

	return 0;
}