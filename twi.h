/**
 * TWI
 *
 * TWI driver
 *
 * @author Jeff Stubler
 * @date April 19, 2012
 */

#ifndef TWI
#define TWI

#include "atmega32.h"
#include <stdio.h>

#define TW_STATUS_MASK ((1 << TWS7) | (1 << TWS6) | (1 << TWS5) | (1 << TWS4) | (1 << TWS3))
#define TW_STATUS (TWSR & TW_STATUS_MASK)

#define TW_START 0x08
#define TW_MT_SLAVE_ACKNOWLEDGE 0x18
#define TW_MT_SLAVE_NOT_ACKNOWLEDGE 0x20
#define TW_MR_SLAVE_ACKNOWLEDGE 0x40
#define TW_MT_DATA_ACKNOWLEDGE 0x28
#define TW_MT_DATA_NOT_ACKNOWLEDGE 0x30
#define TW_MR_DATA_NOT_ACKNOWLEDGE 0x58
#define TW_REPEATED_START 0x10

#define TW_START_COMMAND (1 << TWSTA)
#define TW_STOP_COMMAND (1 << TWSTO)
#define TW_NO_COMMAND 0

/**
 * Initialize TWI interface
 */
void twi_init(void);


/**
 * TWI action for command and status
 * @param command TWI command
 * @return status
 */
uint8_t twi_action(uint8_t command);

/**
 * TWI send data
 * @param data Data to send on the TWI interface
 */
void twi_send_data(uint8_t data);

/**
 * TWI receive data
 * @return Data from TWI interface
 */
uint8_t twi_receive_data(void);

#endif
