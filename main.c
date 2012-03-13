#include "usart.h"
#include "console.h"
#include "os.h"
#include "pwm.h"
#include <stdio.h>

int main(void) {
	usart_init(16000000, 9600, USART_TRANSMIT | USART_RECEIVE);
	console_init(usart_putc, usart_getc);

	printf("The quick brown fox jumps over a lazy dog. Please enter your name: \r\n");
	/*char name[80];
	fgets(name, 80, stdin);
	printf("Hello, %s", name);*/

	//os_init();

	while (1) {
		//printf("x");
	}

	return 0;
}
