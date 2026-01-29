#include <coco.h>
#include <fujinet-fuji.h>


int main(void)
{
    byte cmd = 0xFF; // Command to reset the CoCo

    fuji_reset();

    // Reset the coco itself
    coldStart();
    
	return 0;
}

