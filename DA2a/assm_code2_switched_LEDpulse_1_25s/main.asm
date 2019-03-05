; assm_code2_switched_LEDpulse_1_25s.asm
; Daniel Senda
; Turn on LED for 1.25s after pushbutton is pressed.

.org 0				;Sets origin of code.
LDI R16, 0b00000100	;Represents pin 2 of port b (PB2).
LDI R21, 0b00000000 ;Represents pin 2 of port c (PC2).
OUT DDRB,R16		;Enables PB2 as output.
OUT DDRC, R21		;Enables PC2 as input.
LDI R17, 0b00000100	;Mask used to toggle PB2.
LDI R20,5			;Sets clock prescaler to 1024.
STS TCCR1B,R20		;Sets clock division factor of 32.
OUT PORTB, R17		;Resets LED, Turns it off.

main:					;Main loop.
	IN R22, PINC		;Inputs values of PINB onto R22.
	ANDI R22, 0b100		;Mask, used to isolate the value of PC2.
	CPI R22, 0b100		;Compares R22 with 0b100
	BREQ main			;Branch to top of main if not equal.
	EOR R17,R16			;If equal, XOR to toggle LED on.
	OUT PORTB,R17		;Outputs value on LED.
	RCALL timer_reset	;Calls timer_reset subroutine.
	RCALL delay1		;Calls delay1 subroutine.
	RJMP main			;Jumps back to top of main.

timer_reset:		;This subroutnie resets the timer.
	LDI R20,0x00	;Sets the counter to 0.
	STS TCNT1H,R20	;Stores 0 in high byte.
	STS TCNT1L,R20	;Stores 0 in low byte.
	RET				;Returns to previous position in main.

delay1:				;Creates a delay of 1.25s (Ton value).
	LDS R29,TCNT1H	;Loads upper byte of counter to R29.
	LDS R28,TCNT1L	;Loads lower byte of counter to R28.
	CPI R28,0x4A	;Compares if lower byte of timer is 0x4A.
	BRSH delay1a	;Branch to delay1a if lower byte has reached desired amount.
	RJMP delay1		;If not equal, jump back to top of delay1.
delay1a:			;Subroutnie to check upper byte of timer.
	CPI R29,0x4C	;Compares if upper byte of timer is 0x4C.
	BRLT delay1		;If not equal, jump back to delay1.
	EOR R17,R16		;If equal, XOR to toggle LED off.
	OUT PORTB,R17	;Outputs value on LED.
	RET				;Returns to previous position in main.