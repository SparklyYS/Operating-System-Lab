!
!	setup.s		(C) 1991 Linus Torvalds
!
! setup.s is responsible for getting the system data from the BIOS,
! and putting them into the appropriate places in system memory.
! both setup.s and system has been loaded by the bootblock.
!
! This code asks the bios for memory/disk/other parameters, and
! puts them in a "safe" place: 0x90000-0x901FF, ie where the
! boot-block used to be. It is then up to the protected mode
! system to read them from there before the area is overwritten
! for buffer-blocks.
!

! NOTE! These had better be the same as in bootsect.s!

INITSEG  = 0x9000	! we move boot here - out of the way
SYSSEG   = 0x1000	! system loaded at 0x10000 (65536).
SETUPSEG = 0x9020	! this is the current segment

.globl begtext, begdata, begbss, endtext, enddata, endbss
.text
begtext:
.data
begdata:
.bss
begbss:
.text

entry start
start:
!Print the msg to prove that we are in setup now
	
	mov ax, #SETUPSEG
	mov ds, ax
	mov es, ax
	mov ah, #0x03
	xor bh, bh
	int 0x10

	mov cx, #25
	mov bx, #0x0007
	mov bp, #msg_setup
	mov ax, #0x1301
	int 0x10

! ok, the read went well so we get current cursor position and save it for
! posterity.

	mov	ax,#INITSEG	! this is done in bootsect already, but...
	mov	ds,ax
	mov	ah,#0x03	! read cursor pos
	xor	bh,bh
	int	0x10		! save it in known place, con_init fetches
	mov	[0],dx		! it from 0x90000.
! Get memory size (extended mem, kB)

	mov	ah,#0x88
	int	0x15
	mov	[2],ax

! Get video-card data:

	mov	ah,#0x0f
	int	0x10
	mov	[4],bx		! bh = display page
	mov	[6],ax		! al = video mode, ah = window width

! check for EGA/VGA and some config parameters

	mov	ah,#0x12
	mov	bl,#0x10
	int	0x10
	mov	[8],ax
	mov	[10],bx
	mov	[12],cx

! Get hd0 data

	mov	ax,#0x0000
	mov	ds,ax
	lds	si,[4*0x41]
	mov	ax,#INITSEG
	mov	es,ax
	mov	di,#0x0080
	mov	cx,#0x10
	rep
	movsb

! Get hd1 data

	mov	ax,#0x0000
	mov	ds,ax
	lds	si,[4*0x46]
	mov	ax,#INITSEG
	mov	es,ax
	mov	di,#0x0090
	mov	cx,#0x10
	rep
	movsb

!Print the cursor position
	mov ax, #SETUPSEG
	mov ds, ax
	mov es, ax
	mov ah, #0x03
	xor bh, bh
	int 0x10
	
	mov cx, #40
	mov bx, #0x0007
	mov bp, #msg_cursor
	mov ax, #0x1301
	int 0x10

                  MOV       AX,#INITSEG  
                  MOV       DS,AX   !SET THE DATA SEG ADDRESS, EVEN IT HAS BEEN SET.  
                        
                  MOV       CX,#4  
                  MOV       DX,[0]  
         PRINT_DIGIT1:             !print the data  
                  ROL       DX,#4  
                  MOV       AX,#0XE0F  
                  AND       AL,DL  
                  ADD       AL,#0X30  
                  CMP       AL,#0X3A  
                  JL        OUTP1  
                  ADD       AL, #0X07  
           OUTP1:  
                  INT       0X10  
                  LOOP      PRINT_DIGIT1  
                    
                  ! MOV       AX,#SETUPSEG  
                  ! MOV       DS,AX  
                  ! MOV       ES,AX  
                    
msg_setup:
	.byte 13,10
	.ascii "We are in setup now"
	.byte 13,10,13,10

msg_cursor:
	.byte 13,10
	.ascii "The cursor position is:"
	.byte 13,10

.text
endtext:
.data
enddata:
.bss
endbss:
