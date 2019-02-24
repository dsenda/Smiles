; part1_store99_numbers.asm
; Daniel Senda

.org 0x000			;Sets program origin to 0x000.

start:
ldi r16, 11			;Loads 11 into r16.
ldi r17, 99			;Loads 99 into r17.
ldi xh, 0x02		;Loads 0x02 into high byte of x register.
ldi xl, 0x00		;Loads 0x00 onto low byte of x register.

first_loop:
	st x+,r16		;Store value into address pointed by x register, then move pointer by 1.
	inc r16			;Increments value of r16.
	dec r17			;Decrements r17 to keep track of stored values.
	brne first_loop	;Branch to first_loop if r17 not zero.

end: rjmp end		;End loop.