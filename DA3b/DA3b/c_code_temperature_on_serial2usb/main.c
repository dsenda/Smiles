// c_code_temperature_on_serial2usb.c
// Daniel Senda
// Monitors LM34 to display temperature on serial terminal every 1s.
// Uses a timer with interrupt for the 1s delay.

#define		F_CPU 16000000UL	// Needed Definitions.
#define		BAUD_RATE 9600
#include	<avr/io.h>			// Needed Libraries.
#include	<util/delay.h>
#include	<avr/interrupt.h>

volatile int O_F = 0;			// Used for overflow count.
int num = 0;					// Used for while loops.

void usart_init ();
void usart_send (unsigned char ch);
void initiate_timer()
{
	TCCR0A = 0b10;			// Sets CTC mode.
	TCCR0B = 0b00000101;	// Sets 1024 pre-scaler.
	OCR0A = 255;			// Sets output compare register A to 255.
	TIMSK0 |= (1 << OCIE0A);// Enables output compare match interrupt.
	sei();					// Enables global interrupts.
	TCNT0 = 0;				// Resets timer.
}
ISR(TIMER0_COMPA_vect)		// Keeps count of overflows.
{
	O_F++;					// Increments overflow count.
}

int main (void)
{
	usart_init ();
	// Setup and enable ADC
	ADMUX = (0<<REFS1)|	// Reference Selection Bits.
	(1<<REFS0)|			// AVcc - external cap at AREF.
	(0<<ADLAR)|			// ADC Left Adjust Result.
	(1<<MUX2)|			// Analog Channel Selection Bits.
	(0<<MUX1)|			// ADC4 (PC5).
	(1<<MUX0);
	ADCSRA = (1<<ADEN)|	// ADC ENable.
	(0<<ADSC)|			// ADC Start Conversion.
	(0<<ADATE)|			// ADC Auto Trigger Enable.
	(0<<ADIF)|			// ADC Interrupt Flag.
	(0<<ADIE)|			// ADC Interrupt Enable.
	(1<<ADPS2)|			// ADC Pre-scaler Select Bits.
	(0<<ADPS1)|
	(1<<ADPS0);
	initiate_timer();	// Function that initiates timer.
	while (1)
	{
		ADCSRA|=(1<<ADSC);				// Starts conversion.
		while((ADCSRA&(1<<ADIF))==0);	// Waits for conversion to finish.
		ADCSRA |= (1<<ADIF);
		num = 0;						// Resets num count.
		TCNT0 = 0;						// Resets timer.
		while (num <= 0)
		{
			if (O_F >= 61)
			{
				int a = ADCL;
				a = a | (ADCH<<8);
				a = (a/1024.0) * 5000/10;
				usart_send((a/100)+'0');
				a = a % 100;
				usart_send((a/10)+'0');
				a = a % 10;
				usart_send((a)+'0');
				usart_send('\r');
				num++;					// Increments num to exit while loop.
				O_F = 0;				// Resets overflow count.
			}
		}
	}
	return 0;
}

void usart_init (void)					// Initializes USART.
{
	UCSR0B = (1<<TXEN0);
	UCSR0C = (1<< UCSZ01)|(1<<UCSZ00);
	UBRR0L = F_CPU/16/BAUD_RATE-1;
}

void usart_send (unsigned char ch)		// Transmits characters to computer.
{
	while (! (UCSR0A & (1<<UDRE0)));	// Waits until UDR0 is empty.
	UDR0 = ch;							// Transmit ch.
}

void usart_print(char* str)				// Prints characters on computer.
{
	int i = 0;
	while(str[i] != 0)
	{
		usart_send(str[i]);
	}
}