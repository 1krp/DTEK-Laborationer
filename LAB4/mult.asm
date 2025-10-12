addi	t0,zero,4	# imm = n
addi	t2,zero,1
add	tp,t0,zero
add	ra,zero,zero
addi	t1,t0,-1
beq	t0,t2,done	#imm 1
beq	t0,zero,done	#imm0
beq	zero,zero,mult
outloop:	
addi	tp,tp-1
beq	tp,zero,done
add	t0,ra,zero
add	t2,ra,zero
add	ra,zero,zero
addi,	t1,tp,-1
beq	zero,zero,mult
mult:	
beq	t1,zero,outloop
add	ra,ra,t0
addi	t1,t1,-1
beq	zero,zero,mult
done:	beq	zero,zero,done

