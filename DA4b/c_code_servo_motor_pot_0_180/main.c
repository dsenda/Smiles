// c_code_servo_motor_pot_0_180.c
// Daniel Senda
// This codes rotates the servo motor to 180 degrees when the
// potentiometer is at high value, and rotates to 0 degrees when
// the potentiometer is at low value.

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>

// Declares global variables.
int poten = 0;		// Potentiometer variable.

// Declares function.
void adc_init(void);	// Initializes the ADC.
void poten_calc(void);	// Calculates the value of potentiometer.

int main(void)
{
	DDRC = 0b00000000;		// Sets all pins in PORTC as inputs. (Poten. Input)
	DDRD = 0b01100000;		// Sets PB5 and PB6 as outputs. (PWM output)
	TCCR0A = 0b10000011;	// Sets Compare1 on non-inverting and Fast PWM.
	TCCR0B = 0b101;			// Sets pre-scaler of 1024.
	adc_init();				// Initializes ADC.
	while (1)
	{
		poten_calc();		// Recalculates the value of potentiometer.
		// If statements that determine position of servo motor based on pot. value.
		if (poten >= 255)	// If pot. value high do this:
		{
			OCR0A = 34;		// Sets pulse width for 180 degree position.
		}
		else if (poten <= 200)	// If pot. value low do this:
		{
			OCR0A = 8;		// Sets pulse width for 0 degree position.
		}
	}
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

void poten_calc(void)
{
	ADCSRA|=(1<<ADSC);				// Starts conversion.
	while((ADCSRA&(1<<ADIF))==0);	// Waits for conversion to finish.
	ADCSRA |= (1<<ADIF);			// Resets conversion finished flag.
	poten = ADCL;					// Records potentiometer data.
	poten = poten | (ADCH<<8);		// Data calculations.
	poten = (poten/1024.0) * 5000/10;
}