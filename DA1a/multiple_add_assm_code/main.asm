; multiple_add_assm_code.asm
; Daniel Senda

.org 0x000

start:
ldi r25, 0b100
ldi r26, 0b100
mov r24, r25
dec r26

loop: add r24, r25
dec r26
brne loop

end:	jmp end
