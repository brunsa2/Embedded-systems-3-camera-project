#include "usart.h"
#include "console.h"
#include "os.h"
#include "pwm.h"
#include <stdio.h>

void task_1(void);

void task_1(void) {
	while (1) {
		printf("The quick brown fox jumps over a lazy dog. Please enter your name: \r\n");
		char name[80];
		fgets(name, 80, stdin);
		printf("Hello, %s", name);

		os_delay(1, 20000);
	}
}

int main(void) {
	usart_init(16000000, 9600, USART_TRANSMIT | USART_RECEIVE);
	console_init(usart_putc, usart_getc);
	
	pwm_init();
	os_init();
	os_add_task(task_1, "t1");
	
	while (1);

	return 0;
}
