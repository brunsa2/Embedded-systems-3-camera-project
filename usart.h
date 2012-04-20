/**
 * USART driver
 *
 * Enables asynchronous serial communication, both receiving and transmitting, at single-character level
 *
 * @author Jeff Stubler
 * @date March 9, 2012
 */

#ifndef USART
#define USART

#include <inttypes.h>
#include <stdio.h>

#include "atmega32.h"
#include "os.h"

/**
 * Enable transmitting
 */
#define USART_TRANSMIT 0x01

/**
 * Enable receiving
 */
#define USART_RECEIVE 0x02

/**
 * Initialize USART with specified baud rate and options
 * @param baud Baud rate
 * @param clock_rate MCU clock rate (kHz) to calculate the baud rate value for
 * @param flags Flags for options for serial port
 * @return Nothing
 */
void usart_init(uint32_t clock, uint16_t baud, uint8_t flags);

/**
 * Send one byte over USART
 * @param data Data byte to transmit
 * @param unusued Unused file descriptor to enable stdio use
 */
int usart_putc(char data, FILE *unused);

/**
 * Receive one byte over USART
 * @param unused Unused file descriptor to enable stdio use
 * @return Byte from USART
 */
int usart_getc(FILE *unused); 

#endif
