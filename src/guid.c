#include <fujinet-fuji.h>

int main(void)
{
	char guid_buf[37];

    memset(guid_buf, 0, sizeof(guid_buf));


    if (fuji_generate_guid(guid_buf))
    {
        printf("%s\n", guid_buf);
    }
    else
    {
        printf("FAILED TO GET GENERATED GUID\n");
    }

	return 0;
}

