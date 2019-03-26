 // 3c_code_2_timer0_interrupts_ctc_mode.c
 // Daniel Senda
 // Turn on LED for 1.25s after pushbutton is pressed.

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
	 DDRC &= (0 << 2);		// Set PORTC as input
	 PORTC |= (1 << 2);		// Enable pull-up.
	 initiate_timer();		// Function that initiates timer.
	 while (1)				// While loop that generates wave.
	 {
		 
		 if (PINC & 0x04)		// If switch is not pressed.
		 PORTB = 0b11111111;	// Turns LED off.
		 else					// If switch is pressed.
		 {
			 PORTB = LED1;		// Turns LED connected to PB2 on.
			 num = 0;			// Resets num value.
			 TCNT0 = 0;			// Resets timer.
			 while(num <= 0)	// For loop used to create delay.
			 {
				 if (O_F >= 76)	// If overflow count equals 76:
				 {
					 num++;		// Increment num to exit loop.
					 O_F = 0;	// Reset overflow count.
				 }
			 }
		 }
	 }
 }