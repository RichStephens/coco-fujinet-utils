#include <coco.h>
#include <fujinet.h>
/**
 * @brief Get response data from last Fuji command
 * @param buf Target buffer (needs to be at least len)
 * @param len Length 
 * @return Success status, true if the response was received successfully.
**/

int fuji_get_response(byte *buf, int len)
{
    struct _getresponsecmd
    {
        byte opcode;
        byte command;
    } grc;

    grc.opcode = OP_FUJI;
    grc.command = CMD_RESPONSE;

    fuji_ready();
    dwwrite((byte *)&grc, sizeof(grc));
    dwread((byte *)buf, len);
    
    return 0;
}
