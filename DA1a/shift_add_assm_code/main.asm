; shift_add_assm_code.asm
; Daniel Senda

.org 0x000

start:
ldi r23, 0b0 ;Set register r23 to zero, "zero register".
ldi r24, 0b0 ;Set register r24 to zero. "Answer register at end of code."
ldi r25, 0b100 ;Load multiplicand.
ldi r26, 0b101 ; Load multiplier.

loop:
lsr r26 ;Logical shift right of multiplier.
brcs lsb_multiplier ;If carry is set, branch to lsb_multiplier.
add r26, r23 ;add multiplier and zero.
breq end ;If last result is zero, branch to end.
lsl r25 ;Logical shift left of multilplicand.
jmp loop ;Jump to loop.

lsb_multiplier:
add r24, r25 ;Add multiplicand and sum stored in r24.
lsl r25 ;Logical shift left of multilplicand.
jmp loop ;Jump to loop

end:	jmp end ;End loop.