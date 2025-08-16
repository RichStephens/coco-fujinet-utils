#include <coco.h>
#include <fujinet.h>

int main(void)
{
	byte timebuf[6];
    byte cmd = OP_TIME;

    memset(&timebuf, 0, sizeof(timebuf));

	fuji_ready();
    dwwrite((byte *) &cmd, 1);

    if (dwread(&timebuf, sizeof(timebuf)))
    {
        printf("%04d-%02d-%02d %02d:%02d:%02d\n", timebuf[0] + 1900,
			timebuf[1], timebuf[2], timebuf[3], timebuf[4], timebuf[5]);
    }
    else 
    {
        printf("FAILED TO GET TIME RESPONSE.\n");
    }

	return 0;
}

