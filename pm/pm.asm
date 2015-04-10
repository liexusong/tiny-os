
; +--------------------------+
; | Protect Mode for Tiny-OS |
; +--------------------------+

%include "define.asm"

org 07c00h

jmp LABEL_BEGIN  ; jump to begin flag to run

[SECTION .gdt]
; GDT entries:
; =================================  base,    limit,            type
LABEL_GDT:         DEFINE_GDT_ENTRY  0,       0,                0
LABEL_DESC_CODE32: DEFINE_GDT_ENTRY  0,       SegCode32Len-1,   DA_C+DA_32
LABEL_DESC_VIDEO:  DEFINE_GDT_ENTRY  0B8000H, 0FFFFH,           DA_DRW


GdtLen equ  $-LABEL_GDT
GdtPtr dw   GdtLen-1
dd 0

; GDT selectors
SelectorCode32 equ  LABEL_DESC_CODE32-LABEL_GDT
SelectorVideo  equ  LABEL_DESC_VIDEO-LABEL_GDT
; End of section

[SECTION .s16]
[BITS 16]
LABEL_BEGIN:
mov ax, cs
mov ds, ax
mov es, ax
mov ss, ax
mov sp, 0100H

; initialize 32 bits segment descriptor
; =====================================
xor eax, eax  ; by zero eax
mov ax, cs    ; point to cs
shl eax, 4    ; left shift 4 bits
add eax, LABEL_SEG_CODE32
mov word [LABEL_DESC_CODE32+2], ax
shr eax, 16   ; right shift 16 bits
mov byte [LABEL_DESC_CODE32+4], al
mov byte [LABEL_DESC_CODE32+7], ah


xor eax, eax
mov ax, ds
shl eax, 4
add eax, LABEL_GDT
mov dword [GdtPtr+2], eax

lgdt [GdtPtr]  ; load GDT

; close interrupt
cli

; open A20 address line
in al, 92H
or al, 00000010B
out 92H, al

; turn to protect mode
mov eax, cr0
or eax, 1
mov cr0, eax

jmp dword SelectorCode32:0 ; jump to code 32 segment


[SECTION .s32]
[BITS 32]

LABEL_SEG_CODE32:
mov ax, SelectorVideo
mov gs, ax

mov edi, (80*11+79)*2
mov ah, 0CH
mov al, 'p'
mov [gs:edi], ax

jmp $

SegCode32Len equ $-LABEL_SEG_CODE32


