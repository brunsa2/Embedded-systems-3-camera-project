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
static volatile uint8_t active = FALSE;
static volatile uint8_t right_row = FALSE;

// Auto calibration
static volatile uint8_t min = 0xff;
static volatile uint8_t max = 0x00;
static volatile uint8_t thresh = 0x00;

// Edge detector
static volatile uint8_t color_mode = 0;
static volatile uint8_t noise_removal_counter = 0;
#define NOISE_COUNT 3

// Tracker
static volatile uint8_t first_col = 0;
static volatile uint8_t second_col = 0;
static volatile uint8_t col_waiting_for = 0;
static volatile uint16_t servo = 1500;


void run_servo(void) {
	if (((first_col + second_col) >> 1) > 90) servo += 20;
	if (((first_col + second_col) >> 1) < 86) servo -= 20;
	servo = (servo > 2000) ? 2000 : servo;
	servo = (servo < 1000) ? 1000 : servo;
	pwm_set_pulse_length(PWM_CHANNEL_0, servo);
}

int main(void) {
	uint8_t result;

	usart_init(16000000, 9600, USART_TRANSMIT | USART_RECEIVE);
	console_init(usart_putc, usart_getc);
	pwm_init();
	
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

		while (!(PIND & (1 << P6)));
		
		// VSYNC is high
		row = 0;
		col = 0;
		active = TRUE;
		
		printf("Min: 0x%2x, Max: 0x%2x\r\n", min, max);
		thresh = (min + max) >> 1;
		min = 0xff;
		max = 0x00;
		printf("Threshold: 0x%2x\r\n", thresh);

		while (PIND & (1 << P6));

		while (active == TRUE);
		
		// Between processing and VSYNC pulse
		run_servo();

	}

	return 0;
}




void alert_servo_to_column(uint8_t col) {
	if (col_waiting_for == 0) {
		first_col = col;
		col_waiting_for = 1;
	}
	if (col_waiting_for == 1) second_col = col;
}

// PClk
ISR(INT0_vect) {
	uint8_t value = PINA;
	col++;

	usart_putc('x', NULL);

	if (active && right_row) {
		min = (value < min) ? value : min;
		max = (value > max) ? value : max;

		if (value > thresh) usart_putc('*', NULL);
		else usart_putc('.', NULL);
		
		if ((color_mode == 0x00 && value > thresh) || (color_mode == 0xff && value < thresh)) {
			noise_removal_counter = 0;
		} else {
			noise_removal_counter++;
			if (noise_removal_counter > NOISE_COUNT) {
				
				alert_servo_to_column(col);

				noise_removal_counter = 0;
				color_mode = 0xff - color_mode;
			}
		}
	}
}

// HREF
ISR(INT1_vect) {
	row++;
	if (row == 71) right_row = TRUE;
	if (active && row > 71) {
		right_row = FALSE;
		active = FALSE;
		usart_putc('\r', NULL);
		usart_putc('\n', NULL);
	}
	col = 0;
	col_waiting_for = 0;
}
