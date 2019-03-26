// 1c_code_1_timer0_norm_mode.c
// Daniel Senda
// Square waveform with period of 0.725s and 60% duty cycle.

#include	<avr/io.h>
int O_F = 0;			// Used for overflow count.

int main(void)
{
	DDRB = 0xff;			// Sets port b pins to outputs.
	int LED1 = 0b11111011;	// Output value used to turn on LED.
	TCCR0B = 0x0;			// Sets normal mode.
	TCCR0B = 0b00000101;	// Sets 1024 pre-scaler.
    while (1)				// While loop that generates wave.
	{
		PORTB = LED1;		// Turns LED connected to PB2 on.
		TCNT0 = 0;			// Resets timer.
		for (O_F = 0; O_F < 1320; O_F++) // For loop used to create delay.
		{
			while (TCNT0 <= 254){}; // Loop used to keep track of overflows.
		}
		
		PORTB = 255;		// Turns LED off.
		TCNT0 = 0;			// Resets timer.
		for (O_F = 0; O_F < 1920; O_F++)  // For loop used to create delay.
		{
			while (TCNT0 <= 254){};	// Loop used to keep track of overflows.
		}
    }
}