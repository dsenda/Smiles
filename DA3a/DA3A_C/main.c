// DA3a_C_code_string_serial2usb.c
// Daniel Senda
// Displays a string, random integer and floating point values on the serial terminal every 1s.
// Uses the FTDI chip for serial to USB conversion.

#define		F_CPU 16000000UL
#define		BAUDRATE 9600
#define		BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL)))-1)
#include	<avr/io.h>
#include	<util/delay.h>
#include	<avr/interrupt.h>
#include	<stdio.h>

// Defines functions used for this code.
volatile int Count;
void USART_init( unsigned int ubrr );
void USART_TX_string(char *data);
char outs[30];
int n;
float num_year = 0.001998;				// Declares the float and assigns a value.
char str[] = "Toyota Supra 2JZ-GTE";	// Declares a string.
char empty[] = " ";						// Declares empty string for space.

int main(void)
{
	Count = 0;							// Sets count to 0.
	TIMSK0 |= (1<<TOIE0);				// Sets interrupts when overflow flag is set.
	sei ();								// Sets the global interrupt.
	TCCR0A = 0x00;						// Sets normal mode for timer0.
	TCCR0B |= (1<<CS02)|(1<<CS00);		// Sets 1024 prescaler.
	USART_init(BAUD_PRESCALLER);		// Sets the BAUD prescaler.
	USART_TX_string("Connected!\r\n");	// Prints "Connected" to serial.
    while (1);							// Loop that waits for interrupt.
}

// INT USART (RS-232)
void USART_init( unsigned int ubrr ){
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << TXEN0);				// Enables Tx interrupt.
	UCSR0C = (3 << UCSZ00);				// Asynchronous 8 N 1.
}

// Sends a string to the RS-232
void USART_TX_string(char *data) {
	while (*data != '\0') {
		while (!(UCSR0A & (1<<UDRE0)));
		UDR0 = *data;
		data++;
	}
}

ISR (TIMER0_OVF_vect){				// TV0V flag interrupt.
	while (Count < 61){				// Loops while Count value is less than 76.
		if ((TIFR0 & 0x01) == 1)	// If TV0V flag is set, do the following.
		{
			TIFR0 = 0X01;			// Clears the flag.
			Count++;				// Increments the count.
		}
	}
	if (Count > 60){				// Executes the following if Count is less than 60.
		USART_TX_string(str);		// Prints the string "Toyota Supra 2JZ-GTE".
		USART_TX_string(empty);		// Prints an empty space.
				
		n = rand();					// Generates a random number and stores in n.
				
		snprintf(outs, sizeof(outs), "%3d\r\n", n);
		USART_TX_string(outs);
		USART_TX_string(empty);
		
		sprintf(outs, "%f", num_year);	// Outputs year.
		USART_TX_string(outs);
		USART_TX_string(empty);
		Count = 0;
	}
}