# nome: Diogo Alexandre Botas Carvalho, número: a90247
# nome: Diogo Coelho Freitas, número: a90147

.data
filename: .asciiz "dna.txt"
searchstring: .asciiz "TGGGTGCGTTTGCCACACGCCTTTTATTTGTCTAGCCGTAACCAGAA"
foundstring: .asciiz ">>>> FOUND!"
notfoundstring: .asciiz "NOT FOUND!!!!"

filew: .space 8192     # espaço seguro para leitura do ficheiro

.text
main:
    # Abrir ficheiro
    li $v0, 13
    la $a0, filename
    li $a1, 0
    li $a2, 0
    syscall
    move $s0, $v0

    # Ler conteúdo
    li $v0, 14
    move $a0, $s0
    la $a1, filew
    li $a2, 8192
    syscall
    move $s1, $v0              # número de bytes lidos
    la $t9, filew
    add $t9, $t9, $s1          # limite = buffer + bytes lidos

    # Fechar ficheiro
    li $v0, 16
    move $a0, $s0
    syscall

    la $t0, filew          # ponteiro de leitura
    li $t7, 0                  # flag: encontrou = 0

loop_linhas:
    bgeu $t0, $t9, fim
    lb $t2, 0($t0)
    beqz $t2, fim              # fim do ficheiro

    move $t3, $t0              # $t3 aponta para nome

procurar_linha_nome:
    bgeu $t0, $t9, fim
    lb $t4, 0($t0)
    beq $t4, 10, nome_lido
    addiu $t0, $t0, 1
    j procurar_linha_nome

nome_lido:
    sb $zero, 0($t0)           # termina string do nome
    addiu $t0, $t0, 1

    # Print do nome da pessoa que está a ser analisada
    li $v0, 4
    move $a0, $t3
    syscall

    # Imprimir linefeed após o nome
    li $v0, 11
    li $a0, 10
    syscall

    move $t5, $t0              # início da sequência

procurar_linha_seq:
    bgeu $t0, $t9, fim
    lb $t6, 0($t0)
    beq $t6, 10, seq_lida
    addiu $t0, $t0, 1
    j procurar_linha_seq

seq_lida:
    sb $zero, 0($t0)
    addiu $t0, $t0, 1

    move $a0, $t5              # sequência do pai
    la $a1, searchstring

strsearch_outer:
    lb $t8, 0($a0)
    beqz $t8, loop_linhas      # fim da sequência ? próximo nome
    move $s2, $a0
    la $s1, searchstring

strsearch_inner:
    lb $s3, 0($s2)
    lb $s4, 0($s1)
    beqz $s4, encontrado       # encontrou a searchstring
    beqz $s3, strsearch_fail
    bne $s3, $s4, strsearch_fail
    addiu $s2, $s2, 1
    addiu $s1, $s1, 1
    j strsearch_inner

strsearch_fail:
    addiu $a0, $a0, 1
    j strsearch_outer

encontrado:
    li $t7, 1
    li $v0, 4
    la $a0, foundstring
    syscall
    j done

fim:
    beqz $t7, notfound
    j done

notfound:
    li $v0, 4
    la $a0, notfoundstring
    syscall

done:
    li $v0, 10
    syscall
