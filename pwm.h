/**
 * PWM driver for Timer/Counter 1
 *
 * @author Jeff Stubler
 * @date March 13, 2012
 */

#ifndef PWM
#define PWM

/**
 * PWM channels
 */
#define PWM_CHANNEL_0 0
#define PWM_CHANNEL_1 1

/**
 * Initialize PWM system for pulse every 20 ms (no pulse by default)
 */
void pwm_init(void);

/**
 * Set pwm to pulse length in microseconds on given channel
 * @param channel PWM channel
 * @param length PWM pulse length in microseconds
 */
void pwm_set_pulse_length(

#endif
