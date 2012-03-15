/**
 * PWM driver for Timer/Counter 1
 *
 * @author Jeff Stubler
 * @date March 13, 2012
 */

 // TODO: Rename PWM driver as servo driver and adjust params

#include "pwm.h"

/**
 * Initialize PWM system for pulse every 20 ms (1.5 ms pulse by default)
 */
void pwm_init(void) {
	ICR1H = (uint8_t) (39999 >> 8);
	ICR1L = (uint8_t) (39999 & 0xff);

	OCR1AH = (uint8_t) (1500 >> 8);
	OCR1AL = (uint8_t) (1500 & 0xff);
	OCR1BH = (uint8_t) (1500 >> 8);
	OCR1BL = (uint8_t) (1500 & 0xff);

	DDRD = (1 << P4) | (1 << P5);

	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << CS11);
}

/**
 * Set pwm to pulse length in microseconds on given channel
 * @param channel PWM channel
 * @param length PWM pulse length in microseconds
 */
 // TODO: Return error code if not initted or bad channel or so on
 // TODO: Make all 16-bit register writes with C use one line instead of two
 // TODO: Map channels 0 and 1 in an array to write to indexed by channel number
void pwm_set_pulse_length(uint8_t channel, uint16_t pulse_length) {
	switch (channel) {
		case PWM_CHANNEL_0:
			OCR1AH = (uint8_t) (pulse_length >> 8);
			OCR1AL = (uint8_t) (pulse_length & 0xff);
			break;
		case PWM_CHANNEL_1:
			OCR1BH = (uint8_t) (pulse_length >> 8);
			OCR1BL = (uint8_t) (pulse_length & 0xff);
	}
}
