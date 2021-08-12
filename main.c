#include "main.h"

int main(void)
{
    UARTinitiliaze(1);
    tim_tick_initialize();
    gpio_initialize();
    twi_init();

    sei();

    printf("SYS_READY\n");

    while(1) {
        uint8_t lng = 0;
        uint8_t* pBuff = UARTFetchReceivedLine(&lng);
        if (NULL != pBuff) {
            printf("Lng: %d\n", lng);
            CmdDispatch(pBuff, lng);
            pBuff = NULL;
        }
    }

    return 0;
}
