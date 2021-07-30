#include "main.h"

int main(void)
{
    UARTinitiliaze(1);
    tim_tick_initialize();
    gpio_initialize();
    twi_init();

    sei();

    printf("Started!\n");

    while(1) {
        uint8_t* pBuff = UARTFetchReceivedLine();
        if (NULL != pBuff) {
            //printf((char*)pBuff);
            CmdDispatch(pBuff, 0);
        }
    }

    return 0;
}
