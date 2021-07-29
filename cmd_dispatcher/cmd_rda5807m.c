#include "cmd_rda5807m.h"

#include "stdio.h"

#define LINE_FEED           ((char)('\n'))
#define COLON               ((char)(':'))

uint8_t CmdRDA5807mDoInit(const uint8_t* const pStrCmd, const uint8_t lng) {
    printf("RDA5807mInit\n");
    RDA5807mInit();
	return CMD_RET_OK;
}

uint8_t CmdRDA5807mDoReset(const uint8_t* const pStrCmd, const uint8_t lng) {
    printf("RDA5807mReset\n");
    RDA5807mReset();
	return CMD_RET_OK;
}

uint8_t CmdRDA5807mSetMute(const uint8_t* const pStrCmd, const uint8_t lng) {
	return CMD_RET_OK;
}

uint8_t CmdRDA5807mSetFreq(const uint8_t* const pStrCmd, const uint8_t lng) {
	return CMD_RET_OK;
}

uint8_t CmdRDA5807mSetVolm(const uint8_t* const pStrCmd, const uint8_t lng) {
	return CMD_RET_OK;
}

uint8_t CmdRDA5807mGetRSSI(const uint8_t* const pStrCmd, const uint8_t lng) {
	return CMD_RET_OK;
}
