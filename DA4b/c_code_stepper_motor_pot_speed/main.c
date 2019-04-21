// c_code_stepper_motor_pot_speed.c
// Daniel Senda
// This codes rotates the stepper motor clockwise.
// The speed of the motor is determined by a potentiometer.
// Extra code at bottom.

#include	<avr/io.h>

// Declares global variables.
int speed = 0;		// Speed variable for stepper motor.
int poten = 0;		// Potentiometer variable.

// Declares function.
void adc_init(void);	// Initializes the ADC.
void speed_calc(void);	// Calculates the speed of stepper motor.

int main(void)
{
	DDRB = 0b00111100;	// Sets PB2-PB5 as outputs.
	DDRC = 0b00000000;	// Sets all pins in PORTC as inputs.
	TCCR1A = 0x00;		// Sets normal mode on timer1.
	TCCR1B = 0b011;		// Sets pre-scaler of 64.
	adc_init();			// Initializes ADC.
	while (1)
	{
		PORTB = 0b101000;		// Sequence that turns the stepper
		TCNT1 = 0;				// motor in the clockwise direction.
		while(TCNT1 < speed);
		PORTB = 0b011000;
		TCNT1 = 0;				// Resets timer1.
		while(TCNT1 < speed);	// Waits for timer to reach speed time.
		speed_calc();			// Recalculates speed halfway through.
		PORTB = 0b010100;
		TCNT1 = 0;
		while(TCNT1 < speed);
		PORTB = 0b100100;
		TCNT1 = 0;
		while(TCNT1 < speed);
		speed_calc();			// Recalculates speed at end of cycle.
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

void speed_calc(void)
{
	ADCSRA|=(1<<ADSC);				// Starts conversion.
	while((ADCSRA&(1<<ADIF))==0);	// Waits for conversion to finish.
	ADCSRA |= (1<<ADIF);			// Resets conversion finished flag.
	poten = ADCL;					// Records potentiometer data.
	poten = poten | (ADCH<<8);		// Data calculations.
	poten = (poten/1024.0) * 5000/10;
	if (poten >= 255)
	{
		speed = 1249;	// 5ms
	}
	else if (poten >= 200)
	{
		speed = 2499;	// 10ms
	}
	else if (poten >= 150)
	{
		speed = 9999;	// 40ms
	}
	else if (poten >= 100)
	{
		speed = 17499;	// 70ms
	}
	else if (poten >= 50)
	{
		speed = 24999;	// 100ms
	}
	else if (poten >= 0)
	{
		speed = 32499;	// 130ms
	}
}





// This codes turns the stepper motor clockwise. If the switch is pressed,
// this code will make the stepper motor turn in the counter clockwise direction.
/*#define F_CPU 16000000	// CLock speed of micro-controller used.
#include <avr/io.h>
#include <util/delay.h>

const int stepper_delay = 10;	// Constant that determines speed of rotation.

int main(void)
{
	DDRC = 0b00000000; // PD1(1st Switch) set as input.
	DDRB = 0b00111100; // Sets PB2-PB5 as outputs.
	while (1)
	{
		if ((PINC & 0b00000010) == 0b00000010)	// Clockwise Rotation
		{
			PORTB = 0b101000;			// Sequence that turns the stepper
			_delay_ms(stepper_delay);	// motor in the clockwise direction.
			PORTB = 0b011000;
			_delay_ms(stepper_delay);
			PORTB = 0b010100;
			_delay_ms(stepper_delay);
			PORTB = 0b100100;
			_delay_ms(stepper_delay);
		}
		else if ((PINC & 0b00000010) == 0b00000000)	// Counter Clockwise Rotation.
		{
			PORTB = 0b100100;			// Sequence that turns the stepper
			_delay_ms(stepper_delay);	// motor in the CCW direction.
			PORTB = 0b010100;
			_delay_ms(stepper_delay);
			PORTB = 0b011000;
			_delay_ms(stepper_delay);
			PORTB = 0b101000;
			_delay_ms(stepper_delay);
		}
		
	}
	
}*/