#include "usart.h"
#include "console.h"
#include "pwm.h"
#include <stdio.h>

#define LINE_LENGTH 80

int main(void) {
	char line[LINE_LENGTH];
	usart_init(16000000, 9600, USART_TRANSMIT | USART_RECEIVE);
	console_init(usart_putc, usart_getc);
	pwm_init();
	
	printf("\fEmbedded Systems III temporary shell\r\nfor demonstrations, week 2\r\n\r\n");

	while (1) {
		printf("> ");
		fgets(line, LINE_LENGTH, stdin);
		printf("%s\r\n", line);
	}

	return 0;
}
