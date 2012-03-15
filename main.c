#include "usart.h"
#include "console.h"
#include "pwm.h"
#include <stdio.h>
#include <string.h>

#define LINE_LENGTH 80

int main(void) {
	char line[LINE_LENGTH], token[LINE_LENGTH];
	uint32_t position;
	usart_init(16000000, 9600, USART_TRANSMIT | USART_RECEIVE);
	console_init(usart_putc, usart_getc);
	pwm_init();
	
	printf("\fEmbedded Systems III temporary shell\r\nfor demonstrations, week 2\r\n\r\n");

	while (1) {
		printf("> ");
		fgets(line, LINE_LENGTH, stdin);
		
		if (sscanf(line, "%s", token)) {
			if (strcmp(line, "horiz") == 0) {
			} else if (strcmp(line, "vert") == 0) {
			} else if (strcmp(line, "man") == 0) {
			} else {
				printf("Invalid command %s\r\n", token);
			}
		} else {
			printf("Invalid entry %s\r\n", line);
		}
	}

	return 0;
}
