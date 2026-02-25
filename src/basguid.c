#include <coco.h>
#include <fujinet-fuji.h>
#include "basfuncs.h"

static char guidBuf[37];

void guidCommand(void)
{
    // Get a variable reference, e.g, A$, X, B$(5), Y(3).
    void *varptr;
    BOOL isStringVar = getVariableReference(&varptr);

    if (isStringVar)
    {
        // Get a new GUID as a string, e.g., "123e4567-e89b-12d3-a456-426614174000"
        if (fuji_generate_guid(guidBuf))
        {
            assignStringToVariable(varptr, guidBuf, 37);
        }
    }
    else
    {
        printf("ERROR: EXPECTED STRING VARIABLE\n");
    }
}

int main(void)
{
    const char *newCmdName = "GUID";
    replaceColorBasicCommand("STOP", newCmdName, guidCommand);
    return 0;
}