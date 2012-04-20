#include "usart.h"
#include "console.h"
#include "os.h"
#include "pwm.h"
#include <stdio.h>

#define MAIN_STACK_SIZE 512
char main_stack[MAIN_STACK_SIZE];

#define ALT_STACK_SIZE 512
char alt_stack[ALT_STACK_SIZE];

#define SERVO_STACK_SIZE 512
char servo_stack[SERVO_STACK_SIZE];

void task_1(void);
void init_task(void);
void servo(void);

void task_1(void) {
	while (1) {
		/*console_puts("The quick brown fox jumps over a lazy dog. Please enter your name: \r\n", NULL);
		char name[20];
		console_gets(name, 20, NULL);
		//printf("Hello, %s\r\n", name);
		console_puts("Hello, ", NULL);
		//console_puts(name, NULL);
		console_puts("\r\n", NULL);*/

		os_delay(1, 20000);
	}
}

void init_task(void) {
	usart_init(16000000, 9600, USART_TRANSMIT | USART_RECEIVE);
	console_init(usart_putc, usart_getc);
	pwm_init();

	os_add_task(alt_stack, ALT_STACK_SIZE, task_1, "t1");
	os_add_task(servo_stack, SERVO_STACK_SIZE, servo, "srvo");

	//console_puts("\fHello\r\n", NULL);
	while(1);
}

void servo(void) {
	while(1) {
		os_delay(2, 20000);
		pwm_set_pulse_length(PWM_CHANNEL_0, 1250);
		os_delay(2, 20000);
		pwm_set_pulse_length(PWM_CHANNEL_0, 1750);
	};
}

int main(void) {
	os_init(main_stack, MAIN_STACK_SIZE, init_task, "init");
	return 0;
}
