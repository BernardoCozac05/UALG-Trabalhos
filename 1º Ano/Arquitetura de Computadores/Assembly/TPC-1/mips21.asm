.data
.text
main:
    li $s0, 90        # ASCII de 'Z'
    li $s1, 32        # ASCII do espaço (' ')

read_loop:
    li $v0, 12        # syscall 12: read char
    syscall
    move $t1, $v0     # guardar o char lido em $t1

    # parar se for '\n' ou '\0'
    li $t3, 10        # newline
    beq $t1, $t3, exit
    beqz $t1, exit

    move $t2, $t1     # copiar char para $t2

    beq $t2, $s1, print      # se for espaço, imprime
    blt $t2, $s0, print      # se menor que 'Z', imprime
    bgt $t2, $s0, decrease   # se maior que 'Z', subtrai 32

decrease:
    sub $t2, $t2, $s1        # $t2 = $t2 - 32

print:
    li $v0, 11         # syscall 11: print char
    move $a0, $t2
    syscall

    j read_loop        # ler próximo char

exit:
    li $v0, 10         # syscall 10: exit
    syscall
