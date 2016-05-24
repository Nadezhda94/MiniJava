section	.text
_malloc:
	;давайте считать, что количество байт записано в eax
	mov ebx, currentAddress
	mov ecx, currentAddress
	add ecx, eax
	mov eax, currentAddress
	mov [currentAddress], ecx

	mov ecx, currentAddress
	mov eax, 4
	mov ebx, 1
	mov edx, 8
	int 80h

	mov	eax, 1
	xor	ebx, ebx
	int	80h

section .data
startAddress	dd	0
currentAddress	dd	0
