#include "cmd_commands.h"
#include "cmd_defs.h"
#include "cmd_rda5807m.h"

#define CMD_TABLE_SIZE  (uint8_t)(6)

static const CmdDisp_t cmdTable[CMD_TABLE_SIZE] = {
    {CMD_METHOD_DO,  CMD_RDA5807M_INIT, CmdRDA5807mDoInit},
    {CMD_METHOD_DO,  CMD_RDA5807M_RSET, CmdRDA5807mDoReset},
    {CMD_METHOD_SET, CMD_RDA5807M_VOLM, CmdRDA5807mSetVolm},
    {CMD_METHOD_SET, CMD_RDA5807M_FREQ, CmdRDA5807mSetFreq},
    {CMD_METHOD_SET, CMD_RDA5807M_MUTE, CmdRDA5807mSetMute},
    {CMD_METHOD_GET, CMD_RDA5807M_RSSI, CmdRDA5807mGetRSSI}
};

uint16_t CmdDispatch(const uint8_t* const pStrCmd, const uint8_t lng) {

    uint16_t result = CMD_RET_UKN;

    for(uint8_t idx = 0; idx < CMD_TABLE_SIZE; idx++) {

        if (!memcmp(pStrCmd, cmdTable[idx].cmdMethod, CMD_METHOD_LNG) &&
        !memcmp(pStrCmd + CMD_METHOD_LNG + CMD_DELIMITER_LNG, cmdTable[idx].cmdName, CMD_NAME_LNG)) {

            result = cmdTable[idx].cmdFunc(pStrCmd, lng);

            printf((const char*)pStrCmd);
            printf("RESULT : %x\n", result);
            break;
        }
    }

    return result;
}
