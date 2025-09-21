.data
newline: .asciiz "\n"
.text
main:
 li $t0, 1

loop:
 bgt $t0, 10, ready
 move $a0, $t0
 jal square
 move $a0, $v0
 li $v0, 1
 syscall
 li $v0, 4
 la $a0, newline
 syscall
 addi $t0, $t0, 1
 j loop

ready:
 li $v0, 10
 syscall

################## FUNCTIONS ###############
square:
############################################
# Square of argument #
# Input: $a0: int #
# Output: $v0: int #
############################################
 mul $v0, $a0, $a0
 jr $ra # return
