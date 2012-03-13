/**
 * OS
 *
 * Priority-based real-time multitasking operating system
 *
 * @author Jeff Stubler
 * @date March 10, 2012
 */

#ifndef OS
#define OS

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "atmega32.h"

/**
 * Maximum number of processes that can be managed
 */
#define NUMBER_OF_PROCESSES 4

/**
 * Length of each time quantum (tenth ms)
 */
#define QUANTUM_MILLISECOND_LENGTH 20000

/**
 * Stack size
 */
#define STACK_SIZE 128

/**
 * Initialize operating system
 *
 * @param idle_stack Pointer to top of stack frame for idle process
 */
void os_init(void *idle_stack);

#endif
