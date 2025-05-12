        .data
# ===== Data Segment =====
myintsarray:      .word 8,7,32,8,6,11,9
myintsarray_end:

myfloatsarray:    .float 7.0,8.0,4.0,-13.1,67.3,9.7,4.1,-1.4
myfloatsarray_end:

.text

# ===== I/O Macros =====
.macro print_int %r         # print integer in register %r
    move $a0,%r
    li   $v0,1
    syscall
.end_macro

.macro print_fp %p          # print float at address %p
    lwc1 $f12,0(%p)
    li   $v0,2
    syscall
.end_macro

.macro PNL                  # print newline
    li $v0,11
    li $a0,10    # '\n'
    syscall
.end_macro

.macro PSPACE               # print space
    li $v0,11
    li $a0,32    # ' '
    syscall
.end_macro

.macro exit                 # exit program
    li $v0,10
    syscall
.end_macro

# ===== Main Program =====
main:
    # ------------------------------
    # Compute number of ints: (end-start)/4
    # ------------------------------
    la   $t2,myintsarray_end   # address after last element
    la   $t3,myintsarray       # start address
    subu $t1,$t2,$t3           # total bytes
    srl  $t1,$t1,2             # divide by 4 -> count in $t1

    # Print original int array
    li   $t0,0                 # index = 0
print_int_loop:
    beq  $t0,$t1,after_print_int
    la   $t2,myintsarray
    sll  $t3,$t0,2
    add  $t4,$t2,$t3
    lw   $t5,0($t4)
    print_int $t5
    addi $t0,$t0,1
    blt  $t0,$t1,print_space
    j    print_int_loop
print_space:
    PSPACE
    j    print_int_loop
after_print_int:
    PNL

    # ------------------------------
    # Sort integer array
    # ------------------------------
    la   $a0,myintsarray
    move $a1,$t1              # count
    li   $a2,4                # element size
    la   $a3,int_cmp
    jal  bsort

    # Print sorted int array
    li   $t0,0
print_sorted_int:
    beq  $t0,$t1,after_sorted_int
    la   $t2,myintsarray
    sll  $t3,$t0,2
    add  $t4,$t2,$t3
    lw   $t5,0($t4)
    print_int $t5
    addi $t0,$t0,1
    blt  $t0,$t1,print_space2
    j    print_sorted_int
print_space2:
    PSPACE
    j    print_sorted_int
after_sorted_int:
    PNL

    # ------------------------------
    # Compute number of floats
    # ------------------------------
    la   $t2,myfloatsarray_end
    la   $t3,myfloatsarray
    subu $t1,$t2,$t3
    srl  $t1,$t1,2

    # Print original float array
    li   $t0,0
print_float_loop:
    beq  $t0,$t1,after_print_float
    la   $t2,myfloatsarray
    sll  $t3,$t0,2
    add  $t4,$t2,$t3
    print_fp $t4
    addi $t0,$t0,1
    blt  $t0,$t1,print_float_space
    j    print_float_loop
print_float_space:
    PSPACE
    j    print_float_loop
after_print_float:
    PNL

    # ------------------------------
    # Sort float array
    # ------------------------------
    la   $a0,myfloatsarray
    move $a1,$t1
    li   $a2,4
    la   $a3,float_cmp
    jal  bsort

    # Print sorted float array
    li   $t0,0
print_sorted_float:
    beq  $t0,$t1,after_sorted_float
    la   $t2,myfloatsarray
    sll  $t3,$t0,2
    add  $t4,$t2,$t3
    print_fp $t4
    addi $t0,$t0,1
    blt  $t0,$t1,print_float_space2
    j    print_sorted_float
print_float_space2:
    PSPACE
    j    print_sorted_float
after_sorted_float:
    PNL

    # Exit
    exit

# ===== Recursive Bubble Sort =====
bsort:
    addi $sp,$sp,-20
    sw   $ra,16($sp)
    sw   $s0,12($sp)
    sw   $s1,8($sp)
    sw   $s2,4($sp)
    sw   $s3,0($sp)
    move $s0,$a0
    move $s1,$a1
    move $s2,$a2
    move $s3,$a3
    li   $t0,2
    blt  $s1,$t0,bsort_done
    move $a0,$s0
    move $a1,$s1
    move $a2,$s2
    move $a3,$s3
    jal  bubble
    addi $s1,$s1,-1
    move $a0,$s0
    move $a1,$s1
    move $a2,$s2
    move $a3,$s3
    jal  bsort
bsort_done:
    lw   $ra,16($sp)
    lw   $s0,12($sp)
    lw   $s1,8($sp)
    lw   $s2,4($sp)
    lw   $s3,0($sp)
    addi $sp,$sp,20
    jr   $ra

# ===== One-Pass Bubble =====
bubble:
    addi $sp,$sp,-20
    sw   $ra,16($sp)
    sw   $s0,12($sp)
    sw   $s1,8($sp)
    sw   $s2,4($sp)
    sw   $s3,0($sp)
    move $s0,$a0
    move $s1,$a1
    move $s2,$a2
    move $s3,$a3
    slti $t0,$s1,2
    bne  $t0,$zero,bubble_done
    move $a0,$s0
    add  $a1,$s0,$s2
    jalr $s3
    blez $v0,no_swap
    lw   $t2,0($s0)
    lw   $t3,0($a1)
    sw   $t2,0($a1)
    sw   $t3,0($s0)
no_swap:
    add  $s0,$s0,$s2
    addi $s1,$s1,-1
    move $a0,$s0
    move $a1,$s1
    move $a2,$s2
    move $a3,$s3
    jal  bubble
bubble_done:
    lw   $ra,16($sp)
    lw   $s0,12($sp)
    lw   $s1,8($sp)
    lw   $s2,4($sp)
    lw   $s3,0($sp)
    addi $sp,$sp,20
    jr   $ra

# ===== Compare Ints =====
int_cmp:
    lw   $t0,0($a0)
    lw   $t1,0($a1)
    subu $v0,$t0,$t1
    jr   $ra

# ===== Compare Floats =====
float_cmp:
    lwc1 $f0,0($a0)
    lwc1 $f1,0($a1)
    c.lt.s $f0,$f1
    bc1t Lfl_less
    c.lt.s $f1,$f0
    bc1t Lfl_greater
    li   $v0,0
    jr   $ra
Lfl_less:
    li   $v0,-1
    jr   $ra
Lfl_greater:
    li   $v0,1
    jr   $ra
