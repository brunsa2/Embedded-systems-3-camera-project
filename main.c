#include "usart.h"
#include "console.h"
#include "pwm.h"
#include <stdio.h>
#include <string.h>

#define LINE_LENGTH 80

int main(void) {
	char line[LINE_LENGTH], token[LINE_LENGTH];
	int position, data;
	usart_init(16000000, 9600, USART_TRANSMIT | USART_RECEIVE);
	console_init(usart_putc, usart_getc);
	pwm_init();
	
	printf("\fEmbedded Systems III temporary shell\r\nfor demonstrations, week 2\r\n\r\n");

	while (1) {
		printf("> ");
		fgets(line, LINE_LENGTH, stdin);
		
		if (sscanf(line, "%s", token)) {
			if (strcmp(token, "horiz") == 0 || strcmp(token, "vert") == 0) {
				if (sscanf(line, "%s %d", token, &position) == 2) {

					if (position < 0) position = 0;
					if (position > 1000) position = 1000;
					
					if (strcmp(token, "horiz") == 0) {
						printf("Moving to position %d horizontally\r\n", position);
						pwm_set_pulse_length(PWM_CHANNEL_0, (uint16_t) (1000+position));
					} else if (strcmp(token, "vert") == 0) {
						printf("Moving to position %d vertically\r\n", position);
						pwm_set_pulse_length(PWM_CHANNEL_1, (uint16_t) (1000+position));
					}

				} else {
					printf("Invalid formed command \"%s\"\r\n", line);
				}
			} else if (strcmp(token, "rdio") == 0) {
				if (sscanf(line, "%s %x", token, &position) == 2) {
					printf("Reading address 0x%x is 0x%x\r\n", position, *(uint8_t *)position);
				} else {
					printf("Invalid formed command \"%s\"\r\n", line);
				}	
			} else if (strcmp(token, "wrio") == 0) {
				if (sscanf(line, "%s %x %x", token, &position, &data) == 3) {
					printf("Writing 0x%x to address 0x%x\r\n", position, data);
					*(uint8_t *) position = (uint8_t) data;
				} else {
					printf("Invalid formed command \"%s\"\r\n", line);
				}
			} else if (strcmp(token, "help") == 0) {
				printf("vert pos - Move vertically to position pos from 0 to 1000\r\n");
				printf("horiz pos - Move horizontally to position pos from 0 to 1000\r\n");
				printf("rdio addr - Read address addr\r\n");
				printf("wrio addr data - Write data data to address addr\r\n");
				printf("help - Show this page\r\n\r\n");
			} else {
				printf("Invalid command \"%s\"\r\n", token);
			}
		} else {
			printf("Invalid entry %s\r\n", line);
		}

		strcpy(token, "");
	}

	return 0;
}
