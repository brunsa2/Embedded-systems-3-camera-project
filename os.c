/**
 * OS
 *
 * Priority-based real-time multitasking operating system
 *
 * @author Jeff Stubler
 * @date March 10, 2012
 */

#include "os.h"
#include "pwm.h"

#define OS_UNINITIALIZED_STATE 0
#define OS_RUNNING_STATE 1

typedef struct {
	uint8_t state;
	char name[5];
	uint32_t delay_ticks;
	uint16_t stack_pointer;
} process_control_block;

static process_control_block pcb[NUMBER_OF_PROCESSES];
static volatile uint8_t current_process;
static volatile uint16_t ticks = 0;

void enable_timer(void) {
	TCNT0 = 0;
	TCCR0 = (1 << WGM01) | (1 << CS01) | (1 << CS00); /// CTC mode, clk/64
	OCR0 = 25; // clk/64/25 = clk/1600
	TIMSK |= (1 << OCIE0);
}

NAKED_FUNCTION(schedule) {
	SAVE_CONTEXT();

	pcb[current_process].stack_pointer = STACK_HIGH << 8 | STACK_LOW;

	do {
		current_process = (current_process + 1) % NUMBER_OF_PROCESSES;
	} while (pcb[current_process].state == OS_UNINITIALIZED_STATE || pcb[current_process].delay_ticks > 0);

	STACK_HIGH = (uint8_t) (pcb[current_process].stack_pointer >> 8);
	STACK_LOW = (uint8_t) (pcb[current_process].stack_pointer & 0xff);

	RESTORE_CONTEXT();
	LEAVE_NAKED_FUNCTION();
}

/**
 * Initialize operating system
 */
 // TODO: Pass pointer to init task and init task space, set process to -1 and schedule() to call process 0, the init process.
 // TODO: os_init gets called first, this moves to init task stack space, os_start_multitasking fires up the timer
void os_init(void) {
	for (uint8_t pcb_index = 0; pcb_index < NUMBER_OF_PROCESSES; pcb_index++) {
		pcb[pcb_index].state = OS_UNINITIALIZED_STATE;
		strcpy(pcb[pcb_index].name, "");
		pcb[pcb_index].stack_pointer = 0;
	}

	pcb[0].state = OS_RUNNING_STATE;
	strcpy(pcb[0].name, "init");
	pcb[0].stack_pointer = STACK_HIGH << 8 | STACK_LOW;

	current_process = 0;

	enable_timer();

	asm("sei");
}

/**
 * Delay task for specified number of ticks
 *
 * @param pid Process ID to delay
 * @param ticks Number of ticks to delay
 */
void os_delay(uint8_t pid, uint32_t ticks) {
	ENTER_CRITICAL_SECTION();
	if (pid >= 0 && pid < NUMBER_OF_PROCESSES) pcb[pid].delay_ticks = ticks;
	LEAVE_CRITICAL_SECTION();
	schedule();
}

/**
 * Add new task to operating system
 */
void os_add_task(void (*task)(void), char *name) {
	ENTER_CRITICAL_SECTION();

	uint8_t current_pcb = 0;

	while (pcb[current_pcb].state != OS_UNINITIALIZED_STATE) {
		current_pcb++;
	}

	if (current_pcb >= NUMBER_OF_PROCESSES) {
		LEAVE_CRITICAL_SECTION();
		return;
	}

	pcb[current_pcb].state = OS_RUNNING_STATE;
	strcpy(pcb[current_pcb].name, name);
	pcb[current_pcb].stack_pointer = TOP_OF_MEMORY - current_pcb * STACK_SIZE;

	*(uint8_t *) pcb[current_pcb].stack_pointer = (uint8_t) ((uint16_t) task & 0xff);
	pcb[current_pcb].stack_pointer--;
	*(uint8_t *) pcb[current_pcb].stack_pointer = (uint8_t) ((uint16_t) task >> 8);
	pcb[current_pcb].stack_pointer--;

	pcb[current_pcb].stack_pointer -= 33;

	LEAVE_CRITICAL_SECTION();
}

ISR(TIMER0_COMPARE_MATCH_INTERRUPT) {
	ticks++;

	for (uint8_t pcb_index = 0; pcb_index < NUMBER_OF_PROCESSES; pcb_index++)
			if (pcb[pcb_index].delay_ticks > 0) pcb[pcb_index].delay_ticks--;


	if (ticks >= QUANTUM_MILLISECOND_LENGTH) {
		ticks = 0;
		schedule();		
	}
}
	

