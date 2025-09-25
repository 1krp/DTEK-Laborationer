# timetemplate.S
# Written 2015 by F Lundevall
# Ported 2024/06 by W Szczerek (from MIPS to RISC-V)
# Copyright abandonded - this file is in the public domain.

#############################################################
# Choose the macro syntax for RARS or DTEK-V board.         #
# The syntax for RARS is probably due to its MIPS heritage. #
#############################################################
###################
# Macros for RARS #
###################
#.macro	PUSH (%reg)
#	addi	sp,sp,-4
#	sw	%reg,0(sp) 
#.end_macro
#
#.macro	POP (%reg)
#	lw	%reg,0(sp)
#	addi	sp,sp,4
#.end_macro
###################
# Macros for DTEK #
###################
.macro	PUSH reg
	addi sp,sp,-4
	sw \reg,0(sp) 
.endm

.macro	POP reg
	lw	\reg,0(sp)
	addi	sp,sp,4
.endm
#############################################################

	.data
	.align 2
mytime:	.word 	0x5957
timstr:	.asciz 	"text more text lots of text\0"
	.text
	.globl timetemplate, tick, time2string, delay, display_string

# Function for displaying a string with a newline at the end	
display_string:	
	li a7,4
	ecall
	li a0, 10
	li a7,11
	ecall
	jr ra
	
timetemplate:
	la	a0, timstr
	jal     display_string
	
	# wait a little
	li	a0, 1000		# ms
	jal	delay
	
	# call tick
	la 	a0, mytime
	jal	tick
	
	# call your function time2string
	la	a0, timstr
	la	t0, mytime
	lw	a1, 0(t0)
	jal	time2string
	
	# go back and do it all again
	j	timetemplate

	
# tick: update time pointed to by $a0
tick:	lw	t0, 0(a0)	# get time
	addi	t0, t0, 1	# increase
	andi	t1, t0, 0xf	# check lowest digit
	sltiu	t2, t1, 0xa	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x6	# adjust lowest digit
	
	andi	t1, t0, 0xf0	# check next digit
	sltiu	t2, t1, 0x60	# if digit < 6, okay
	bnez	t2, tiend
	addi	t0, t0, 0xa0	# adjust digit
	
	li	t3, 0xF
	slli	t3, t3, 0x8
	and	t1, t0, t3	# check minute digit
	addi	t3, x0, 0xA
	slli	t3, t3, 0x8
	slt	t2, t1, t3	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x600	# adjust digit - this one's okay, it's lower than 0x7FF 
	
	li	t3, 0xF
	slli	t3, t3, 0xC
	and	t1, t0, t3	# check last digit
	addi	t3, x0, 0x6
	slli	t3, t3, 0xC
	slt	t2, t1, t3	# if digit < 6, okay
	bnez	t2, tiend
	
	li	t3, 0xA
	slli	t3, t3, 0xC
	add	t0, t0, t3	# adjust last digit
tiend:	sw	t0,0(a0)	# save updated result
	jr	ra		# return

#########################################################
# Place for your functions: time2string, hex2asc, delay.#
#########################################################

# Input a0 - håller värde 0-9, tex 0x3
hex2asc:
	addi a0, a0, 0x30
	ret
    
# Input a0 - håller antal ms, tex 10
delay:
	
	PUSH ra
	
out_loop:
	blez 	a0, done_out_loop	# while a0 (ms) > 0
	addi 	a0, a0, -1		# ms--
	
	li t0, 0	# i = 0
	li t1, 9999	# limit
	
in_loop:
	beq	t0, t1, done_in_loop
	addi	t0, t0, 1
	j	in_loop
	
done_in_loop:
	j	out_loop
	
done_out_loop:
	
	POP ra
	jr ra
	
time2string:

	PUSH ra
	PUSH s0
	
	add	s0, zero, a0

	# Store null-byte
	li      t0, 0
	sb 	t0, 5(s0)

	# Store second second
	andi	a0, a1, 0xF	# Tar ut lsn från a1
	jal	hex2asc		# returnerar ascii-värdet i a2
	sb 	a0, 4(s0)
	srli	a1, a1, 4	# Right shift a1 4 steg

	# Store first second
	andi	a0, a1, 0xF	# Tar ut lsn från a1
	jal	hex2asc		# returnerar ascii-värdet i a2
	sb 	a0, 3(s0)
	srli	a1, a1, 4	# Right shift a1 4 steg

	# Store colon
	li      t0, 0x3A      	# ASCII ':'
	sb 	t0, 2(s0)

	# Store second min
	andi	a0, a1, 0xF	# Tar ut lsn från a1
	jal	hex2asc		# returnerar ascii-värdet i a2
	sb 	a0, 1(s0)
	srli	a1, a1, 4	# Right shift a1 4 steg

	# Store first min
	andi	a0, a1, 0xF	# Tar ut lsn från a1
	jal	hex2asc		# returnerar ascii-värdet i a2
	sb 	a0, 0(s0)
	
	POP s0
	POP ra

	jr	ra
