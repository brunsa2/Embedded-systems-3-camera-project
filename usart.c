/**
 * USART driver
 *
 * Enables asynchronous serial communication, both receiving and transmitting, at single-character level
 *
 * @author Jeff Stubler
 * @date March 9, 2012
 */

#include "usart.h"

volatile static uint8_t usart_is_initialized = 0;

#define BAUD_HIGH 0
#define BAUD_LOW 207

 /**
  * Initialize USART with specified baud rate and options
  * @param clock MCU clock rate (Hz) to calculate the baud rate value for
  * @param baud Baud rate
  * @param flags Flags for options for serial port
  */
void usart_init(uint32_t clock, uint16_t baud, uint8_t flags) {
	UCSRB = 0;
	if (flags & USART_TRANSMIT) {
		// Enable transmitting
		UCSRB |= (1 << TXEN);
	}
	if (flags & USART_RECEIVE) {
		// Enable receiving with receive interrupts enabled
		UCSRB |= ((0 << RXCIE) | (1 << RXEN));
	}
	UCSRA = (1 << U2X);
	USRRH = (unsigned char) BAUD_HIGH;
	USRRL = (unsigned char) BAUD_LOW;

	usart_is_initialized = 1;
}

/**
 * Send one byte over USART
 * @param data Data byte to transmit
 * @param unusued Unused file descriptor to enable stdio use
 */
 // TODO: Error codes
int usart_putc(char data, FILE *unused) {
	if (usart_is_initialized) {
		while (!(UCSRA & (1 << UDRE)));
		UDR = data;
	}
	return 0;
}

/**
 * Receive one byte over USART
 * @param unused Unused file descriptor to enable stdio use
 * @return Byte from USART
 */
// TODO: Error codes
int usart_getc(FILE *unused) {
	if (usart_is_initialized) {
		while (!(UCSRA & (1 << RXC)));
		return UDR;
	}
	return 0;
}
