#include "usart.h"
#include "console.h"
#include "pwm.h"
#include "twi.h"
#include <stdio.h>
#include <string.h>

#define LINE_LENGTH 80
#define FALSE 0
#define TRUE 1

// Camera/middle row positioning
static volatile uint8_t row = 0;
static volatile uint8_t col = 0;

// Manual threshold calibration
static volatile uint8_t thresh = 0xa0;
static volatile uint8_t output_picture = 1;

// Tracker
static volatile int16_t pan_servo = 1500;
static volatile int16_t tilt_servo = 1500;

// Row/column statistics gathering
#define ROWS 176
#define COLS 176
#define BUFFER_COUNT 2
static volatile uint8_t buffer = 0;
static volatile uint8_t working_buffer = 1;
static volatile uint8_t row_buffer[BUFFER_COUNT][ROWS + 1];
static volatile uint8_t col_buffer[BUFFER_COUNT][COLS + 1];




int main(void) {
	uint8_t result;

	usart_init(16000000, 9600, USART_TRANSMIT | USART_RECEIVE);
	console_init(usart_putc, usart_getc);
	pwm_init();
	pwm_set_pulse_length(PWM_CHANNEL_1, tilt_servo);	

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

	twi_send_data(0x12);
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

	twi_send_data(0x2f);
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

	// Clear buffers
	for (uint8_t buffer_index = 0; buffer_index < BUFFER_COUNT; buffer_index++) {
		for (uint8_t row_index = 0; row_index < ROWS + 1; row_index++) row_buffer[buffer_index][row_index] = 0;
		for (uint8_t col_index = 0; col_index < COLS + 1; col_index++) col_buffer[buffer_index][col_index] = 0;
	}

	asm("sei");

	for (;;) {
		while (!(PIND & (1 << P6)));
		
		// VSYNC is high
		printf("\fThreshold: 0x%2x\r\n", thresh);

		row = 0;
		col = 0;

		// Switch buffer
		buffer = 1 - buffer;
		working_buffer = 1 - working_buffer;

		// Clear buffers
		for (uint8_t row_index = 0; row_index < ROWS + 1; row_index++) row_buffer[buffer][row_index] = 0;
		for (uint8_t col_index = 0; col_index < COLS + 1; col_index++) col_buffer[buffer][col_index] = 0;

		while (PIND & (1 << P6));
		
		// Between processing and VSYNC pulse

		while (usart_hasc()) {
			int command = usart_getc(NULL);
			switch ((char) command) {
				case 'u':
					thresh++;
					break;
				case 'd':
					thresh--;
					break;
				case '+':
					thresh += 16;
					break;
				case '-':
					thresh -= 16;
					break;
				case 'p':
					output_picture = 1 - output_picture;
					break;
			}
		}

		uint8_t row_min = 0xff, row_max = 0x00, row_thresh;
		uint8_t col_min = 0xff, col_max = 0x00, col_thresh;

		for (uint8_t row_index = 0; row_index < ROWS + 1; row_index++) {
			row_min = (row_buffer[working_buffer][row_index] < row_min) ? row_buffer[working_buffer][row_index] : row_min;
			row_max = (row_buffer[working_buffer][row_index] > row_max) ? row_buffer[working_buffer][row_index] : row_max;
		}

		for (uint8_t col_index = 0; col_index < COLS + 1; col_index++) {
			col_min = (col_buffer[working_buffer][col_index] < col_min) ? col_buffer[working_buffer][col_index] : col_min;
			col_max = (col_buffer[working_buffer][col_index] > col_max) ? col_buffer[working_buffer][col_index] : col_max;
		}

		row_thresh = (row_min + row_max) >> 1;
		col_thresh = (col_min + col_max) >> 1;
		
		uint8_t current_count = 0, max_count = 0, last_point = 0;
		uint8_t row_start, row_end, row_mid, col_start, col_end, col_mid;

		for (uint8_t row_index = 0; row_index < ROWS + 1; row_index++) {
			if (row_buffer[working_buffer][row_index] > row_thresh) current_count++;
			else {
				if (current_count > max_count) {
					last_point = row_index - 1;
					max_count = current_count;
				}
				current_count = 0;
			}
			if (row_index == ROWS) printf("Hit end\r\n");
		}

		row_start = last_point - max_count + 1;
		row_end = last_point;
		row_mid = (row_start + row_end) >> 1;

		current_count = 0;
		max_count = 0;
		last_point = 0;

		for (uint8_t col_index = 0; col_index < COLS + 1; col_index++) {
			if (col_buffer[working_buffer][col_index] > col_thresh) current_count++;
			else {
				if (current_count > max_count) {
					last_point = col_index - 1;
					max_count = current_count;
				}
				current_count = 0;
			}
		}

		col_start = last_point - max_count + 1;
		col_end = last_point;
		col_mid = (col_start + col_end) >> 1;

		int8_t col_delta = col_mid - 88;
		int8_t row_delta = row_mid - 72;

		pan_servo -= 2*col_delta;
		tilt_servo += 2*row_delta;

		pan_servo = (pan_servo > 1750) ? 1750 : pan_servo;
		pan_servo = (pan_servo < 1250) ? 1250 : pan_servo;
		tilt_servo = (tilt_servo > 1750) ? 1750 : tilt_servo;
		tilt_servo = (tilt_servo < 1500) ? 1500 : tilt_servo;

		pwm_set_pulse_length(PWM_CHANNEL_0, pan_servo);
		pwm_set_pulse_length(PWM_CHANNEL_1, tilt_servo);

		if (1 - output_picture) {
			printf("Row min: %d, Row max: %d, Row thresh: %d\r\n", row_min, row_max, row_thresh);
			printf("Col min: %d, Col max: %d, Col thresh: %d\r\n", col_min, col_max, col_thresh);
			for (uint8_t row_index = 0; row_index < ROWS + 1; row_index++) {
				if (row_buffer[working_buffer][row_index] > row_thresh) usart_putc('@', NULL);
				else usart_putc('.', NULL);
			}

			printf("   %d to %d, mid %d\r\n", row_start, row_end, row_mid);

			for (uint8_t col_index = 0; col_index < COLS + 1; col_index++) {
				if (col_buffer[working_buffer][col_index] > col_thresh) usart_putc('@', NULL);
				else usart_putc('.', NULL);
			}

			printf("   %d to %d, mid %d\r\n", col_start, col_end, col_mid);

			printf("Pan: %d, Tilt: %d\r\n", pan_servo, tilt_servo);
			printf("DPan: %d, DTilt: %d\r\n", col_delta, row_delta);
		}
	}

	return 0;
}





// PClk
ISR(INT0_vect) {
	uint8_t value = PINA;
	uint8_t ascii_value = '@';

	if (value > thresh) {
		ascii_value = '.';
		row_buffer[buffer][row]++;
		col_buffer[buffer][col]++;
	}

	if (output_picture && (row & 0x01) == 0x01) usart_putc(ascii_value, NULL);

	col++;
}

// HREF
ISR(INT1_vect) {
	if (output_picture && (row & 0x01) == 0x01) {
		usart_putc('\r', NULL);
		usart_putc('\n', NULL);
	}

	row++;
	col = 0;
}
