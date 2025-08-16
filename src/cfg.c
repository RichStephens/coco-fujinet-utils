#include <coco.h>
#include <fujinet.h>


int main(void)
{
    byte cmd = OP_RESET1;
    fuji_ready(); 
    dwwrite((byte *) &cmd, 1);

    // Reset the coco itself
    coldStart();
    
	return 0;
}

