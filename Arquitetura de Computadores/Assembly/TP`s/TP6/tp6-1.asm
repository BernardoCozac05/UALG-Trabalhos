.data
hellow: .asciiz "Hello world!\n"
.text
main:
 jal printhello
 jal printhello
 jal printhello

 li $v0, 10
 syscall

################## FUNCTIONS ###############
printhello:
 li $v0, 4
 la $a0, hellow
 syscall
 jr $ra # return