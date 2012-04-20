/**
 * Console
 *
 * Wraps send and receive byte functions to form a console that user can edit add and create newlines which can
 * be accessible to standard IO
 *
 * @author Jeff Stubler
 * @date March 10, 2012
 */

#ifndef CONSOLE
#define CONSOLE

#include <stdio.h>

/**
 * Length of console input buffer
 */
#define CONSOLE_BUFFER_LENGTH 80

/**
 * Initialize console operations with given send and receive routines
 *
 * @param send Function to send byte
 * @param receive Function to receive byte
 */
void console_init(int(*send)(char, FILE*), int(*receive)(FILE*));

/**
 * Get character from the console (handles backspace and allows for an 80-character line)
 * @param unused File stream to enable stdio
 * @return Character from console
 */
int console_getc(FILE *unused);

/**
 * Put character to the console
 * @param char_to_put Character to write to console
 * @param unused File stream to enable stdio
 * @return Nothing
 */
int console_putc(char char_to_put, FILE *unused);

/**
 * Put string to the console
 * @param string_to_push String to write to console
 * @param unused File stream to enable stdio
 */
void console_puts(char *string_to_put, FILE *unused);

/**
 * Get string from the console
 * @param string_to_get String to get from console
 * @param length Maximum length of string
 */
void console_gets(char *string_to_get, uint8_t length, FILE *unused);

#endif
