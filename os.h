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
#include <string.h>

#include "atmega32.h"

/**
 * Maximum number of processes that can be managed
 */
#define NUMBER_OF_PROCESSES 4

/**
 * Length of each time quantum (tenth ms)
 */
#define QUANTUM_MILLISECOND_LENGTH 2000

/**
 * Stack size
 */
#define STACK_SIZE 128

/**
 * Initialize operating system
 */
void os_init(char *stack, uint16_t stack_size, void (*task)(void), char *name);

/**
 * Add new task to operating system
 */
void os_add_task(char *stack, uint16_t stack_size, void (*task)(void), char *name);

/**
 * Delay task for specified number of ticks
 *
 * @param pid Process ID to delay
 * @param ticks Number of ticks to delay
 */
void os_delay(uint8_t pid, uint32_t ticks);

/**
 *
 * @return Current PID
 */
uint8_t os_get_current_process(void);

#endif
