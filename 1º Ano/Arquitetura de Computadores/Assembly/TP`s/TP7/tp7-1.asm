.text
 li $t5, 10 # A
 li $t6, -5 # B
 move $t0, $zero # $t0 = result
loop:
 beqz $t6, ready
 and $t1, $t6, 1
 beqz $t1, continue
 addu $t0, $t0, $t5
continue:
 sll $t5, $t5, 1
 srl $t6, $t6, 1
 j loop
ready:
 li $v0, 1 # print int
 move $a0, $t0
 syscall
return:
 li $v0, 10
 syscall