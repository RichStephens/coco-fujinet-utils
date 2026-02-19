#include <cmoc.h>
#include <coco.h>
#include <fujinet-clock.h>

static uint8_t time_result[7];

int main(void)
{
    memset(time_result, 0, sizeof(time_result));

    if (clock_get_time(time_result, SIMPLE_BINARY) == 0)
    {
        printf("%02d%02d-%02d-%02d %02d:%02d:%02d\n", time_result[0], time_result[1], time_result[2], time_result[3], time_result[4], time_result[5], time_result[6]);
    }
    else 
    {
        printf("FAILED TO GET TIME RESPONSE.\n");
    }

	return 0;
}


