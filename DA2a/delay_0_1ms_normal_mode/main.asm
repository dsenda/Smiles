; delay_0_1ms_normal_mode.asm
; Daniel Senda

delay_0_1ms:
	push r16 ;Pushes the value of r16 to stack.
	ldi r16,100 ;Loads r16 with 100 (counter value).
	delay_1a:
		nop ;No operation.
		nop ;No operation.
		nop ;No operation.
		nop ;No operation.
		nop ;No operation.
		nop ;No operation.
		nop ;No operation.
		nop ;No operation.
		nop ;No operation.
		nop ;No operation.
		nop ;No operation.
		nop ;No operation.
		nop ;No operation.
		dec r16 ;Decrements the counter.
		brne delay_1a ;Branches to delay_1a if previous not zero.
		pop r16 ;Retrives top value from stack and re-stores in r16.
		ret ;Returns to main function.