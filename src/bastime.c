#include <coco.h>
#include <fujinet-clock.h>
#include "basfuncs.h"

static char stringBuffer[20];
static uint8_t time_result[7];

void dateTimeCommand(void)
{
    // Get a variable reference, e.g, A$, X, B$(5), Y(3).
    void *varptr;
    BOOL isStringVar = getVariableReference(&varptr);

    if (isStringVar)
    {
        // Get the current date and time as a string, e.g., "2024-06-30 12:34:56"
        if (clock_get_time(time_result, SIMPLE_BINARY) == 0)
        {

            sprintf(stringBuffer, "%02d%02d-%02d-%02d %02d:%02d:%02d",
                time_result[0], time_result[1], time_result[2], time_result[3],
                time_result[4], time_result[5], time_result[6]);

            assignStringToVariable(varptr, stringBuffer, 20);
        }
    }
    else
    {
        printf("ERROR: EXPECTED STRING VARIABLE\n");
    }
}

int main(void)
{
    const char *newCmdName = "DATTM";
    replaceColorBasicCommand("SKIPF", newCmdName, dateTimeCommand);
    return 0;
}