// Midterm1_c_code.c
// Daniel Senda
// This code reads temperature data from the sensor and sends this data to
// the Thingspeak server to create a temperature vs time graph.

#define		F_CPU 16000000UL		// Defines frequency of CPU.
#define		BAUD_RATE 9600			// Sets BAUD rate.
#include	<avr/io.h>				// Included libraries.
#include	<util/delay.h>
#include	<avr/interrupt.h>

volatile int O_F = 0;				// Used for overflow count.
int num = 0;						// Used for while loops.
int tempf = 0;						// Used for temperature data.
char outs[256];						// Used for sending USART commands.

void usart_init (void);				// Function declarations.
void initiate_timer(void);
void at_cmd_init (void);
void usart_send (char *ch);

int main (void)
{
	usart_init ();		// Initializes USART communication
						// Sets up and enables ADC.
	ADMUX = (0<<REFS1)|	// Reference Selection Bits.
	(1<<REFS0)|			// AVcc - external cap at AREF.
	(0<<ADLAR)|			// ADC Left Adjust Result.
	(1<<MUX2)|			// Analog Channel Selection Bits.
	(0<<MUX1)|			// ADC4 (PC5).
	(1<<MUX0);
	ADCSRA = (1<<ADEN)|	// ADC Enable.
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
		sei();							// Enables global interrupts.
		num = 0;						// Resets num count.
		O_F = 0;						// Resets overflow count.
		TCNT0 = 0;						// Resets timer.
		while (num <= 0)
		{
			if (O_F >= 61)
			{
				cli();						// Disables global interrupts.
				at_cmd_init ();				// Initializes AT commands.
				tempf = ADCL;				// Records temp sensor data.
				tempf = tempf | (ADCH<<8);
				tempf = (tempf/1024.0) * 5000/10;
				snprintf(outs,sizeof(outs),"GET https://api.thingspeak.com/update?api_key=QJIVD22Q8N49KTCH&field1=%3d\r\n", tempf);
				usart_send (outs);
				_delay_ms(5000);
				num++;						// Increments num to exit while loop.
			}
		}
	}
	return 0;
}

ISR(TIMER0_COMPA_vect)		// Keeps count of overflows.
{
	O_F++;					// Increments overflow count.
}

void initiate_timer(void)	// Initializes timer.
{
	TCCR0A = 0b10;			// Sets CTC mode.
	TCCR0B = 0b00000101;	// Sets 1024 pre-scaler.
	OCR0A = 255;			// Sets output compare register A to 255.
	TIMSK0 |= (1 << OCIE0A);// Enables output compare match interrupt.
	sei();					// Enables global interrupts.
	TCNT0 = 0;				// Resets timer.
}

void at_cmd_init (void)		// Initializes AT commands.
{
	char AT[] = "AT\r\n";									// Checks AT connect.
	char AT_CWMODE[] = "AT+CWMODE=1\r\n";					// Sets device mode, 1 = Station mode.
	char AT_CWJAP[] = "AT+CWJAP=\"SSD\",\"Password\"\r\n";	// Connects to WiFi using SSID and Password.
	char AT_CIPMUX[] = "AT+CIPMUX=0\r\n";					// Sets device for single IP Address Mode.
	char AT_CIPSTART[] = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n"; // Starts TCP connection to Thingspeak.com at port 80.
	char AT_CIPSEND[] = "AT+CIPSEND=100\r\n";				// Gives upcoming string length.
	_delay_ms(200);											// Sends commands listed above
	usart_send(AT);
	_delay_ms(5000);
	usart_send(AT_CWMODE);
	_delay_ms(5000);
	usart_send(AT_CWJAP);
	_delay_ms(3000);
	usart_send(AT_CIPMUX);
	_delay_ms(3000);
	usart_send(AT_CIPSTART);
	_delay_ms(3000);
	usart_send(AT_CIPSEND);
	_delay_ms(5000);
}

void usart_init (void)		// Initializes USART.
{
	UCSR0B = (1<<TXEN0);
	UCSR0C = (1<< UCSZ01)|(1<<UCSZ00);
	UBRR0L = F_CPU/16/BAUD_RATE-1;
}

void usart_send (char *ch)	// Sends characters out.
{
	while ((*ch != '\0'))
	{
		while (!(UCSR0A & (1 <<UDRE0)));
		UDR0 = *ch;
		ch++;
	}
}