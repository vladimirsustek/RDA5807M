#include "main.h"

int main(void)
{
    /* variables for CmdDispatch service */
    uint8_t rxStrlng;
    uint8_t* rxStrBuff = NULL;

    /* Initialize peripherals*/
    UARTinitiliaze(UART_ISR_MODE);
    tim_tick_initialize();
    gpio_initialize();
    twi_init();

    /* Enable interrupts */
    sei();

    /* printf redirected to UART in uart_interface.c*/
    printf("SYS_READY\n");

    while(1) {
        /* Check whether any new string has arrived (UART) */
        rxStrBuff = UARTFetchReceivedLine(&rxStrlng);
        if (NULL != rxStrBuff) {
            /* If so and is terminated by <LF>,
               process it as command*/
            CmdDispatch(rxStrBuff, rxStrlng);
        }
    }

    return 0;
}
