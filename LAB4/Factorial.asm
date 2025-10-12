addi	t0,zero,4		# 3 is n, 
addi	tp,zero,1
addi	ra,zero,1
add	t1,zero,zero

beq	t0,ra,done
beq	zero,zero,inloop

outLoop:	beq	ra,t0,done
addi	ra,ra,1
add	t1,zero,zero
add	tp,t2,zero


inloop:		beq	ra,t1,outLoop
add	t2,tp,tp
addi	t1,t1,1
beq	zero,zero,inloop

done:	beq	zero,zero,done