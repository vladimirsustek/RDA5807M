#include "cmd_rda5807m.h"

uint16_t CmdRDA5807mDoInit(const uint8_t* const pStrCmd, const uint8_t lng) {

	RDA5807mInit();

	return CMD_RET_OK;
}

uint16_t CmdRDA5807mDoReset(const uint8_t* const pStrCmd, const uint8_t lng) {

	RDA5807mReset();
    return CMD_RET_OK;

}

uint16_t CmdRDA5807mSetMute(const uint8_t* const pStrCmd, const uint8_t lng) {

    uint8_t mute = pStrCmd[CMD_ARG_OFFSET + 0] - '0';

	uint16_t result = RDA5807mMute(mute);

	result = (RDA5807M_FN_OK == result) ? CMD_RET_OK : CMD_RET_ERR;

	return result;
}

uint16_t CmdRDA5807mSetFreq(const uint8_t* const pStrCmd, const uint8_t lng) {

    uint16_t freq = (pStrCmd[CMD_ARG_OFFSET + 0] - '0')*10000;

    freq += (pStrCmd[CMD_ARG_OFFSET + 1] - '0')*1000;
    freq += (pStrCmd[CMD_ARG_OFFSET + 2] - '0')*100;
    freq += (pStrCmd[CMD_ARG_OFFSET + 3] - '0')*10;
    freq += (pStrCmd[CMD_ARG_OFFSET + 4] - '0')*1;

	uint16_t result = RDA5807mSetFreq(freq);

    result = (RDA5807M_FN_OK == result) ? CMD_RET_OK : CMD_RET_ERR;

    return result;
}

uint16_t CmdRDA5807mSetVolm(const uint8_t* const pStrCmd, const uint8_t lng) {

    uint8_t volm;

    volm = (pStrCmd[CMD_ARG_OFFSET + 0] - '0')*10;
    volm += (pStrCmd[CMD_ARG_OFFSET + 1] - '0')*1;

	uint16_t result = RDA5807mSetVolm(volm);

    result = (RDA5807M_FN_OK == result) ? CMD_RET_OK : CMD_RET_ERR;

    return result;
}

uint16_t CmdRDA5807mGetRSSI(const uint8_t* const pStrCmd, const uint8_t lng) {

	return RDA5807mGetRSSI();
}

