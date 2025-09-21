# name: Simão Marques Sousa, number: a90466
# name: Bernado Filipe Cardeira Cozac: a90142

.data
filename: .asciiz "dna.txt"
searchstring: .asciiz "TGGGTGCGTTTGCCACACGCCTTTTATTTGTCTAGCCGTAACCAGAA"
found_msg: .asciiz ">>>> FOUND!"
notfound_msg: .asciiz "NOT FOUND!!!!"

dna: .space 750
buffer: .space 200
char: .byte 0



.text
main:
    li $v0, 13
    la $a0, filename
    li $a1, 0
    li $a2, 0
    syscall
    move $s0, $v0

    li $s1, 0

read_next_person:
    la $t0, buffer

read_name_loop:
    li $v0, 14
    move $a0, $s0
    la $a1, char
    li $a2, 1
    syscall
    beqz $v0, end
    lb $t1, char
    li $t2, 10
    beq $t1, $t2, name
    sb $t1, 0($t0)
    addiu $t0, $t0, 1
    j read_name_loop

name:
    sb $zero, 0($t0)

    li $v0, 4
    la $a0, buffer
    syscall

    li $v0, 11
    li $a0, 10
    syscall

    la $t0, dna

read_dna_loop:
    li $v0, 14
    move $a0, $s0
    la $a1, char
    li $a2, 1
    syscall
    beqz $v0, end
    lb $t1, char
    li $t2, 10
    beq $t1, $t2, dna_done
    sb $t1, 0($t0)
    addiu $t0, $t0, 1
    j read_dna_loop

dna_done:
    sb $zero, 0($t0)

    la $t3, dna

strstr_outer_loop:
    lb $t4, 0($t3)
    beqz $t4, read_next_person

    move $t5, $t3
    la $t6, searchstring

strstr_inner_loop:
    lb $t7, 0($t5)
    lb $t8, 0($t6)
    beqz $t8, match_found
    beqz $t7, try_next_position
    bne $t7, $t8, try_next_position
    addiu $t5, $t5, 1
    addiu $t6, $t6, 1
    j strstr_inner_loop

try_next_position:
    addiu $t3, $t3, 1
    j strstr_outer_loop

match_found:
    li $s1, 1
    li $v0, 4
    la $a0, found_msg
    syscall
    j close_file

end:
    beqz $s1, print_not_found
    j close_file

print_not_found:
    li $v0, 4
    la $a0, notfound_msg
    syscall

close_file:
    li $v0, 16
    move $a0, $s0
    syscall

    li $v0, 10
    syscall
