; DA2b.asm
; Daniel Senda
; Turn LED on for 1.25s after GPIO external pin interrupt is set.

JMP MAIN	;Jumps to main.
.ORG 0x02	;Location for external interrupt 0.
JMP EX0_ISR	;Jumps to EXO_ISR.

MAIN:
	LDI R20,HIGH(RAMEND);Loads the high byte of RAMEND(the address of the last memory location in SRAM) into R20.
	OUT SPH,R20			;Stores the high byte of RAMEND onto the Stack Pointer High register.
	LDI R20,LOW(RAMEND)	;Loads the low byte of RAMEND into R20.
	OUT SPL,R20			;(Initializes stack)Stores the low byte of RAMEND onto the Stack Pointer Low register.
	LDI R20,0x2			;Loads a value of 0b10 into R20.
	STS EICRA,R20		;Sets the EICRA(External Interrupt Control Register A) to falling edge of INT0.
	LDI R18,0xFF		;Loads 0xFF into R18, used to turn all LEDs off.
	OUT DDRB,R18		;Sets port B pins as outputs.
	LDI R19,0xFB		;Loads 0xFB into R19, used to turn LED on.
	OUT PORTB,R18		;Turns all LEDs off.
	SBI PORTD,2			;Pull-up activated.
	LDI R20,1<<INT0		;Enables external interrupt 0 INT0.
	OUT EIMSK,R20		;Stores the value R20 into EIMSK(External Interrupt Mask Register).
	SEI					;(Enables interrupts)Sets the Global Interrupt flag I in SREG.
HERE:JMP HERE			;Waits for interrupt here (Wait loop).

EX0_ISR:
	OUT PORTB,R19	;Turns on LED connected to PB2.
	ldi  R21, 51	;Loads 51 to R21.
    ldi  R22, 187	;Loads 187 to R22.
    ldi  R23, 224	;Loads 224 to R23.
Loop1:				;Delay of 1.250s.
	dec  R23		;Decrements R23.
    brne Loop1		;Branches if not equal.
    dec  R22		;Decrements R22.
    brne Loop1		;Branches if not equal.
    dec  R21		;Decrements R21.
    brne Loop1		;Branches if not equal.
	OUT PORTB,R18	;Loads the value of R21 onto PORTB.
	RETI			;Returns from the interrupt.