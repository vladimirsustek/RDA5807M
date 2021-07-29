#include "cmd_rda5807m.h"

#include "stdio.h"

#define LINE_FEED           ((char)('\n'))
#define COLON               ((char)(':'))

uint8_t CmdRDA5807mDoInit(const uint8_t* const pStrCmd, const uint8_t lng) {

    RDA5807mInit();

	return CMD_RET_OK;
}

uint8_t CmdRDA5807mDoReset(const uint8_t* const pStrCmd, const uint8_t lng) {

    RDA5807mReset();

	return CMD_RET_OK;
}

uint8_t CmdRDA5807mSetMute(const uint8_t* const pStrCmd, const uint8_t lng) {

    uint8_t mute = pStrCmd[CMD_ARG_OFFSET + 0] - '0';

    RDA5807mMute(mute);

	return CMD_RET_OK;
}

uint8_t CmdRDA5807mSetFreq(const uint8_t* const pStrCmd, const uint8_t lng) {

    uint16_t freq;

    freq = (pStrCmd[CMD_ARG_OFFSET + 0] - '0')*10000;
    freq += (pStrCmd[CMD_ARG_OFFSET + 1] - '0')*1000;
    freq += (pStrCmd[CMD_ARG_OFFSET + 2] - '0')*100;
    freq += (pStrCmd[CMD_ARG_OFFSET + 3] - '0')*10;
    freq += (pStrCmd[CMD_ARG_OFFSET + 4] - '0')*1;

    RDA5807mSetFreq(freq);

	return CMD_RET_OK;
}

uint8_t CmdRDA5807mSetVolm(const uint8_t* const pStrCmd, const uint8_t lng) {

    uint8_t volm;

    volm = (pStrCmd[CMD_ARG_OFFSET + 0] - '0')*10;
    volm += (pStrCmd[CMD_ARG_OFFSET + 1] - '0')*1;

    RDA5807mSetVolm(volm);

	return CMD_RET_OK;
}

uint8_t CmdRDA5807mGetRSSI(const uint8_t* const pStrCmd, const uint8_t lng) {

    uint8_t rssi;

    rssi = RDA5807mGetRSSI();
    printf("RSSI: %d\n", rssi);

	return CMD_RET_OK;
}
