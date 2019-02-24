; DA1b.asm
; Daniel Senda

.org 0x000		;Sets program origin to 0x000.

start:
ldi r20, 11		;Loads 11 into r20.
ldi r21, 99		;Loads 99 into r21.
ldi xh, 0x02	;Loads 0x02 into high byte of x register.
ldi xl, 0x00	;Loads 0x00 onto low byte of x register.
				;Sets the address pointer to 0x0200.
loop1:
	st x+,r20	;Store value of r20 into address pointed by x register, then moves pointer by 1.
	inc r20		;Increments value of r20.
	dec r21		;Decrements r21 to keep track of stored values.
	brne loop1	;Branch to first_loop if r17 not zero.

ldi r21, 100	;Loads 100 into r21.
ldi yh, 0x04	;Loads 0x04 into high byte of y register.
ldi yl, 0x00	;Loads 0x00 into low byte of y register.
ldi zh, 0x06	;Loads 0x06 into high byte of z register.
ldi zl, 0x00	;Loads 0x00 into low byte of z register.
ldi r16, 0x00	;Set r16 to zero. "Low byte" of sum1 (numbers divisible by 3).
ldi r17, 0x00	;Set r17 to zero. "High byte" of sum1 (numbers divisible by 3).
ldi r18, 0x00	;Set r18 to zero. "Low byte" of sum2 (numbers not divisible by 3).
ldi r19, 0x00	;Set r19 to zero. "High byte" of sum2 (numbers not divisible by 3).
ldi r22, 0x00	;Set register r23 to zero, "Zero register".

div_test:		;Loop that sorts out numbers that are divisible by 3.
	 ld r20, -X		;Loads value stored at the address indicated by register X into r20.
	 dec r21		;Decrements r21 to keep track of values sorted.
	 breq end		;Goes to end once all numbers are sorted.
	 loop2:
		subi r20, 3		;Subtracts 3 from value stored in r20
		cpi r20, 9		;Compares r20 with 9.
		breq div_by3	;Branches if r20 equals 9.
		cpi r20, 9		;Compares r20 with 9.
		brmi ndiv_by3	;Branches if r20 is less than 9.
		rjmp loop2		;Jumps back to loop2.

div_by3:
	ld r20, X		;Loads value stored at the address indicated by register X into r20.
	st y+, r20		;Store value of r20 into address pointed by y, then moves pointer by 1.
	add r16, r20	;Add value in r20 with value in r16. Stores result in r16(low byte of sum1).
	adc r17, r22	;Add zero with carry to value in r17. Stores result in r17(high byte of sum1).
	rjmp div_test	;Jumps back to div_test.
ndiv_by3:
	ld r20, X		;Loads value stored at the address indicated by register X into r20.
	st z+, r20		;Store value of r20 into address pointed by z, then moves pointer by 1.
	add r18, r20	;Add value in r20 with value in r18. Stores result in r18(low byte of sum2).
	adc r19, r22	;Add zero with carry to value in r19. Stores result in r19(high byte of sum2).
	rjmp div_test	;Jumps back to div_test.

end: rjmp end		;End loop.