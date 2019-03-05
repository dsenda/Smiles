// c_code2_switched_LEDpulse_1_25s.c
// Daniel Senda
// Turn on LED for 1.25s after pushbutton is pressed.

#define		F_CPU	16000000
#include	<avr/io.h>
#include	<util/delay.h>
int num = 1;				// Used for while loop.
int comp;					// Used to compare in loop.

int main(void)
{
	DDRB = 0b100;		// Enables PB2 as output.
	DDRC = 0x00;		// Enables PC2 as input.
	while(num = 1)		// Loop used to keep program running.
	{
		comp = PINB & 0b100;	// Bit masking.
		if (comp = 0b100)		// If button is pressed, this will be true.
		{
			PORTB = 0xFB;		// Sets PB2 to 0.
			_delay_ms(1250);	// Turns LED on for 1.25s (Ton).
			PORTB = PORTB^0b100;// Turns LED back off.
		}
	}
	return 0;
}