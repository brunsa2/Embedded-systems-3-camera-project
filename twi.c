/**
 * TWI
 *
 * TWI driver
 *
 * @author Jeff Stubler
 * @date April 19, 2012
 */

#include "twi.h"

/**
 * Initialize TWI interface
 */
void twi_init(void) {
	printf("TWI init\r\n");
	TWBR = 200;
	TWSR = (0 << TWPS1) | (0 << TWPS0);
	printf("TWI init complete\r\n");
}

/**
 * TWI action for command and status
 * @param command TWI command
 * @return status
 */
uint8_t twi_action(uint8_t command) {
	printf("Doing TWI action\r\n");
	// Clear interrupts, enable, command
	TWCR = (1 << TWINT) | (1 << TWEN) | command;
	// Wait for interrupt flag
	printf("Waiting...\r\n");
	if (command == TW_STOP_COMMAND) {
		printf("Stop, leave TWI action\r\n");
		return TW_STATUS;
	}
	while (!(TWCR & (1 << TWINT)));
	printf("Finished TWI action\r\n");
	return TW_STATUS;
}

/**
 * TWI send data
 * @param data Data to send on the TWI interface
 */
void twi_send_data(uint8_t data) {
	TWDR = data;
}

/**
 * TWI receive data
 * @return Data from TWI interface
 */
uint8_t twi_receive_data(void) {
	return TWDR;
}
