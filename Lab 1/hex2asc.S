  # hexmain.S
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
	.globl hex2asc

hex2asc:
	li	a0, 17	# test number (from 0 to 15)
	
	addi    sp,sp,-4	# push av ra
	sw      ra,0(sp)
	
	jal	hexasc		# call hexasc
	
	li	a7, 11	# write a0 to stdout
	ecall

	lw      ra,0(sp)	# pop av ra
	addi    sp,sp,4
	jr      ra	

  # You can write your own code for hexasc here
  
hexasc:	
	li 	t0, 0x30	# 0 in ASCII
	li	t1, 0x37	# A in ASCII - 10, 0x41 -0xA = 0x37
	li	t2, 9		
	
	andi	t3, a0, 0xF	# Find last nibble of a0, using bitwise and, and (1111)
	
	ble	t3, t2, startFromZero  # if t3 <= t2 (9)
	
	# else
	add	a0, t3, t1
	ret
	
startFromZero:
	add 	a0, t3, t0
	ret	
	
	
# Question: 
# No s registers are used.
# If argument is 17 the reurn is 0x1, ...0001.
# The branch is taken if t4 <= t2

	

