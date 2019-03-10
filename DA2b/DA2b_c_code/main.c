// DA2b_c_code.c
// Daniel Senda
// Turn LED on for 1.25s after GPIO external pin interrupt is set.

#define		F_CPU	8000000
#include	<util/delay.h>
#include	<avr/io.h>
#include	<avr/interrupt.h>

int main ()
{
	DDRB = 0xFF;		//Sets port B pins as outputs.
	PORTB = 0xFF;		//Turns all LEDs off.
	PORTD = 1<<2;		//Pull-up activated.
	EICRA = 0x02;		//Sets the EICRA(External Interrupt Control Register A) to falling edge of INT0.
	EIMSK = (1<<INT0);	//Enables external interrupt 0 INT0.
	sei ();				//(Enables interrupts)Sets the Global Interrupt flag I in SREG.
	while (1);			//Waits for interrupt here (Wait loop).
}
ISR (INT0_vect)	//ISR for external interrupt 0.
{
	PORTB = 0xFB;	//Turns LED connected to PB2 on.
	_delay_ms(1250);	// Turns LED on for 1.25s (Ton).
	PORTB = PORTB^0b100;// Turns LED back off.
}