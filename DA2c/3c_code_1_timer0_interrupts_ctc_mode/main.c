// 3c_code_1_timer0_interrupts_ctc_mode.c
// Daniel Senda
// Square waveform with period of 0.725s and 60% duty cycle.

#include	<avr/io.h>
#include	<avr/interrupt.h>
volatile int O_F = 0;		// Used for overflow count.
int num = 0;				// Used for while loops.
int LED1 = 0b11111011;		// Output value used to turn LED on.

void initiate_timer()
{
	TCCR0B = 0b10;			// Sets CTC mode.
	TCCR0B = 0b00000101;	// Sets 1024 pre-scaler.
	TIMSK0 |= (1 << OCIE0A);// Enables output compare match interrupt.
	OCR0A = 255;			// Sets output compare register A to 255.
	sei();					// Enables global interrupts.
	TCNT0 = 0;				// Resets timer.
}

ISR(TIMER0_COMPA_vect)		// Keeps count of overflows.
{
	O_F++;					// Increments overflow count.
}

int main(void)
{
	DDRB = 0xff;			// Sets port b pins to outputs.
	initiate_timer();		// Function that initiates timer.
	while (1)				// While loop that generates wave.
	{
		PORTB = LED1;		// Turns LED connected to PB2 on.
		num = 0;			// Resets num value.
		TCNT0 = 0;			// Resets timer.
		while(num <= 0)		// For loop used to create delay.
		{
			if (O_F >= 18)	// If overflow count equals 18:
			{
				num++;		// Increment num to exit loop.
				O_F = 0;	// Reset overflow count.
			}
		}
		
		PORTB = 255;		// Turns LED off.
		num = 0;			// Resets num value.
		TCNT0 = 0;			// Resets timer.
		while(num <= 0)		// For loop used to create delay.
		{
			if (O_F >= 26)	// If overflow count equals 26:
			{
				num++;		// Increment num to exit loop.
				O_F = 0;	// Reset overflow count.
			}
		}
	}
}