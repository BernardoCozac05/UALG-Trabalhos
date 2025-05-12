.data
myintsarray:    .word 8,7,32,8,6,11,9
myfloatsarray:  .float 7.0,8.0,4.0,-13.1,67.3,9.7,4.1,-1.4
n_ints:    .word 7
n_floats: .word 8

#######################################################
###################- M A C R O S -#####################

.macro print_int (%p)
li $v0, 1
move $a0, %p
syscall
.end_macro 

.macro print_fp (%p)
lwc1  $f12,0(%p)
li $v0, 2
syscall
.end_macro 

.macro PNL
li $v0, 11			# Syscall to print a character
li $a0, 10			# ASCII code for newline '\n'
syscall
.end_macro 

.macro PSPACE
li $v0, 11			# Syscall to print a character
li $a0, 32			# ASCII code for space ' '
syscall
.end_macro 

.macro exit
li $v0, 10
syscall
.end_macro 



.text
main:
    # Print original integer array
    li    $t0,0            # index = 0
    lw    $t1,n_ints       # t1 = number of ints
print_int_loop:
    beq   $t0,$t1,after_print_int
    la    $t2,myintsarray       # base address
    sll   $t3,$t0,2        # offset = index*4
    add   $t4,$t2,$t3
    lw    $t5,0($t4)       # load int
    print_int $t5
    addi  $t0,$t0,1
    blt   $t0,$t1,print_int_space
    j     print_int_loop
print_int_space:
PSPACE
    j     print_int_loop
after_print_int:
PNL

    # Sort integer array
    la    $a0,myintsarray
    lw    $a1,n_ints
    li    $a2,4
    la    $a3,int_cmp
    jal   bsort

    # Print sorted integer array
    li    $t0,0
print_sorted_int:
    beq   $t0,$t1,after_sorted_int
    la    $t2,myintsarray
    sll   $t3,$t0,2
    add   $t4,$t2,$t3
    lw    $t5,0($t4)
    li    $v0,1
    move  $a0,$t5
    syscall
    addi  $t0,$t0,1
    blt   $t0,$t1,print_int_space2
    j     print_sorted_int
print_int_space2:
PSPACE
    j     print_sorted_int
after_sorted_int:
PNL

    # Print original float array
    li    $t0,0
    lw    $t1,n_floats
print_float_loop:
    beq   $t0,$t1,after_print_float
    la    $t2,myfloatsarray
    sll   $t3,$t0,2
    add   $t4,$t2,$t3
print_fp $t4
    addi  $t0,$t0,1
    blt   $t0,$t1,print_float_space
    j     print_float_loop
print_float_space:
PSPACE
    j     print_float_loop
after_print_float:
PNL

    # Sort float array
    la    $a0,myfloatsarray
    lw    $a1,n_floats
    li    $a2,4
    la    $a3,float_cmp
    jal   bsort

    # Print sorted float array
    li    $t0,0
print_sorted_float:
    beq   $t0,$t1,after_sorted_float
    la    $t2,myfloatsarray
    sll   $t3,$t0,2
    add   $t4,$t2,$t3
print_fp $t4
    addi  $t0,$t0,1
    blt   $t0,$t1,print_float_space2
    j     print_sorted_float
print_float_space2:
PSPACE
    j     print_sorted_float
after_sorted_float:
PNL

# Exit
exit

# bsort(base,nitems,size,compare) - recursive bubble sort
bsort:
    addi  $sp,$sp,-20
    sw    $ra,16($sp)
    sw    $s0,12($sp)
    sw    $s1, 8($sp)
    sw    $s2, 4($sp)
    sw    $s3, 0($sp)
    move  $s0,$a0
    move  $s1,$a1
    move  $s2,$a2
    move  $s3,$a3
    li    $t0,2
    blt   $s1,$t0,bsort_done
    # one bubble pass
    move  $a0,$s0
    move  $a1,$s1
    move  $a2,$s2
    move  $a3,$s3
    jal   bubble
    addi  $s1,$s1,-1
    # recurse on remaining
    move  $a0,$s0
    move  $a1,$s1
    move  $a2,$s2
    move  $a3,$s3
    jal   bsort
bsort_done:
    lw    $ra,16($sp)
    lw    $s0,12($sp)
    lw    $s1, 8($sp)
    lw    $s2, 4($sp)
    lw    $s3, 0($sp)
    addi  $sp,$sp,20
    jr    $ra

# bubble(base,nitems,size,compare) - one-pass bubble
bubble:
    addi  $sp,$sp,-20
    sw    $ra,16($sp)
    sw    $s0,12($sp)
    sw    $s1, 8($sp)
    sw    $s2, 4($sp)
    sw    $s3, 0($sp)
    move  $s0,$a0
    move  $s1,$a1
    move  $s2,$a2
    move  $s3,$a3
    # if nitems < 2, done
    slti  $t0,$s1,2
    bne   $t0,$zero,bubble_done
    # compare A[0] vs A[1]
    move  $a0,$s0
    add   $a1,$s0,$s2
    jalr  $s3       # compare()
    blez  $v0,no_swap
    # swap
    lw    $t2,0($s0)
    lw    $t3,0($a1)
    sw    $t2,0($a1)
    sw    $t3,0($s0)
no_swap:
    add   $s0,$s0,$s2
    addi  $s1,$s1,-1
    # recurse on rest
    move  $a0,$s0
    move  $a1,$s1
    move  $a2,$s2
    move  $a3,$s3
    jal   bubble
bubble_done:
    lw    $ra,16($sp)
    lw    $s0,12($sp)
    lw    $s1, 8($sp)
    lw    $s2, 4($sp)
    lw    $s3, 0($sp)
    addi  $sp,$sp,20
    jr    $ra

# int_cmp(a0,a1) -> v0 = a0 - a1
int_cmp:
    lw    $t0,0($a0)
    lw    $t1,0($a1)
    subu  $v0,$t0,$t1
    jr    $ra

# float_cmp(a0,a1) -> v0 = -1,0,1
float_cmp:
    lwc1  $f0,0($a0)
    lwc1  $f1,0($a1)
    c.lt.s $f0,$f1
    bc1t  Lfl_less
    c.lt.s $f1,$f0
    bc1t  Lfl_greater
    li    $v0,0
    jr    $ra
Lfl_less:
    li    $v0,-1
    jr    $ra
Lfl_greater:
    li    $v0,1
    jr    $ra