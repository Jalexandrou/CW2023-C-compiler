.text
.globl f

test:
	addi    sp, sp, -16
	sw      s0, 12(sp)
	addi    s0, sp, 16
	li      a0, 3
	lw      s0, 12(sp)
	addi    sp, sp, 16
	jr      ra