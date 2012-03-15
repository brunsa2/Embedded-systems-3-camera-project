/**
 * Port definitions for ATmega32
 *
 * @author Jeff Stubler
 * @date March 9, 2012
 */

#ifndef PORT
#define PORT

#define _VECTOR(N) __vector_ ## N

/* Status register */

#define SREG (*((volatile uint8_t *)(0x5f)))

/* Stack */

#define STACK_HIGH (*((volatile uint8_t *)(0x5e)))
#define STACK_LOW (*((volatile uint8_t *)(0x5d)))

/* GPIO ports */

#define DDRD (*((volatile uint8_t *)(0x31)))

#define P4 4
#define P5 5

/* USART I/O ports */

#define UCSRA (*((volatile uint8_t *)(0x2b)))
#define RXC 7
#define UDRE 5
#define U2X 1

#define UCSRB (*((volatile uint8_t *)(0x2a)))
#define RXCIE 7
#define RXEN 4
#define TXEN 3

#define USRRL (*((volatile uint8_t *)(0x29)))
#define USRRH (*((volatile uint8_t *)(0x40)))
#define UDR (*((volatile uint8_t *)(0x2c)))

/* Timer/Counter 0 ports */

#define TCCR0 (*((volatile uint8_t *)(0x53)))
#define WGM01 3
#define CS02 2
#define CS01 1
#define CS00 0

#define OCR0 (*((volatile uint8_t *)(0x5c)))

#define TCNT0 (*((volatile uint8_t *)(0x52)))

#define TIMER0_COMPARE_MATCH_INTERRUPT _VECTOR(10)

/* Timer/Counter 1 ports */

#define TCCR1A (*((volatile uint8_t *)(0x4f)))
#define COM1A1 7
#define COM1A0 6
#define COM1B1 5
#define COM1B0 4
#define WGM11 1
#define WGM10 0

#define TCCR1B (*((volatile uint8_t *)(0x4e)))
#define WGM13 4
#define WGM12 3
#define CS12 2
#define CS11 1
#define CS10 0

#define OCR1AH (*((volatile uint8_t *)(0x4b)))
#define OCR1AL (*((volatile uint8_t *)(0x4a)))
#define OCR1BH (*((volatile uint8_t *)(0x49)))
#define OCR1BL (*((volatile uint8_t *)(0x48)))
#define ICR1H (*((volatile uint8_t *)(0x47)))
#define ICR1L (*((volatile uint8_t *)(0x46)))

/* Shared timer ports */

#define TIMSK (*((volatile uint8_t *)(0x59)))
#define OCIE0 1

/* Memory information */

#define TOP_OF_MEMORY 0x085f

/* Interrupts */

#define __INTR_ATTRS used, externally_visible

#define ISR(vector) \
    void vector(void) __attribute__ ((signal,__INTR_ATTRS)); \
    void vector(void)

/* Naked functions and interrupts */

#define NAKED_ISR(vector) \
	void vector(void) __attribute__ ((signal, naked,__INTR_ATTRS)); \
	void vector(void)

#define NAKED_FUNCTION(function) \
	void function(void) __attribute__ ((naked, noinline)); \
	void function(void)

/* Clothing for naked functions */

#define SAVE_CONTEXT() \
	asm volatile ( \
		"push r0 \n\t" \
		"in r0, __SREG__ \n\t" \
		"cli \n\t" \
		"push r0 \n\t" \
		"push r1 \n\t" \
		"clr r1 \n\t" \
		"push r2 \n\t" \
		"push r3 \n\t" \
		"push r4 \n\t" \
		"push r5 \n\t" \
		"push r6 \n\t" \
		"push r7 \n\t" \
		"push r8 \n\t" \
		"push r9 \n\t" \
		"push r10 \n\t" \
		"push r11 \n\t" \
		"push r12 \n\t" \
		"push r13 \n\t" \
		"push r14 \n\t" \
		"push r15 \n\t" \
		"push r16 \n\t" \
		"push r17 \n\t" \
		"push r18 \n\t" \
		"push r19 \n\t" \
		"push r20 \n\t" \
		"push r21 \n\t" \
		"push r22 \n\t" \
		"push r23 \n\t" \
		"push r24 \n\t" \
		"push r25 \n\t" \
		"push r26 \n\t" \
		"push r27 \n\t" \
		"push r28 \n\t" \
		"push r29 \n\t" \
		"push r30 \n\t" \
		"push r31 \n\t" \
	)

#define RESTORE_CONTEXT() \
	asm volatile ( \
		"pop r31 \n\t" \
		"pop r30 \n\t" \
		"pop r29 \n\t" \
		"pop r28 \n\t" \
		"pop r27 \n\t" \
		"pop r26 \n\t" \
		"pop r25 \n\t" \
		"pop r24 \n\t" \
		"pop r23 \n\t" \
		"pop r22 \n\t" \
		"pop r21 \n\t" \
		"pop r20 \n\t" \
		"pop r19 \n\t" \
		"pop r18 \n\t" \
		"pop r17 \n\t" \
		"pop r16 \n\t" \
		"pop r15 \n\t" \
		"pop r14 \n\t" \
		"pop r13 \n\t" \
		"pop r12 \n\t" \
		"pop r11 \n\t" \
		"pop r10 \n\t" \
		"pop r9 \n\t" \
		"pop r8 \n\t" \
		"pop r7 \n\t" \
		"pop r6 \n\t" \
		"pop r5 \n\t" \
		"pop r4 \n\t" \
		"pop r3 \n\t" \
		"pop r2 \n\t" \
		"pop r1 \n\t" \
		"pop r0 \n\t" \
		"out __SREG__, r0 \n\t" \
		"pop r0 \n\t" \
	)

#define LEAVE_NAKED_ISR() asm("reti");
#define LEAVE_NAKED_FUNCTION() asm("ret");

/* Critical sections */

#define ENTER_CRITICAL_SECTION() \
	uint8_t flags = SREG; \
	asm volatile ("cli");
	
#define LEAVE_CRITICAL_SECTION() \
	SREG = flags;

#endif
