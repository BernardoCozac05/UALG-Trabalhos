#Programa assamble no MIPS que escreve "Ola Mundo!"

#data segment with "variables"
.data	

olam: .asciiz "Ola Mundo!"

#Instrucoes do codigo

.text

li $v0, 4
la $a0, olam
syscall # 4: print string

#Terminar Programa
li $v0, 10
syscall