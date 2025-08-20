#include <coco.h>
#include <dw.h>
#include <fujinet-fuji-coco.h>


int main(void)
{
    byte cmd = 0xFF; // Command to reset the CoCo
    bus_ready(); 
    dwwrite((byte *) &cmd, 1);

    // Reset the coco itself
    coldStart();
    
	return 0;
}

