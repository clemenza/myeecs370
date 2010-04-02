	lw	0	1	n	$1 = n
	lw	0	2	r	$2 = r
	lw	0	4	comba	prepare to call combin	$4 = combin
	jalr	4	7	 call combn; $7 = return add;$3 = ans
	halt
combn	beq	0	2	ret	if r == 0 then $3 = 1
	beq	2	1	ret	like last line
	lw	0	6	pos1	$6 = 1
	sw	5	7	stack	save return addr on stack
	add	5	6	5	increment stack pointer
	sw	5	1	stack	save n
	add	5	6	5
	sw	5	2	stack	save r
	add	5	6	5	increment stack pointer
	sw	5	4	stack	save $4
	add	5	6	5	increment stack pointer
	lw	0	6	neg1	$6 = -1
	add	1	6	1	n -= 1
	lw	0	4	comba
	jalr	4	7	
	lw	0	6	neg1	$6 = -1
	add	5	6	5	decrement stack pointer
	lw	5	4	stack	restore $4
	add	5	6	5	decrement stack pointer
	lw	5	2	stack	restore r
	add	5	6	5	decrement stack pointer
	lw	5	1	stack	restore n
	add	5	6	5	decrement stack pointer
	lw	5	7	stack	restore return addr
	add	0	3	4	keep $3 in $4 tmp	
	add	2	6	2	r -= 1
	add	1	6	1	n -= 1
	lw	0	6	pos1
	sw	5	7	stack	save return addr
	add	5	6	5	increment stack pointer
	sw	5	1	stack	save n
	add	5	6	5	increment stack pointer
	sw	5	2	stack	save r
	add	5	6	5	increment stack pointer
	sw	5	4	stack	save $4
	add	5	6	5	increment stack pointer
	lw	0	4	comba
	jalr	4	7
	lw	0	6	neg1
	add	5	6	5	decrement stack pointer
	lw	5	4	stack	restore stack pointer
	add	5	6	5	decrement stack pointer
	lw	5	2	stack	restore r
	add	5	6	5	decrement stack pointer
	lw	5	1	stack	restore n
	add	5	6	5	decrement stack pointer
	lw	5	7	stack	restore return addr
	add	3	4	3	combn(n,r)=combn(n-1,r)+combn(n-1,r-1)
goOn	jalr	7	4
ret	lw	0	6	pos1	$6 = 1
	add	0	6	3	$3 = 1 right now
	beq	0	0	goOn	go on to execute
pos1	.fill	1
neg1	.fill	-1
n	.fill	10
r	.fill	5
comba	.fill	combn
goa	.fill	goOn
stack	.fill	0
