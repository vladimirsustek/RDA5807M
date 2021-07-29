/*
 * Created by Vladimir Sustek, 23.6.2019
 *
 * Feel free to use/modify/whatever
 *
**/
/***********************************************************************/
/*****************    Includes   ***************************************/
/***********************************************************************/
#include "uart_interface.h"

/***********************************************************************/
/*****************    Private macro *  *********************************/
/***********************************************************************/

/* Defined speed of UART stage in Baud, note that 57600 is maximal */
#define USART_BAUDRATE      (uint16_t)(57600)
/* Calculating prescaler to set an appropriate baud speed */
#define BAUD_PRESCALE       (uint16_t)(((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

/* Macro to substitute set bit of the UART busy line */
#define UART_BUFF_BUSY      (!(UCSR0A & (1 << UDRE0)))

/***********************************************************************/
/*****************    Private data     *********************************/
/***********************************************************************/

static int uart_putchar(char c, FILE *stream);
/* Output file declaration for UART output redirection */
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL,_FDEV_SETUP_WRITE);

static volatile int uart_lf_flag = 0;
static volatile uint8_t inner_buffer[UART_RX_BUFF_SIZE] = {0};
static volatile uint8_t inner_buff_i     = 0;


/***********************************************************************/
/*****************    Private functions    *****************************/
/***********************************************************************/

/* STDIO redirection of printf to appropriate UART.
 * Note that TX line is always enabled and disabled
   after each transmission (function call)*/
static int uart_putchar(char c, FILE *stream) {

    /* Enable TX line */
    UCSR0B|= (1 << TXEN0);

    /* Start UART transmission - fill buffer*/
    UDR0 = c;

    while (UART_BUFF_BUSY){
        /* Do nothing - stuck here*/
    }

    /* Disable TX line */
    UCSR0B&=~(1 << TXEN0);

    return 0;
}
/* @brief Disable UART interrupt (so its ISR)
 *
 *
 */
static void uart_enter_critical(void) {
    UCSR0B &=~(1 << RXCIE0);
}
/* @brief Enable UART interrupt (so its ISR)
 *
 *
 */
static void uart_exit_critical(void) {
    UCSR0B |=(1 << RXCIE0);
}

/* @brief USART RX ISR handler
 *
 * Functions stores received characters into the
 * static buffer (owned by this module) to the array
 * inner_buffer. In case of received <LF> character
 * or if more than UART_RX_BUFF_SIZE is received
 * without any reading the uart_lf_flag is set and
 * user may handle this event as the further receive
 * is disabled (RX interrupt disabled).
 *
 * @sa uart_received_LF
 * @sa uart_copy_buffer
 *
 */
ISR(USART_RX_vect){

    inner_buffer[inner_buff_i] = UDR0;

    if(inner_buffer[inner_buff_i] == '\n') {
            uart_enter_critical();
            uart_lf_flag = 1;
    }

    if(inner_buff_i < UART_RX_BUFF_SIZE - 1) {
        inner_buff_i++;
        }
    else {
        uart_enter_critical();
        uart_lf_flag = 1;
    }
}

/***********************************************************************/
/*****************    Public functions    ******************************/
/***********************************************************************/
/* @brief Initialize UART peripheral
 *
 * @param [in] ISR_Mode - optional interrupt on receive enable
 * @return [None]
 *
 * Function initializes AVR USART peripheral as the 8-bit,
 * further is by default set no parity, 1 stop bit.
 * The Baud Rate is set to maximal 57600 baud and constant
 * for clock prescaler is calculated in the MACROS section
 * It is intended to access the transmit stage by stdio
 * printf function, so stdout is customized as well.
 *
 */

void UARTinitiliaze(uint8_t isr_enable_flag) {

    /* Use 8 - bit size of transaction symbol */
    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
    /* Upper part of bitrate 57600*/
    UBRR0H |= (BAUD_PRESCALE >> 8);
    /* Lower part of bitrate 57600 */
    UBRR0L |= BAUD_PRESCALE;
    /* Enable Interrupt on receive */
    if(isr_enable_flag) {
            UCSR0B |= (1 << RXCIE0);
    }
    /* Force off the TX and RX for further enabling */
    //UCSR0B &= ~(1 << RXEN0) | ~(1 << TXEN0);

    /* Redirect output direction of stdio*/
    stdout = &mystdout;
}

/* @brief Has been <LF> character received ?
 *
 * @return uart_lf_flag - 0 or 1 according to the flag
 *
 * Function checks whether uart_lf_flag triggered by
 * uart_copy_buffer has been set (<LF> received). In
 * Valid case, function returns 1 and resets this flag.
 *
 */
uint8_t UARTisLFreceived(void) {
    uint8_t result = 0;
    if(uart_lf_flag) {
        result = uart_lf_flag;
    }
    return result;
}
/* @brief Copy content of received UART to buffer
 *
 * @return 0 in case of success, -1 otherwise
 *
 * Function copies content of buffer handled by
 * receiving ISR(USART_RX_vect) interrupt. After
 * the data are copied, buffer is cleared. Note
 * that function may return -1 if passed buffer
 * is NULL pointer. Maximal length of copied data
 * is determined by UART_RX_BUFF_SIZE - prevent
 * potential overflow passing sufficient buffer.
 *
 */
uint8_t UARTcopyBuffer(uint8_t * buffer){

    uart_enter_critical();

    uint8_t result = 0;
    if(buffer == NULL) {
        return -1;
    }

    memcpy(buffer, (uint8_t*)inner_buffer, inner_buff_i);
    memset((uint8_t*)inner_buffer, '\0', inner_buff_i);
    inner_buff_i = 0;
    uart_lf_flag = 0;
    result = inner_buff_i;

    uart_exit_critical();

    return result;
}

/* @brief
 *
 * @return
 *
 */
uint8_t* UARTFetchReceivedLine(void) {

    static uint8_t uart_received[UART_RX_BUFF_SIZE] = {0};

    uint8_t* pBuff = NULL;

    if(UARTisLFreceived()) {
        UARTcopyBuffer(uart_received);
        pBuff = uart_received;
    }
    return pBuff;
}

