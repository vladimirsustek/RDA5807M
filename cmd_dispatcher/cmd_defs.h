#ifndef CMD_DEFS_H_INCLUDED
#define CMD_DEFS_H_INCLUDED

#include <stdint.h>
#include <string.h>

#define CMD_DELIMITER_LNG   ((uint8_t)(1))
#define CMD_DELIMITER       ((uint8_t)('_'))

#define CMD_PLATFORM_LNG    ((uint8_t)(0))

#define CMD_NAME_LNG        ((uint8_t)(4))
#define CMD_METHOD_LNG      ((uint8_t)(2))

#define CMD_ARG_LNG         ((uint8_t)(5))

#define CMD_ARG_OFFSET      ((uint8_t)(CMD_PLATFORM_LNG + CMD_NAME_LNG + CMD_METHOD_LNG + CMD_DELIMITER_LNG*2))

#define CMD_RET_OK          ((uint8_t)(0u))
#define CMD_RET_ERR         ((uint8_t)(255u))

typedef uint8_t (*pfn_u8_cpuc8cu8)(const uint8_t* const, const uint8_t lng);

typedef struct CmdDisp {

    uint8_t cmdMethod[CMD_METHOD_LNG];
    uint8_t cmdName[CMD_NAME_LNG];
    pfn_u8_cpuc8cu8 cmdFunc;
} CmdDisp_t;

#endif // CMD_DEFS_H_INCLUDED
