# MIPS asset+ler program that prints "Hello world! "
#data segrent with 'variables'

.data
hellow: .asciiz "Hello world!"

#code segnpnt with instructions

.text

li $v0, 4
la $a0, hellow
syscall # 4: print string

#terminate program
li $v0, 10
syscall