/**
 * Console
 *
 * Wraps send and receive byte functions to form a console that user can edit add and create newlines which can
 * be accessible to standard IO
 *
 * @author Jeff Stubler
 * @date March 10, 2012
 */

#include "console.h"

static FILE stdio_stream;
static int(*console_used_putc)(char, FILE*);
static int(*console_used_getc)(FILE*);

static char console_buffer[CONSOLE_BUFFER_LENGTH];
static char *console_read_pointer;
static char *console_write_pointer; 

/**
 * Initialize console operations with given send and receive routines
 *
 * @param send Function to send byte
 * @param receive Function to receive byte
 */
void console_init(int(*send)(char, FILE*), int(*receive)(FILE*)) {
	console_read_pointer = console_write_pointer = console_buffer;
	console_used_putc = send;
	console_used_getc = receive;

	stdio_stream.put = console_putc;
	stdio_stream.get = console_getc;
	stdio_stream.flags = _FDEV_SETUP_RW;
	stdio_stream.udata = 0;
	stdin = &stdio_stream;
	stdout = &stdio_stream;
	stderr = &stdio_stream;
}

/**
 * Get character from the console (handles backspace and allows for an 80-character line)
 * @param unused File stream to enable stdio
 * @return Character from console
 */
int console_getc(FILE *unused) {
	char next_character;

	if (console_read_pointer == console_buffer) {
		for (console_write_pointer = console_buffer;;) {
			char input = console_used_getc(NULL);
			if (input == '\r') {
				input = '\n';
				*(console_write_pointer++) = input;
				console_used_putc('\r', NULL);
				console_used_putc('\n', NULL);
				console_read_pointer = console_buffer;
				break;
			}

			if (input >= ' ' && input <= '~') {
				if (console_write_pointer >= console_buffer + CONSOLE_BUFFER_LENGTH) {
					console_used_putc('\a', NULL);
				} else {
					*(console_write_pointer++) = input;
					console_used_putc(input, NULL);
				}
			}

			if (input == '\b' || input == '\x7f') {
				if (console_write_pointer > console_buffer) {
					console_used_putc('\b', NULL);
					console_used_putc(' ', NULL);
					console_used_putc('\b', NULL);
					console_write_pointer--;
				} else {
					console_used_putc('\a', NULL);
				}
			}
		}
	}

	next_character = *(console_read_pointer++);
	if (next_character == '\n') {
		console_read_pointer = console_buffer;
	}
	return next_character;
}

/**
 * Put character to the console
 * @param char_to_put Character to write to console
 * @param unused File stream to enable stdio
 * @return Nothing
 */
int console_putc(char char_to_put, FILE *unused) {
	return console_used_putc(char_to_put, unused);
}

/**
 * Put string to the console
 * @param string_to_push String to write to console
 * @param unused File stream to enable stdio
 */
void console_puts(char *string_to_put, FILE *unused) {
	while(*string_to_put != '\0')
			console_putc(*string_to_put++, unused);
}

/**
 * Get string from the console
 * @param string_to_get String to get from console
 * @param length Maximum length of string
 */
void console_gets(char *string_to_get, uint8_t length, FILE *unused) {
	char *current_character;
	int character;
	for(character = 0, current_character = string_to_get; character != '\n' && length > 0; length--, current_character++) {
		character = console_getc(unused);
		*current_character = (char) character;
	}
	*current_character = '\0';
}
