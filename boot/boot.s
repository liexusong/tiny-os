;----------------------------
;
;  boot.s -- kernel startup
;
;----------------------------

; Multiboot magic number

MBOOT_HEADER_MAGIC equ 0x1BADB002
MBOOT_PAGE_ALIGN   equ 1 << 0
MBOOT_MEM_INFO     equ 1 << 1
MBOOT_HEADER_FLAGS equ MBOOT_PAGE_ALIGN|MBOOT_MEM_INFO
MBOOT_CHECKSUM     equ -(MBOOT_HEADER_MAGIC+MBOOT_HEADER_FLAGS)

; Multiboot header map
;-------------------------------------
; offset   type   domian    desc
;
;  0        u32    magic     yes
;  4        u32    flags     yes
;  8        u32    checksum  yes
;-------------------------------------


; ----------- start here -------------
[BITS 32]
section .init.text ; code segment start

dd MBOOT_HEADER_MAGIC
dd MBOOT_HEADER_FLAGS
dd MBOOT_CHECKSUM

[GLOBAL start]
[GLOBAL global_mboot_tmp]
[EXTERN kernel_start]

start:
	cli

	mov esp, STACK_TOP
	mov ebp, 0
	and esp, 0FFFFFFF0H
	mov [global_mboot_tmp], ebx
	call kernel_start

stop:
	hlt
	jmp stop

section .init.data
stack: times 1024 db 0

STACK_TOP equ $-stack-1

global_mboot_tmp: dd 0
