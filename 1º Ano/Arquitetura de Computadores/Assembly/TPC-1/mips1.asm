.data
str: .ascii "Universidade do Algarve\0"

.text
main:
li $s0, 90
li $s1, 32
la $t0, str

strv:
lb $t1, 0($t0)
beqz $t1, exit

move $t2, $t1

beq $t2, $s1, print
blt $t2, $s0, print
bgt $t2, $s0, dcrease

dcrease:
sub $t2, $t2, $s1

print:
li $v0, 11
move $a0, $t2
syscall

next:
addi $t0, $t0, 1
li $t1,0
j strv

exit:
li $v0, 11
li $a0, 10
syscall

sll $t2, $t1, 2

li $v0, 10
syscall

