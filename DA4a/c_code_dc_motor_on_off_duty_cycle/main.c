// c_code_dc_motor_on_off_duty_cycle.c
// Daniel Senda
// Turn DC motor on and off with pushbutton and
// also changes speed with change of potentiometer.

#define		F_CPU 16000000UL
#include	<avr/io.h>
#include	<util/delay.h>
#include	<avr/interrupt.h>

// Declares global variables.
int motor = 0;	// On_Off variable for motor.
int poten = 0;	// Potentiometer variable.

// Declares function.
void adc_init(void);

int main(void)
{
	DDRD = 0b00100000;	// Sets PD5 as an output.
	DDRC = 0b00000000;	// Sets all pins in PORTC as inputs.
	TCCR2A = 0x00;		// Sets normal mode on timer0.
	TCCR2B = 0x01;		// Sets no pre-scaling.
	TIMSK2 = 0x01;		// Enables timer0 overflow interrupt.
	sei ();				// Enables global interrupts.
	
	TCCR0A = 0b10100011;//Sets Fast PWM.
	TCCR0B = 0x01;		// Sets no pre-scaling.
	OCR0B = 0xCC;		// Sets 80% or 20% duty cycle.
	adc_init();			// Initializes ADC.
	
	while (1)
	{
		ADCSRA|=(1<<ADSC);				// Starts conversion.
		while((ADCSRA&(1<<ADIF))==0);	// Waits for conversion to finish.
		ADCSRA |= (1<<ADIF);			// Resets conversion finished flag.
		poten = ADCL;					// Records potentiometer data.
		poten = poten | (ADCH<<8);		// Data calculations.
		poten = (poten/1024.0) * 5000/10;
		OCR0B = poten;					// Sets OCR0B to poten value.
	}
}

ISR (TIMER2_OVF_vect)		// timer0 overflow interrupt vector
{
	if ((PINC & 0b10) == 0)	// If pushbutton is pressed.
	{	
		motor ^= 1;			// Toggles motor variable.
		if (motor == 1)		// Turns motor on.
		{
			TCCR0A = 0b00100011;
		}
		else				// Turns motor off.
		{
			TCCR0A = 0b00000011;
		}
		_delay_ms(500);		// Delay for de-bouncing.
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





// This codes turns DC motor on and off with switch 1 and
// changes duty cycle with switch 3.
/*int motor = 0;
int dutyc = 0;

int main(void)
{
	DDRD = 0b00100000;	// Sets PD5 as an output.
	DDRC = 0b00000000;	// Sets all pins in PORTC as inputs.
	TCCR2A = 0x00;		// Sets normal mode on timer0.
	TCCR2B = 0x01;		// Sets no pre-scaling.
	TIMSK2 = 0x01;		// Enables timer0 overflow interrupt.
	sei ();				// Enables global interrupts.
	
	TCCR0A = 0b10100011; //Sets Fast PWM.
	TCCR0B = 0x01;	// Sets no prescaling.
	OCR0B = 0xCC;	// Sets 80% or 20% duty cycle.
	
	while (1)
	{
		if ((PINC & 0b1000) == 0)
		{
			dutyc ^= 1;			// Toggles dutyc variable.
			if (dutyc == 1)		// Turns duty cycle to 80%.
			{
				OCR0B = 0xFF;
			}
			else				// Turns duty cycle to 20%.
			{
				OCR0B = 0x55;
			}
			_delay_ms(500);		// Delay for de-bouncing.
		}
	}
}

ISR (TIMER2_OVF_vect)		// timer0 overflow interrupt vector
{
	if ((PINC & 0b10) == 0)	// If pushbutton is pressed.
	{	
		motor ^= 1;			// Toggles motor variable.
		if (motor == 1)		// Turns motor on.
		{
			TCCR0A = 0b00100011;
		}
		else				// Turns motor off.
		{
			TCCR0A = 0b00000011;
		}
		_delay_ms(500);		// Delay for de-bouncing.
	}
}*/