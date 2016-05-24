%include "Malloc.asm"
global _start
section .data
mmap_arg:  ; ugly
  .addr:   dd 0
  .len:    dd 10485760
  .prot:   dd 3
  .flags:  dd 34
  .fd:     dd -1
  .offset: dd 0

section .text

mallocSample:
        ;old_mmap
        mov eax, 90
        lea ebx, [mmap_arg]
        int 80h

        ret

_start:
        ;malloc sample
        call mallocSample
        push eax
	mov eax, 4
	mov ebx, 1
	mov ecx, msg
	mov edx, len
	mov [startAddress], eax
	mov [currentAddress], eax
	int 80h
	call _malloc
	mov	eax, 1
	xor	ebx, ebx
	int	80h

section .data
msg   db      'address stored in eax', 10, 0
len	equ	$ - msg
