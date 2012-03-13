/**
 * OS
 *
 * Priority-based real-time multitasking operating system
 *
 * @author Jeff Stubler
 * @date March 10, 2012
 */

#include "os.h"

#define OS_UNINITIALIZED_STATE 0
#define OS_RUNNING_STATE 1

typedef struct {
	uint8_t state;
	char name[5];
	uint16_t stack_pointer;
} process_control_block;

static process_control_block pcb[NUMBER_OF_PROCESSES];
static volatile uint8_t current_process;
static volatile uint16_t millisecond_ticks = 0;

void enable_timer(void) {
	TCNT0 = 0;
	TCCR0 = (1 << WGM01) | (1 << CS01) | (1 << CS00); /// CTC mode, clk/64
	OCR0 = 25; // clk/64/25 = clk/1600
	TIMSK |= (1 << OCIE0);
}

/**
 * Initialize operating system
 *
 * @param idle_stack Pointer to top of stack frame for idle process
 */
void os_init(void *idle_stack) {
	for (uint8_t pcb_index = 0; pcb_index < NUMBER_OF_PROCESSES; pcb_index++) {
		pcb[pcb_index].state = OS_UNINITIALIZED_STATE;
		strcpy(pcb[pcb_index].name, "");
		pcb[pcb_index].stack_pointer = 0;
	}

	pcb[0].state = OS_RUNNING_STATE;
	strcpy(pcb[0].name, "idle");
	pcb[0].stack_pointer = STACK_HIGH << 8 | STACK_LOW;

	current_process = 0;

	enable_timer();

	asm("sei");
}

NAKED_ISR(TIMER0_COMPARE_MATCH_INTERRUPT) {
	SAVE_CONTEXT();
	millisecond_ticks++;
	if (millisecond_ticks >= QUANTUM_MILLISECOND_LENGTH) {
		millisecond_ticks = 0;
		
		//printf("Scheduling\r\n");

		uint16_t stack_pointer = STACK_HIGH << 8 | STACK_LOW;
		//printf("Stack at 0x%x\r\n", stack_pointer);

		pcb[current_process].stack_pointer = stack_pointer;

		stack_pointer = pcb[current_process].stack_pointer;

		STACK_HIGH = (uint8_t) (stack_pointer >> 8);
		STACK_LOW = (uint8_t) (stack_pointer & 0xff);

	}
	RESTORE_CONTEXT();
	LEAVE_NAKED_ISR();
}
	

