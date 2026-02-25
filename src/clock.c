#include <fujinet-fuji.h>
#include <fujinet-clock.h>

#define SCREEN (byte *)0x0400
#define SCREEN_WIDTH 32
#define X0 1
#define Y0 4
#define X1 29
#define Y1 2
#define SPACE 0x20

static byte last_h = 255;
static byte last_m = 255;
static byte last_s = 255;
static byte last_century = 255;
static byte last_year = 255;
static byte last_month = 255;
static byte last_day = 255;

typedef enum 
{
    AMPM,
    MILITARY
} ClockMode;

const byte digits4x8[10][8][4] =
{
/* 0 */
{
    {' ','0','0',' '},
    {'0',' ',' ','0'},
    {'0',' ',' ','0'},
    {'0',' ',' ','0'},
    {'0',' ',' ','0'},
    {'0',' ',' ','0'},
    {'0',' ',' ','0'},
    {' ','0','0',' '}
},
/* 1 */
{
    {' ','1','1',' '},
    {'1','1','1',' '},
    {' ','1','1',' '},
    {' ','1','1',' '},
    {' ','1','1',' '},
    {' ','1','1',' '},
    {' ','1','1',' '},
    {'1','1','1','1'}
},
/* 2 */
{
    {' ','2','2',' '},
    {'2',' ',' ','2'},
    {' ',' ',' ','2'},
    {' ',' ','2',' '},
    {' ','2',' ',' '},
    {'2',' ',' ',' '},
    {'2',' ',' ','2'},
    {'2','2','2','2'}
},
/* 3 */
{
    {' ','3','3',' '},
    {'3',' ',' ','3'},
    {' ',' ',' ','3'},
    {' ','3','3',' '},
    {' ',' ',' ','3'},
    {' ',' ',' ','3'},
    {'3',' ',' ','3'},
    {' ','3','3',' '}
},
/* 4 */
{
    {' ',' ',' ','4'},
    {' ',' ','4','4'},
    {' ','4',' ','4'},
    {'4',' ',' ','4'},
    {'4','4','4','4'},
    {' ',' ',' ','4'},
    {' ',' ',' ','4'},
    {' ',' ',' ','4'}
},
/* 5 */
{
    {'5','5','5','5'},
    {'5',' ',' ',' '},
    {'5',' ',' ',' '},
    {'5','5','5',' '},
    {' ',' ',' ','5'},
    {' ',' ',' ','5'},
    {'5',' ',' ','5'},
    {' ','5','5',' '}
},
/* 6 */
{
    {' ','6','6',' '},
    {'6',' ',' ','6'},
    {'6',' ',' ',' '},
    {'6','6','6',' '},
    {'6',' ',' ','6'},
    {'6',' ',' ','6'},
    {'6',' ',' ','6'},
    {' ','6','6',' '}
},
/* 7 */
{
    {'7','7','7','7'},
    {' ',' ',' ','7'},
    {' ',' ',' ','7'},
    {' ',' ','7',' '},
    {' ',' ','7',' '},
    {' ','7',' ',' '},
    {' ','7',' ',' '},
    {' ','7',' ',' '}
},
/* 8 */
{
    {' ','8','8',' '},
    {'8',' ',' ','8'},
    {'8',' ',' ','8'},
    {' ','8','8',' '},
    {'8',' ',' ','8'},
    {'8',' ',' ','8'},
    {'8',' ',' ','8'},
    {' ','8','8',' '}
},
/* 9 */
{
    {' ','9','9',' '},
    {'9',' ',' ','9'},
    {'9',' ',' ','9'},
    {'9',' ',' ','9'},
    {' ','9','9','9'},
    {' ',' ',' ','9'},
    {'9',' ',' ','9'},
    {' ','9','9',' '}
}
};

const char *month_names[12] = {
    "JANUARY", "FEBRUARY", "MARCH", "APRIL",
    "MAY", "JUNE", "JULY", "AUGUST",
    "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER"
};

byte timebuf[7];

