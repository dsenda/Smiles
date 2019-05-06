// c_code_MPU6050_gyro_accel.c
// Daniel Senda
// Makes use of the MPU6050 (Gyroscope, Accelerometer, Temperature) Sensor.

#define F_CPU 16000000UL	// Defines CPU clock frequency.

// Included libraries:
#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include "inc\MPU6050_Res_DefineFile.h"
#include "inc\I2C_MasterFileH.h"
#include "inc\USART_RS232File_H.h"

// Variables:
char buffer[20], float_[10];
float Accel_x,Accel_y,Accel_z;
float Gyro_x,Gyro_y,Gyro_z;

// Functions:
void MPU6050_Init(void);
void values_in(void);
void MPU_Start_Loc(void);
void value_calcs(void);
void values2usart(void);

int main()
{
	I2C_Init();				// Calls the I2C_Init function.
	MPU6050_Init();			// Initializes MPU6050 sensor.
	USART_Init(9600);		// Initializes USART.
	
	while(1)
	{
		values_in();		// Reads values in from sensor.
		value_calcs();		// Calculates sensor values.
		values2usart();		// Sends calculated values to USART.
		_delay_ms(1000);
	}
}

void MPU6050_Init()			// Gyro initialization function.
{
	_delay_ms(150);			// Power up time >100ms.
	I2C_Start_Wait(0xD0);	// Starts at device that will be written to address.
	I2C_Write(SMPLRT_DIV);	// Writes to sample rate register.
	I2C_Write(0x07);		// Sets 1KHz sample rate.
	I2C_Stop();
	I2C_Start_Wait(0xD0);
	I2C_Write(PWR_MGMT_1);	// Writes to power management register.
	I2C_Write(0x01);		// X axis gyroscope reference frequency.
	I2C_Stop();
	I2C_Start_Wait(0xD0);
	I2C_Write(CONFIG);		// Writes to configuration register.
	I2C_Write(0x00);		// Fs = 8KHz.
	I2C_Stop();
	I2C_Start_Wait(0xD0);
	I2C_Write(GYRO_CONFIG);	// Writes to Gyroscope configuration register.
	I2C_Write(0x18);		// Full scale range +/- 2000 degree/C
	I2C_Stop();
	I2C_Start_Wait(0xD0);
	I2C_Write(INT_ENABLE);	// Writes to interrupt enable register.
	I2C_Write(0x01);
	I2C_Stop();
}

void values_in()			// Reads values from sensor.
{
	MPU_Start_Loc();
	Accel_x = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Accel_y = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Accel_z = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Gyro_x = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Gyro_y = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Gyro_z = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Nack());
	I2C_Stop();
}

void MPU_Start_Loc()
{
	I2C_Start_Wait(0xD0);		// I2C starts with device write address.
	I2C_Write(ACCEL_XOUT_H);	// Writes start location address from where to read.
	I2C_Repeated_Start(0xD1);	// I2C starts with device read address.
}

void value_calcs()
{
	// Calculates accelerometer values into G force units.
	Accel_x = Accel_x/16384.0;
	Accel_y = Accel_y/16384.0;
	Accel_z = Accel_z/16384.0;
	
	// Calculates gyroscope values into degrees/second units.
	Gyro_x = Gyro_x/16.4;
	Gyro_y = Gyro_y/16.4;
	Gyro_z = Gyro_z/16.4;
}

void values2usart()
{
	// Sends values to USART.
	dtostrf( Accel_x, 3, 2, float_ );
	sprintf(buffer," Accel_x = %s g\t",float_);
	USART_SendString(buffer);

	dtostrf( Accel_y, 3, 2, float_ );
	sprintf(buffer," Accel_y = %s g\t",float_);
	USART_SendString(buffer);
	
	dtostrf( Accel_z, 3, 2, float_ );
	sprintf(buffer," Accel_z = %s g\t",float_);
	USART_SendString(buffer);

	dtostrf( Gyro_x, 3, 2, float_ );
	sprintf(buffer," Gyro_x = %s%c/s\t",float_,0xF8);
	USART_SendString(buffer);

	dtostrf( Gyro_y, 3, 2, float_ );
	sprintf(buffer," Gyro_y = %s%c/s\t",float_,0xF8);
	USART_SendString(buffer);
	
	dtostrf( Gyro_z, 3, 2, float_ );
	sprintf(buffer," Gyro_z = %s%c/s\r\n",float_,0xF8);
	USART_SendString(buffer);
}