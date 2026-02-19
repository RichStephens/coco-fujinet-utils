#include <coco.h>
#include <fujinet-fuji.h>

int main(void)
{
    HostSlot h[8];

    char rw;
    
    for (int i = 0; i < 8; i++) 
    {
        memset(&h[i], 0, sizeof(HostSlot)); 
    }

    fuji_get_host_slots(&h[0], sizeof(h));;

    for (int i = 0; i < 8; i++)
    {
        printf("%d %s\n", i + 1,  strupr((char *) &h[i]));
    }

    return 0;
}