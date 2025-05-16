.data
myintsarray:     .word 8 7 32 8 6 11 9
int_size:        .word 7

myfloatsarray:   .float 7.0 8.0 4.0 -13.1 67.3 9.7 4.1 -1.4
float_size:      .word 8

newline:         .asciiz "\n"
intmsg:          .asciiz "Sorted int array:\n"
floatmsg:        .asciiz "Sorted float array:\n"

.text

############################
# main
############################
main:
    # sort integer array
    li $v0, 4
    la $a0, intmsg
    syscall

    la $a0, myintsarray
    lw $a1, int_size
    li $a2, 4
    la $a3, int_compare
    jal bsort

    la $a0, myintsarray
    lw $a1, int_size
    jal print_int_array

    # sort float array
    li $v0, 4
    la $a0, floatmsg
    syscall

    la $a0, myfloatsarray
    lw $a1, float_size
    li $a2, 4
    la $a3, float_compare
    jal bsort

    la $a0, myfloatsarray
    lw $a1, float_size
    jal print_float_array

    li $v0, 10
    syscall



############################
# bsort
# Arguments:
# $a0 - base address of array
# $a1 - number of elements
# $a2 - size of one element (in bytes)
# $a3 - address of comparison function
############################
bsort:
    addi $sp, $sp, -16
    sw $ra, 0($sp)
    sw $s0, 4($sp)
    sw $s1, 8($sp)
    sw $s2, 12($sp)

    move $s0, $a0        # base address
    move $s1, $a1        # element count
    move $s2, $a2        # element size

outer_loop:
    li $t0, 0            # change = 0 (false)
    li $t1, 0            # i = 0

inner_loop:
    # if i >= n-1, break
    addi $t2, $s1, -1
    bge $t1, $t2, check_exit

    # compute addresses of element[i] and element[i+1]
    mul $t3, $t1, $s2
    add $t4, $s0, $t3              # addr1 = base + i * size
    add $t5, $t3, $s2
    add $t5, $s0, $t5              # addr2 = base + (i+1) * size

    # prepara argumentos para função de comparação
    move $a0, $t4
    move $a1, $t5
    jalr $a3

    # return value in $v0: 1 if element[i] > element[i+1]

    lw $t4, 0($sp)
    lw $t5, 4($sp)
    addi $sp, $sp, 8

    beqz $v0, skip_swap

    # swap the elements byte-by-byte
    li $t6, 0
swap_loop:
    bge $t6, $s2, done_swap
    lb $t7, 0($t4)
    lb $t8, 0($t5)
    sb $t8, 0($t4)
    sb $t7, 0($t5)
    addi $t4, $t4, 1
    addi $t5, $t5, 1
    addi $t6, $t6, 1
    j swap_loop
done_swap:
    li $t0, 1  # change = true
skip_swap:
    addi $t1, $t1, 1
    j inner_loop

check_exit:
    beqz $t0, exit_bsort
    j outer_loop

exit_bsort:
    lw $ra, 0($sp)
    lw $s0, 4($sp)
    lw $s1, 8($sp)
    lw $s2, 12($sp)
    addi $sp, $sp, 16
    jr $ra

############################
# int_compare
# $a0: addr1
# $a1: addr2
# Returns:
# $v0 = 1 if *addr1 > *addr2
#       0 otherwise
############################
int_compare:
    lw $t0, 0($a0)
    lw $t1, 0($a1)
    slt $v0, $t1, $t0
    jr $ra

############################
# float_compare
# $a0: addr1
# $a1: addr2
# Returns:
# $v0 = 1 if *addr1 > *addr2
#       0 otherwise
############################
float_compare:
    l.s $f0, 0($a0)
    l.s $f1, 0($a1)
    c.le.s $f1, $f0
    bc1f notgreater
    li $v0, 1
    jr $ra
notgreater:
    li $v0, 0
    jr $ra

############################
# print_int_array
# $a0 = base address
# $a1 = number of elements
############################
print_int_array:
    li $t0, 0
print_int_loop:
    bge $t0, $a1, print_done
    mul $t1, $t0, 4
    add $t2, $a0, $t1
    lw $a0, 0($t2)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    addi $t0, $t0, 1
    j print_int_loop
print_done:
    jr $ra

############################
# print_float_array
# $a0 = base address
# $a1 = number of elements
############################
print_float_array:
    li $t0, 0
print_float_loop:
    bge $t0, $a1, printf_done
    mul $t1, $t0, 4
    add $t2, $a0, $t1
    l.s $f12, 0($t2)
    li $v0, 2
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    addi $t0, $t0, 1
    j print_float_loop
printf_done:
    jr $ra
