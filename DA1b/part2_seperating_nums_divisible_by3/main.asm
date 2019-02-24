; part2_seperating_nums_divisible_by3.asm
; Daniel Senda

.org 0x000		;Sets program origin to 0x000.

start:
ldi r16, 11		;Loads 11 into r16.
ldi r17, 99		;Loads 99 into r17.
ldi xh, 0x02	;Loads 0x02 into high byte of x register.
ldi xl, 0x00	;Loads 0x00 onto low byte of x register.
				;Sets the address pointer to 0x0200.
loop1:
	st x+,r16	;Store value into address pointed by x register, then move pointer by 1.
	inc r16		;Increments value of r16.
	dec r17		;Decrements r17 to keep track of stored values.
	brne loop1	;Branch to first_loop if r17 not zero.

ldi r17, 100	;Loads 100 into r17.
ldi yh, 0x04	;Loads 0x04 into high byte of y register.
ldi yl, 0x00	;Loads 0x00 into low byte of y register.
ldi zh, 0x06	;Loads 0x06 into high byte of z register.
ldi zl, 0x00	;Loads 0x00 into low byte of z register.

div_test:		;Loop that sorts out numbers that are divisible by 3.
	 ld r16, -X		;Loads value stored at the address indicated by register X into r16.
	 dec r17		;Decrements r17 to keep track of values sorted.
	 breq end		;Goes to end once all numbers are sorted.
	 loop2:
		subi r16, 3		;Subtracts 3 from value stored in r16
		cpi r16, 9		;Compares r16 with 9.
		breq div_by3	;Branches if r19 equals 9.
		cpi r16, 9		;Compares r16 with 9.
		brmi ndiv_by3	;Branches if r16 is less than 9.
		rjmp loop2		;Jumps back to loop2.

div_by3:
	ld r16, X		;Loads value stored at the address indicated by register X into r16.
	st y+, r16		;Store value into address pointed by y, then move pointer by 1.
	rjmp div_test	;Jumps back to div_test.
ndiv_by3:
	ld r16, X		;Loads value stored at the address indicated by register X into r16.
	st z+, r16		;Store value into address pointed by z, then move pointer by 1.
	rjmp div_test	;Jumps back to div_test.

end: rjmp end		;End loop.