; DA1a.asm
; Daniel Senda

.org 0x000

start:
ldi r21, 0x0 ;Set register r23 to zero, "Zero register".
ldi r18, 0x0 ;Set register r24 to zero. "Low byte of final answer".
ldi r19, 0x0 ;Set register r24 to zero. "Mid byte of final answer".
ldi r20, 0x0 ;Set register r24 to zero.	"High byte of final answer".
ldi r23, 0x0 ;Set register r23 to zero. "Overflow sum of multiplicand."
ldi r24, 0b00100011 ;Load low byte of multiplicand.
ldi r25, 0b00000000 ;Load high byte of multiplicand.
ldi r22, 0b00010111 ; Load 8-bit multiplier.

main_loop:
lsr r22 ;Logical shift right of multiplier.
brcs lsb_multiplier ;Branch to lsb_multiplier if carry is set.
add r22, r21 ;add "zero register" and multiplier.
breq end ;If last result is zero, branch to end.
lsl r24 ;Logical shift left low byte of multiplicand.
rol r25 ;Rotate left with carry high byte of multiplicand.
rol r23 ;Rotate left with carry overflow sum of multiplicand.
jmp main_loop ;Jump to main loop.

lsb_multiplier:
add r18, r24 ;Add low byte of multiplicand with low byte of answer register.
adc r19, r25 ;Add with carry high byte of multiplicand with mid byte of answer register.
adc r20, r23 ;Add overflow sum of multiplicand with the high byte of answer register.
lsl r24 ;Logical shift left low byte of multiplicand.
rol r25 ;Rotate left with carry high byte of multiplicand.
rol r23 ;Rotate left with carry overflow sum of multiplicand.
jmp main_loop ;Jump to main loop.

end:	jmp end ;End loop, signifying that it finished.