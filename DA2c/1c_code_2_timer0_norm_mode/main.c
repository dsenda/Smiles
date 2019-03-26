// 1c_code_2_timer0_norm_mode.c
// Daniel Senda
// Turn on LED for 1.25s after pushbutton is pressed.

#include	<avr/io.h>
int O_F = 0;			// Used for overflow count.

int main (void)
{
	DDRB = 0xFF;		// Set PORTB as output.
	DDRC &= (0 << 2);	// Set PORTC as input
	PORTC |= (1 << 2);	// Enable pull-up.
	TCCR0B = 0x0;		// Sets normal mode.
	TCCR0B = 0b00000101;// Sets 1024 pre-scaler.
	while (1)
	{
		if (PINC & 0x04)	// If switch is not pressed.
		PORTB = 0b11111111;	// Turns of LED.
		else				// If switch is pressed.
		{
			PORTB = 0b11111011;	// Turns LED on.
			TCNT0 = 0;			// Resets timer.
			for (O_F = 0; O_F < 5630; O_F++) // For loop used to create delay.
			{
				while (TCNT0 <= 254){}; // Loop used to keep track of overflows.
			}
		}
	}
	return 0;
}