; dotp.asm - floating-point vector dot product

; p = vector 1	[XAR4]
; q = vector 2  [XAR5]
; N = vector length [AL] - N must be even
; return [R0H]

   		.def 	_dotp					; define C callable label

		.text
_dotp:
		.asmfunc
		; context save
		ASP
		PUSH  	XAR7
		MOV32   *SP++, R2H
		MOV32   *SP++, R3H
		MOV32   *SP++, R6H
		MOV32   *SP++, R7H

		; init
		MOVL	XAR7, XAR5
		SUBB	AL, #1
		ZERO 	R2H
		ZERO 	R3H
		ZERO 	R6H
		ZERO 	R7H

		; dot product
		RPT 	AL
|| 		MACF32 	R7H, R3H, *XAR4++, *XAR7++
		ADDF32 	R7H, R7H, R2H
		NOP
		ADDF32 	R7H, R7H, R3H
		NOP
		ADDF32 	R7H, R7H, R6H
		NOP
		MOV32	R0H, R7H

		; context restore
		MOV32   R7H, *--SP, UNCF
		MOV32   R6H, *--SP, UNCF
		MOV32   R3H, *--SP, UNCF
	    MOV32   R2H, *--SP, UNCF
		POP   	XAR7
		NASP
		LRETR
		.endasmfunc

		.end

; end of file
