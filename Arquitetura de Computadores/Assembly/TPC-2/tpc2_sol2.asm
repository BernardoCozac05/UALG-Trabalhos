# nome: Diogo Alexandre Botas Carvalho, número: a90247
# nome: Diogo Coelho Freitas, número: a90147

.data
filename: .asciiz "dna.txt"
searchstring: .asciiz "TGGGTGCGTTTGCCACACGCCTTTTATTTGTCTAGCCGTAACCAGAA"
foundstring: .asciiz ">>>> FOUND!"
notfoundstring: .asciiz "NOT FOUND!!!!"

char: .byte 0           # buffer de 1 byte para leitura do ficheiro
nome: .space 128        # buffer temporário para nome (1 linha)
seq: .space 512         # buffer para sequência de DNA

.text
main:
    # Abrir ficheiro
    li $v0, 13
    la $a0, filename
    li $a1, 0
    li $a2, 0
    syscall
    move $s0, $v0        # descritor do ficheiro

    li $t7, 0            # flag encontrou

loop_pessoas:
    # Ler nome para buffer
    la $t0, nome
ler_nome:
    li $v0, 14
    move $a0, $s0
    la $a1, char
    li $a2, 1
    syscall
    beqz $v0, fim            # EOF
    lb $t1, char
    li $t2, 10
    beq $t1, $t2, nome_lido
    sb $t1, 0($t0)
    addiu $t0, $t0, 1
    j ler_nome

nome_lido:
    sb $zero, 0($t0)         # termina string

    # Print do nome
    li $v0, 4
    la $a0, nome
    syscall

    li $v0, 11
    li $a0, 10               # '\n'
    syscall

    # Ler DNA para buffer
    la $t0, seq
ler_seq:
    li $v0, 14
    move $a0, $s0
    la $a1, char
    li $a2, 1
    syscall
    beqz $v0, fim
    lb $t1, char
    li $t2, 10
    beq $t1, $t2, seq_lida
    sb $t1, 0($t0)
    addiu $t0, $t0, 1
    j ler_seq

seq_lida:
    sb $zero, 0($t0)

    # strstr(seq, searchstring)
    la $t3, seq              # $t3 percorre seq

strstr_outer:
    lb $t4, 0($t3)
    beqz $t4, loop_pessoas   # fim ? próxima pessoa

    move $t5, $t3            # $t5 = posição atual da sequência
    la $t6, searchstring     # $t6 = início da string a procurar

strstr_inner:
    lb $t8, 0($t5)           # da sequência
    lb $t9, 0($t6)           # do filho
    beqz $t9, encontrado     # chegou ao fim da searchstring ? sucesso
    beqz $t8, falha_str
    bne $t8, $t9, falha_str
    addiu $t5, $t5, 1
    addiu $t6, $t6, 1
    j strstr_inner

falha_str:
    addiu $t3, $t3, 1
    j strstr_outer

encontrado:
    li $t7, 1
    li $v0, 4
    la $a0, foundstring
    syscall
    j fechar

fim:
    beqz $t7, notfound
    j fechar

notfound:
    li $v0, 4
    la $a0, notfoundstring
    syscall

fechar:
    li $v0, 16
    move $a0, $s0
    syscall

    li $v0, 10
    syscall
