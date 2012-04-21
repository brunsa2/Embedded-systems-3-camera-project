#include "usart.h"
#include "console.h"
#include "pwm.h"
#include "twi.h"
#include <stdio.h>
#include <string.h>

#define LINE_LENGTH 80

int main(void) {
	uint8_t result;

	usart_init(16000000, 9600, USART_TRANSMIT | USART_RECEIVE);
	console_init(usart_putc, usart_getc);
	
	printf("\fEmbedded Systems III TWI test system\r\n\r\n");

	twi_init();

	result = twi_action(TW_START_COMMAND);
	if (result == TW_START) {
		printf("Start sent\r\n");
	} else {
		printf("Start not sent\r\n0x%2x", result);
		while (1);
	}

	twi_send_data(0xc0);
	result = twi_action(TW_NO_COMMAND);
	if (result == TW_MT_SLAVE_ACKNOWLEDGE) {
		printf("Address sent and acknowledged\r\n");
	} else {
		printf("Address not acknowledged\r\n0x%2x", result);
		while (1);
	}

	twi_send_data(0x11);
	result = twi_action(TW_NO_COMMAND);
	if (result == TW_MT_DATA_ACKNOWLEDGE) {
		printf("Subaddress sent and acknowledged\r\n");
	} else {
		printf("Subaddress not acknowledged\r\n0x%2x", result);
		while (1);
	}

	result = twi_action(TW_START_COMMAND);
	if (result == TW_REPEATED_START) {
		printf("Repeated start sent\r\n");
	} else {
		printf("Repeated start not sent\r\n0x%2x", result);
		while (1);
	}

	twi_send_data(0x08);
	result = twi_action(TW_NO_COMMAND);
	if (result == TW_MT_SLAVE_NOT_ACKNOWLEDGE) {
		printf("Data sent and acknowledged\r\n");
	} else {
		printf("Data not acknowledged\r\n0x%2x", result);
		while (1);
	}


	result = twi_action(TW_STOP_COMMAND);
	printf("Stop sent\r\n0x%2x", result);



	result = twi_action(TW_START_COMMAND);
	if (result == TW_START) {
		printf("Start sent\r\n");
	} else {
		printf("Start not sent\r\n0x%2x", result);
		while (1);
	}

	twi_send_data(0xc0);
	result = twi_action(TW_NO_COMMAND);
	if (result == TW_MT_SLAVE_ACKNOWLEDGE) {
		printf("Address sent and acknowledged\r\n");
	} else {
		printf("Address not acknowledged\r\n0x%2x", result);
		while (1);
	}

	twi_send_data(0x39);
	result = twi_action(TW_NO_COMMAND);
	if (result == TW_MT_DATA_ACKNOWLEDGE) {
		printf("Subaddress sent and acknowledged\r\n");
	} else {
		printf("Subaddress not acknowledged\r\n0x%2x", result);
		while (1);
	}

	result = twi_action(TW_START_COMMAND);
	if (result == TW_REPEATED_START) {
		printf("Repeated start sent\r\n");
	} else {
		printf("Repeated start not sent\r\n0x%2x", result);
		while (1);
	}

	twi_send_data(0x40);
	result = twi_action(TW_NO_COMMAND);
	if (result == TW_MT_SLAVE_NOT_ACKNOWLEDGE) {
		printf("Data sent and acknowledged\r\n");
	} else {
		printf("Data not acknowledged\r\n0x%2x", result);
		while (1);
	}


	result = twi_action(TW_STOP_COMMAND);
	printf("Stop sent\r\n0x%2x", result);




	result = twi_action(TW_START_COMMAND);
	if (result == TW_START) {
		printf("Start sent\r\n");
	} else {
		printf("Start not sent\r\n0x%2x", result);
		while (1);
	}

	twi_send_data(0xc0);
	result = twi_action(TW_NO_COMMAND);
	if (result == TW_MT_SLAVE_ACKNOWLEDGE) {
		printf("Address sent and acknowledged\r\n");
	} else {
		printf("Address not acknowledged\r\n0x%2x", result);
		while (1);
	}

	twi_send_data(0x14);
	result = twi_action(TW_NO_COMMAND);
	if (result == TW_MT_DATA_ACKNOWLEDGE) {
		printf("Subaddress sent and acknowledged\r\n");
	} else {
		printf("Subaddress not acknowledged\r\n0x%2x", result);
		while (1);
	}

	result = twi_action(TW_START_COMMAND);
	if (result == TW_REPEATED_START) {
		printf("Repeated start sent\r\n");
	} else {
		printf("Repeated start not sent\r\n0x%2x", result);
		while (1);
	}

	twi_send_data(0x20);
	result = twi_action(TW_NO_COMMAND);
	if (result == TW_MT_SLAVE_NOT_ACKNOWLEDGE) {
		printf("Data sent and acknowledged\r\n");
	} else {
		printf("Data not acknowledged\r\n0x%2x", result);
		while (1);
	}


	result = twi_action(TW_STOP_COMMAND);
	printf("Stop sent\r\n0x%2x", result);



	while (1);

	return 0;
}
