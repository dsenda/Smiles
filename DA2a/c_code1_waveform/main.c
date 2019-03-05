// c_code1_waveform.c
// Daniel Senda
// Square waveform with period of 0.725s and 60% duty cycle.

#define		F_CPU	16000000
#include	<avr/io.h>
#include	<util/delay.h>
int num = 1;				// Used for while loop.

int main(void)
{
   DDRB = 0b100;		// Enables PB2 as output.
   while(num = 1)			// Loop used to create waveform.
   {
	   PORTB = 0xFB;		// Sets PB2 to 0.
	   _delay_ms(435);		// Turns LED on for 435ms (Ton).
	   PORTB = PORTB^0b100;	// Sets PB2 to 1.
	   _delay_ms(290);		// Turns LED off for 290ms (Toff).
   }
   return 0;
}

