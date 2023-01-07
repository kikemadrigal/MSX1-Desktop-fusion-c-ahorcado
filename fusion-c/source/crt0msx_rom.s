	;; Generic crt0.s for a Z80
	.module crt0

	.globl	_main
	.globl	l__INITIALIZED
	.globl	l__INITIALIZER
	.globl	s__INITIALIZED
	.globl	s__INITIALIZER

;; ROM Header
	.area _HEADER (ABS)
        .org    0x4000

        .db     0x41
        .db     0x42 ; "AB" HEADER
        .dw     init
        .dw     0x0000
        .dw     0x0000
        .dw     0x0000
        .dw     0x0000
        .dw     0x0000
        .dw     0x0000

init:
	;; Stack at the top of memory.
        ld      sp,(0xfc4a)

        ;; Initialise global variables
        call    gsinit
	call	_main
done::
	jp	done

	;; Ordering of segments for the linker.
	.area	_HOME
	.area	_CODE
        .area   _GSINIT
        .area   _GSFINAL
	.area	_INITIALIZER

	.area	_DATA
	.area	_INITIALISED
	.area	_BSEG
     .area   _BSS
;`	.area	_HEAP
;`	.area	_STACK


	.area	_STACK
	stacksize = 64
	.ds	stacksize
stack:

        .area   _CODE

        ;; Special RLE decoder used for initing global data
;;__initrleblock::
;;        ret

        .area   _GSINIT

gsinit::
	ld	bc, #l__INITIALIZED
	ld	a, b
	or	a, c
	jr	z, gsinit_next
	ld	de, #s__INITIALIZED
	ld	hl, #s__INITIALIZER
	ldir
gsinit_next:

        .area   _GSFINAL
        ret