void draw_glyph(byte glyph, byte x)
{
    byte row, col;
    byte *dst;
    byte ch;
    dst = SCREEN + Y0 * 32 + x;
    for (row = 0; row < 8; row++)
    {
        for (col = 0; col < 4; col++)
        {
            ch = digits4x8[glyph][row][col];
            if (ch == ' ')
                dst[col] = ch | 0x40;  /* Space is normal video (black on green) */
            else
                dst[col] = ch;  /* Digit is reverse video (green on black) */
        }
        dst += 32;   /* next screen row */
    }
}

void draw_colon(byte x)
{
    byte row;
    byte *dst;
    const byte colon_pattern[8] = {' ', ':', ':', ' ', ':', ':', ' ', ' '};
    
    dst = SCREEN + Y0 * 32 + x;
    for (row = 0; row < 8; row++)
    {
        dst[0] = colon_pattern[row] | 0x40;  /* All colon characters in normal video */
        dst += 32;   /* next screen row */
    }
}

void draw_time(byte h, byte m, byte s, ClockMode mode)
{
    byte x = X0;

    if (mode == AMPM)
    {
        // draw AM/PM indicator (1 char wide)
        if (h >= 12)
        {
            locate(X1, Y1);
            printf("PM");
        }
        else
        {
            locate(X1, Y1);
            printf("AM");
        }

        if (h == 0)
            h = 12; /* Midnight */
        else if (h > 12)
            h -= 12; /* Convert to 12-hour format */
    }
    else
    {
        locate(X1, Y1);
        printf("  ");  /* Clear AM/PM area */
    }

    if (h != last_h)
    {
        draw_glyph(h / 10, x);
        draw_glyph(h % 10, x + 5);  /* 4 chars + 1 space */
        last_h = h;
    }
    x += 9;  /* 4 + 1 + 4 */
    
    draw_colon(x);   // colon (1 char wide)
    x += 1;
    
    if (m != last_m)
    {
        draw_glyph(m / 10, x);
        draw_glyph(m % 10, x + 5);  /* 4 chars + 1 space */
        last_m = m;
    }
    x += 9;  /* 4 + 1 + 4 */
    
    draw_colon(x);
    x += 1;
    
    if (s != last_s)
    {
        draw_glyph(s / 10, x);
        draw_glyph(s % 10, x + 5);  /* 4 chars + 1 space */
        last_s = s;
    }
}

void draw_date(byte century, byte year, byte month, byte day)
{
    const char *month_name;
    byte len, x;
    
    if (century == last_century && year == last_year && month == last_month && day == last_day)
        return;  /* No change */
    
    last_century = century;
    last_year = year;
    last_month = month;
    last_day = day;
    
    /* Get month name (month is 1-12, array is 0-11) */
    if (month >= 1 && month <= 12)
        month_name = month_names[month - 1];
    else
        month_name = "UNKNOWN";
    
    /* Calculate length of month name */
    len = 0;
    while (month_name[len] != '\0')
        len++;
    
    /* Calculate starting position to center date */
    /* Format: "MONTHNAME DD, YYYY" */
    /* Length is: len + 1 + 2 + 2 + 5 = len + 10 */
    x = (32 - (len + 10)) / 2;
    
    /* Position cursor and print date */
    locate(x, 15);
    printf("%s %02d, %02d%02d", month_name, day, century, year);
}

int main(void)
{
    ClockMode mode = MILITARY;  /* Default to 24-hour mode */
    width(32);
    cls(1);
    byte key = 0;


    while (true)
    {
        key = inkey();

        if (key == SPACE)
        {
            /* Toggle clock mode */
            if (mode == MILITARY)
                mode = AMPM;
            else
                mode = MILITARY;
        }
        else if (key != 0)
        {
            break; /* Exit on any other key press */
        }

        clock_get_time(timebuf, SIMPLE_BINARY);

        draw_time(timebuf[4], timebuf[5], timebuf[6], mode);
        draw_date(timebuf[0], timebuf[1], timebuf[2], timebuf[3]);
    }

    cls(1);
    return 0;
}