#include <coco.h>
#include <fujinet-fuji.h>

int main(void)
{
    DeviceSlot d[4];
    
    for (int i = 0; i < 4; i++) 
    {
        memset(&d[i], 0, sizeof(DeviceSlot)); 
    }

    fuji_get_device_slots(d,sizeof(d));
	
    for (int i = 0; i < 4; i++) 
    {
        if (d[i].hostSlot != 0xFF) 
        {
            printf("%d: %s\n", i, (char *)d[i].file);
        }
    }           

	return 0;
}