// c_code_RF_temperature_serial2usb.c
// Daniel Senda
// Interface the provided NRF24L01+ RF Module to the ATmega328p using the SPI interface.
// Using the earlier developed code for ADC, transmit the ADC value of the internal
// temp sensor, or LM35 sensor between two RF Modules. The ATmega328p interfacing the
// RF Modules should alternate between TX and RX modes every 0.5 secs (hopefully they are
// not both at TX and RX modes in the same interval). The temp of both ATmega328p’s
// should be displayed on both ATmega328p’s.

#ifndef F_CPU				// Sets clock frequency.
#define F_CPU 16000000UL
#endif

#include <avr/io.h>			// Includes needed libraries.
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "inc\nrf24l01.c"	//	Include nRF24L01+ library.
#include "inc\nrf24l01-mnemonics.h"
#include "inc\spi.c"

#ifndef BAUD				// Sets up UART for printf();
#define BAUD 9600
#endif
#include "inc\STDIO_UART.c"

void print_config(void);	// Functions
void adc_init(void);

volatile bool message_received = false;	// Used in IRQ ISR.
volatile bool status = false;			// Used in IRQ ISR.
int tempf = 0;							// Used for temperature data.

int main(void)
{
	adc_init();							// Initializes the ADC.
	char tx_message[32];				// Defines string array.
	strcpy(tx_message,"Hello World!");	// Copies string into array.
	uart_init();						// Initializes UART.
	nrf24_init();						// Initializes nRF24L01+ and print configuration info.
	print_config();						// Configures prints.
	nrf24_start_listening();			// Start listening to incoming messages.
	nrf24_send_message(tx_message);		// Sends message.
	while (1)
	{
		ADCSRA|=(1<<ADSC);						// Starts conversion.
		while((ADCSRA&(1<<ADIF))==0);			// Waits for conversion to finish.
		ADCSRA |= (1<<ADIF);					// Resets flag for conversion.
		tempf = ADCL;							// Records temp sensor data.
		tempf = tempf | (ADCH<<8);
		tempf = (tempf/1024.0) * 5000/10;
		char temp[5];							// Variable used to store tempf string.
		itoa(tempf, temp, 10);					// Converts tempf integer to string.
		
		if (message_received)
		{
			message_received = false;			//	Message received, print it.
			printf("Received message: %s\n",nrf24_read_message());
			_delay_ms(500);
			status = nrf24_send_message(temp);	// Send message as response.
			if (status == true) printf("Message sent successfully\n");
		}
	}
}

ISR(INT0_vect)	// Interrupt on IRQ pin.
{
	message_received = true;
}

void print_config(void)
{
	uint8_t data;
	printf("Startup successful\n\n nRF24L01+ configured as:\n");
	printf("-------------------------------------------\n");
	nrf24_read(CONFIG,&data,1);
	printf("CONFIG		0x%x\n",data);
	nrf24_read(EN_AA,&data,1);
	printf("EN_AA			0x%x\n",data);
	nrf24_read(EN_RXADDR,&data,1);
	printf("EN_RXADDR		0x%x\n",data);
	nrf24_read(SETUP_RETR,&data,1);
	printf("SETUP_RETR		0x%x\n",data);
	nrf24_read(RF_CH,&data,1);
	printf("RF_CH			0x%x\n",data);
	nrf24_read(RF_SETUP,&data,1);
	printf("RF_SETUP		0x%x\n",data);
	nrf24_read(STATUS,&data,1);
	printf("STATUS		0x%x\n",data);
	nrf24_read(FEATURE,&data,1);
	printf("FEATURE		0x%x\n",data);
	printf("-------------------------------------------\n\n");
}

void adc_init (void)	// Sets up and enables ADC.
{
	ADMUX = (0<<REFS1)|	// Reference Selection Bits.
	(1<<REFS0)|			// AVcc - external cap at AREF.
	(0<<ADLAR)|			// ADC Left Adjust Result.
	(0<<MUX2)|			// Analog Channel Selection Bits.
	(0<<MUX1)|			// ADC0 (PC0).
	(0<<MUX0);
	ADCSRA = (1<<ADEN)|	// ADC Enable.
	(0<<ADSC)|			// ADC Start Conversion.
	(0<<ADATE)|			// ADC Auto Trigger Enable.
	(0<<ADIF)|			// ADC Interrupt Flag.
	(0<<ADIE)|			// ADC Interrupt Enable.
	(1<<ADPS2)|			// ADC Pre-scaler Select Bits.
	(0<<ADPS1)|
	(1<<ADPS0);
}