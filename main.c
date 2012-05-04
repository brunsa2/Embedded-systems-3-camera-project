#include "usart.h"
#include "console.h"
#include "pwm.h"
#include "twi.h"
#include <stdio.h>
#include <string.h>

#define LINE_LENGTH 80
#define FALSE 0
#define TRUE 1

static volatile uint8_t row = 0;
static volatile uint8_t col = 0;
static volatile uint8_t active = FALSE;
static volatile uint8_t right_row = FALSE;
static volatile uint16_t sum_of_values = 0;

int main(void) {
	uint8_t result;
	char input[LINE_LENGTH];

	usart_init(16000000, 9600, USART_TRANSMIT | USART_RECEIVE);
	console_init(usart_putc, usart_getc);
	
	printf("\fEmbedded Systems III TWI test system\r\n\r\n");

	twi_init();

	result = twi_action(TW_START_COMMAND);
	if (result != TW_START) {
		printf("Start not sent\r\n0x%2x", result);
		while (1);
	}

	twi_send_data(0xc0);
	result = twi_action(TW_NO_COMMAND);
	if (result != TW_MT_SLAVE_ACKNOWLEDGE) {
		printf("Address not acknowledged\r\n0x%2x", result);
		while (1);
	}

	twi_send_data(0x11);
	result = twi_action(TW_NO_COMMAND);
	if (result != TW_MT_DATA_ACKNOWLEDGE) {
		printf("Subaddress not acknowledged\r\n0x%2x", result);
		while (1);
	}

	twi_send_data(0x3f);
	result = twi_action(TW_NO_COMMAND);
	if (result != TW_MT_DATA_ACKNOWLEDGE) {
		printf("Data not acknowledged\r\n0x%2x", result);
		while (1);
	}

	twi_action(TW_STOP_COMMAND);
	for(volatile int x = 0; x < 5000; x++);

	result = twi_action(TW_START_COMMAND);
	if (result != TW_START) {
		printf("Start not sent\r\n0x%2x", result);
		while (1);
	}

	twi_send_data(0xc0);
	result = twi_action(TW_NO_COMMAND);
	if (result != TW_MT_SLAVE_ACKNOWLEDGE) {
		printf("Address not acknowledged\r\n0x%2x", result);
		while (1);
	}

	twi_send_data(0x39);
	result = twi_action(TW_NO_COMMAND);
	if (result != TW_MT_DATA_ACKNOWLEDGE) {
		printf("Subaddress not acknowledged\r\n0x%2x", result);
		while (1);
	}

	twi_send_data(0x40);
	result = twi_action(TW_NO_COMMAND);
	if (result != TW_MT_DATA_ACKNOWLEDGE) {
		printf("Data not acknowledged\r\n0x%2x", result);
		while (1);
	}

	twi_action(TW_STOP_COMMAND);
	for(volatile int x = 0; x < 5000; x++);

	result = twi_action(TW_START_COMMAND);
	if (result != TW_START) {
		printf("Start not sent\r\n0x%2x", result);
		while (1);
	}

	twi_send_data(0xc0);
	result = twi_action(TW_NO_COMMAND);
	if (result != TW_MT_SLAVE_ACKNOWLEDGE) {
		printf("Address not acknowledged\r\n0x%2x", result);
		while (1);
	}

	twi_send_data(0x14);
	result = twi_action(TW_NO_COMMAND);
	if (result != TW_MT_DATA_ACKNOWLEDGE) {
		printf("Subaddress not acknowledged\r\n0x%2x", result);
		while (1);
	}

	twi_send_data(0x20);
	result = twi_action(TW_NO_COMMAND);
	if (result != TW_MT_DATA_ACKNOWLEDGE) {
		printf("Data not acknowledged\r\n0x%2x", result);
		while (1);
	}

	twi_action(TW_STOP_COMMAND);
	for(volatile int x = 0; x < 5000; x++);

	// INT0 PD2 PClk
	// INT1 PD3 HREF
	DDRD &= 0xff - ((1 << P2) | (1 << P3));
	MCUCR |= (1 << ISC11) | (1 << ISC10) | (1 << ISC01) | (1 << ISC00);
	GICR |= (1 << INT1) | (1 << INT0);

	// Setup port D for VSYNC
	DDRD &= 0xff - (1 << P6);
	//PORTD |= (1 << P6);

	// Setup port A for Y channel
	DDRA = 0x00;
	PORTA = 0xff;

	asm("sei");

	for (;;) {
		printf("Place a target and press enter\r\n");
		fgets(input, LINE_LENGTH, stdin);

		while (!(PIND & (1 << P6)));
		row = 0;
		col = 0;
		sum_of_values = 0;
		active = TRUE;
		while (PIND & (1 << P6));
		while (active == TRUE);
		sum_of_values >>= 4;
		sum_of_values &= 0xff;
		printf("Value: 0x%2x\r\n", sum_of_values);
	}

	return 0;
}

// PClk
ISR(INT0_vect) {
	uint8_t value = PINA;
	col++;
	if (active && right_row && col >=87 && col <= 102) {
		sum_of_values += value;
	}
}

// HREF
ISR(INT1_vect) {
	row++;
	if (row == 71) right_row = TRUE;
	if (active && row > 71) {
		right_row = FALSE;
		active = FALSE;
	}
	col = 0;
}
