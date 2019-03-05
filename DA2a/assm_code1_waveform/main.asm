; assm_code1_waveform.asm
; Daniel Senda
; Square waveform with period of 0.725s and 60% duty cycle.

.org 0				;Sets origin of code.
LDI R16, 0b00000100	;Represents pin 2 of port b (PB2).
OUT DDRB,R16		;Enables PB2 as output.
LDI R17, 0b00000100	;Mask used to toggle PB2.
LDI R20,5			;Sets clock prescaler to 1024.
STS TCCR1B,R20		;Sets clock division factor of 32.

main:					;Main loop of waveform.
	RCALL delay1		;Calls delay1 subroutine.
	EOR R17,R16			;XOR to toggle LED.
	OUT PORTB,R17		;Outputs value on LED.
	RCALL timer_reset	;Calls timer_reset subroutine.
	RCALL delay2		;Calls delay2 subroutine.
	EOR R17,R16			;XOR to toggle LED.
	OUT PORTB,R17		;Outputs value on LED.
	RCALL timer_reset	;Calls timer_reset subroutine.
	RJMP main			;Jump to top of main and repeat.

timer_reset:		;This subroutnie resets the timer.
	LDI R20,0x00	;Sets the counter to 0.
	STS TCNT1H,R20	;Stores 0 in high byte.
	STS TCNT1L,R20	;Stores 0 in low byte.
	RET				;Returns to main.

delay1:				;Creates a delay of 435ms (Ton value).
	LDS R29,TCNT1H	;Loads upper byte of counter to R29.
	LDS R28,TCNT1L	;Loads lower byte of counter to R28.
	CPI R28,0x8C	;Compares if lower byte of timer is 0x8C.
	BRSH delay1a	;Branch to delay1a if lower byte has reached desired amount.
	RJMP delay1		;If not equal, jump back to top of delay1.
delay1a:			;Subroutnie to check upper byte of timer.
	CPI R29,0x1A	;Compares if upper byte of timer is 0x1A.
	BRLT delay1		;If not equal, jump back to delay1.
	RET				;If equal, return to main.

delay2:				;Creates a delay of 290ms (Toff value).
	LDS R29,TCNT1H	;Loads upper byte of counter to R29.
	LDS R28,TCNT1L	;Loads lower byte of counter to R28.
	CPI R28,0xB2	;Compares if lower byte of timer is 0xB2.
	BRSH delay2a	;If lower byte is equal to 0xB2, jump to delay2a.
	RJMP delay2		;If not equal, jump back to top of delay2.
delay2a:			;Subroutine to check upper byte of timer.
	CPI R29,0x11	;Compares if upper byte of timer is 0x11.
	BRLT delay2		;If not equal, jump back to delay2.
	RET				;If equal, return to main.