 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
 ;     这是一个简单的引导区程序     ;
 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


	org	07c00h			;告诉编译器这个程序要装载到07c00内存地址
	mov	ax, cs
	mov	ds, ax
	mov	es, ax
	call	SayHelloToOS
	jmp	$			; jump forever

SayHelloToOS:
	mov	ax, HelloMessage
	mov	bp, ax
	mov	cx, 12
	mov	ax, 01301h
	mov	bx, 000ch
	mov	dl, 0
	int	10h
	ret

HelloMessage:
	db	"Hello LieOS!"        ;显示信息
	times	510-($-$$)  db  0
	dw	0xaa55
