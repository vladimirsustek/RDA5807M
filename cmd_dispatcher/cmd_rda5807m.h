#ifndef CMD_RDA5807M_H_INCLUDED
#define CMD_RDA5807M_H_INCLUDED

#include "../rda5807m/rda5807m.h"
#include "cmd_defs.h"

uint8_t CmdRDA5807mDoInit(const uint8_t* const pStrCmd, const uint8_t lng);
uint8_t CmdRDA5807mDoReset(const uint8_t* const pStrCmd, const uint8_t lng);
uint8_t CmdRDA5807mSetMute(const uint8_t* const pStrCmd, const uint8_t lng);
uint8_t CmdRDA5807mSetFreq(const uint8_t* const pStrCmd, const uint8_t lng);
uint8_t CmdRDA5807mSetVolm(const uint8_t* const pStrCmd, const uint8_t lng);
uint8_t CmdRDA5807mGetRSSI(const uint8_t* const pStrCmd, const uint8_t lng);

#endif // CMD_RDA5807M_H_INCLUDED