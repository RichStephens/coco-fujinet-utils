#include <fujinet.h>
#include <cmoc.h>

int main(void)
{
	byte timebuf[6];
    byte cmd = OP_RESET1;
    
    memset(&timebuf, 0, sizeof(timebuf));

    fuji_ready(); 
    dwwrite((byte *) &cmd, 1);


    
	return 0;
}

