#include <cmoc.h>
#include <coco.h>
#include <fujinet-fuji.h>

#define KEY_LEFT_ARROW       0x08
#define KEY_RIGHT_ARROW      0x09
#define KEY_UP_ARROW         0x5E
#define KEY_DOWN_ARROW       0x0A
#define KEY_SHIFT_UP_ARROW   0x5F
#define KEY_SHIFT_DOWN_ARROW 0x5B
#define KEY_ENTER            0x0D
#define KEY_BREAK            0x03
#define KEY_CLEAR            0x0C

#define MAX_HOST_LEN 32
#define NUM_HOST_SLOTS 8
#define DIR_MAX_LEN 31
#define MAX_APP_KEYS 64
#define EMPTY_FILTER ""

typedef struct 
{
    uint16_t creator_id;
    uint8_t  app_id;
    uint8_t  key_id;
    uint16_t key_size;
    uint8_t  key_value[65];
} KeyInfo;

HostSlot hostSlots[8];
char response[256];
KeyInfo app_keys[MAX_APP_KEYS];
int num_appkeys = 0;

bool isprint(byte c)
{
    // Check if c falls within the ASCII range for printable characters (0x20 to 0x7E)
    // This range includes space (0x20) up to tilde (0x7E).
    return (c >= 0x20 && c <= 0x7E);
}

void printstr(uint8_t *s, uint16_t size)
{
    uint16_t count = 0;
    while (count < size)
    {
        char c = *s++;
        if (isprint(c))
        {
            putchar(c);
        }
        else
        {
            // Print non-printable characters in hexadecimal format
            printf(" 0X%02X ", c);
        }
        count++;
    }
}

uint16_t get_keyvalue(uint8_t *buffer, uint16_t creatorid, uint8_t appid, uint8_t keyid)
{
    uint8_t read_buffer[65];
    bool key_result = false;
    bool read_result = false;
    uint16_t read_count = 0;

    fuji_set_appkey_details(creatorid, appid, DEFAULT);

    memset(read_buffer, 0, sizeof(read_buffer));
    key_result = fuji_read_appkey(keyid, &read_count, read_buffer);
    if (key_result && read_count > 0)
    {
        memcpy(buffer, read_buffer, read_count);
        read_buffer[read_count] = 0;
        read_result = true;
    }

    return read_count;
}

int get_appkeys(void)
{
    KeyInfo key_info;
    char creator_key_id_buf[5];
    char key_id_buf[3];
    int count = 0;

    fuji_get_host_slots(&hostSlots[0], NUM_HOST_SLOTS);
    fuji_close_directory();
    fuji_mount_host_slot(0);

    if (fuji_error())
    {
        printf("FAILED TO MOUNT\nSD CARD HOST SLOT.\n");
        return 0;
    }

    fuji_open_directory2(0, "/FujiNet/", EMPTY_FILTER);

    if (fuji_error())
    {
        printf("FAILED TO OPEN\nFUJINET DIRECTORY.\n");
        return 0;
    }

    memset(response, 0, sizeof(response));

    while (count < MAX_APP_KEYS)
    {
        memset(&key_info, 0, sizeof(key_info));
        memset(creator_key_id_buf, 0, sizeof(creator_key_id_buf));
        memset(key_id_buf, 0, sizeof(key_id_buf));
        fuji_read_directory(DIR_MAX_LEN, 0, response);

        if (response[1] == 0x7F)
        {
            break;
        }
        else
        {
            strncpy(creator_key_id_buf, response, 4);
            app_keys[count].creator_id = (uint16_t) atoul16(creator_key_id_buf);
            strncpy(key_id_buf, &response[4], 2);
            app_keys[count].app_id = (uint8_t) atoul16(key_id_buf);
            strncpy(key_id_buf, &response[6], 2);
            app_keys[count].key_id = (uint8_t) atoul16(key_id_buf);
            app_keys[count].key_size = get_keyvalue(app_keys[count].key_value, app_keys[count].creator_id,
                app_keys[count].app_id, app_keys[count].key_id);
            count++;
        }
    }

    fuji_close_directory();

    return count;
}

void show_key_value(int key_ndx, bool raw)
{
    cls(1);

    if (raw)
    {
        printstr(app_keys[key_ndx].key_value, MAX_APPKEY_LEN);
    }
    else
    {
        printf("%s", (char *) app_keys[key_ndx].key_value);
    }

    locate(0,15);
    printf("PRESS ANY KEY...");
    waitkey(0);
}

void print_app_key(int key_ndx)
{

    cls(1);
    printf("KEY %d OF %d\n\n", key_ndx + 1, num_appkeys);
    printf("CREATOR ID: 0X%04X\n", app_keys[key_ndx].creator_id);
    printf("    APP ID: 0X%02X\n", app_keys[key_ndx].app_id);
    printf("    KEY ID: 0X%02X\n", app_keys[key_ndx].key_id);
    printf("\n");

    printf("up/dn BROWSE APP KEYS\n");
    printf("s SHOW STRING KEY VALUE\n");
    printf("r SHOW RAW KEY VALUE\n");
    printf("break TO EXIT");
}

int main(void)
{
    cls(1);
    int key_ndx = 0;
    byte ch;
    
    num_appkeys = get_appkeys();

    if (num_appkeys > 0)
    {
        print_app_key(key_ndx);

        while (true)
        {
            ch = waitkey(0);

            switch (ch)
            {
                case KEY_UP_ARROW: 
                if (key_ndx > 0)
                {
                    key_ndx--;
                }
                print_app_key(key_ndx);
                break;
                case KEY_DOWN_ARROW:
                if (key_ndx < num_appkeys-1)
                {
                    key_ndx++;
                }
                print_app_key(key_ndx);
                break;
                case 'R':
                case 'r':
                show_key_value(key_ndx, true);
                print_app_key(key_ndx);
                break;
                case 'S':
                case 's':
                show_key_value(key_ndx, false);
                print_app_key(key_ndx);
                break;
                case KEY_BREAK:
                    cls(1);
                    return(0);
                    break;
            }
        }
    }
    else
    {
        printf("NO APP KEYS FOUND.\n");
        return(1);
    }

	return(0);
}

