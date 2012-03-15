/**
 * PWM driver for Timer/Counter 1
 *
 * @author Jeff Stubler
 * @date March 13, 2012
 */

#ifndef PWM
#define PWM

#include <inttypes.h>

#include "atmega32.h"

/**
 * PWM channels
 */
#define PWM_CHANNEL_0 0
#define PWM_CHANNEL_1 1

/**
 * Initialize PWM system for pulse every 20 ms (1.5 ms pulse by default)
 */
void pwm_init(void);

/**
 * Set pwm to pulse length in microseconds on given channel
 * @param channel PWM channel
 * @param length PWM pulse length in microseconds
 */
 // TODO: Return error code if not initted or bad channel or so on
 // TODO: Make all 16-bit register writes with C use one line instead of two
 // TODO: Map channels 0 and 1 in an array to write to indexed by channel number
void pwm_set_pulse_length(uint8_t channel, uint16_t pulse_length);

#endif
