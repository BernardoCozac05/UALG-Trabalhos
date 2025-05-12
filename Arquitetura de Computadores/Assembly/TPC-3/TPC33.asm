.data
# ===== Data Segment =====
myintsarray:      .word 8,7,32,8,6,11,9
myintsarray_end:

myfloatsarray:    .float 7.0,8.0,4.0,-13.1,67.3,9.7,4.1,-1.4
myfloatsarray_end:

.text
.globl main

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
    # Compute number of ints: (end-start)/4
    la   $t2,myintsarray_end
    la   $t3,myintsarray
    subu $t1,$t2,$t3
    srl  $t1,$t1,2             # divide by 4 -> count in $t1

    # Print original int array recursively
    la   $a0,myintsarray       # address
    move $a1,$t1               # count
    jal  print_ints_rec
    PNL

    # Sort integer array
    la   $a0,myintsarray
    move $a1,$t1               # count
    li   $a2,4                 # element size
    la   $a3,int_cmp
    jal  bsort

    # Print sorted int array recursively
    la   $a0,myintsarray
    move $a1,$t1
    jal  print_ints_rec
    PNL

    # Compute number of floats
    la   $t2,myfloatsarray_end
    la   $t3,myfloatsarray
    subu $t1,$t2,$t3
    srl  $t1,$t1,2

    # Print original float array recursively
    la   $a0,myfloatsarray
    move $a1,$t1
    jal  print_floats_rec
    PNL

    # Sort float array
    la   $a0,myfloatsarray
    move $a1,$t1
    li   $a2,4
    la   $a3,float_cmp
    jal  bsort

    # Print sorted float array recursively
    la   $a0,myfloatsarray
    move $a1,$t1
    jal  print_floats_rec
    PNL

    exit

# ===== Recursive Int Printer =====
# print_ints_rec(addr, count)
print_ints_rec:
    addi $sp,$sp,-8
    sw   $ra,4($sp)
    sw   $s0,0($sp)
    move $s0,$a0            # current address
    beqz $a1,pr_intr_done   # if count==0

    # print current element
    lw   $t0,0($s0)
    print_int $t0
    addi $a1,$a1,-1         # count--
    beqz $a1,skip_space
    PSPACE
skip_space:
    # recurse on next
    addi $a0,$s0,4
    jal  print_ints_rec
pr_intr_done:
    lw   $ra,4($sp)
    lw   $s0,0($sp)
    addi $sp,$sp,8
    jr   $ra

# ===== Recursive Float Printer =====
# print_floats_rec(addr, count)
print_floats_rec:
    addi $sp,$sp,-8
    sw   $ra,4($sp)
    sw   $s0,0($sp)
    move $s0,$a0
    beqz $a1,pr_flt_done

    # print current float
    print_fp $s0
    addi $a1,$a1,-1
    beqz $a1,skip_flt_space
    PSPACE
skip_flt_space:
    # recurse
    addi $a0,$s0,4
    jal  print_floats_rec
pr_flt_done:
    lw   $ra,4($sp)
    lw   $s0,0($sp)
    addi $sp,$sp,8
    jr   $ra

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
    blt  $s1,$t0,bsort_ret

    # one bubble pass
    move $a0,$s0
    move $a1,$s1
    move $a2,$s2
    move $a3,$s3
    jal  bubble
    addi $s1,$s1,-1

    # recurse
    move $a0,$s0
    move $a1,$s1
    move $a2,$s2
    move $a3,$s3
    jal  bsort
bsort_ret:
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
    bne  $t0,$zero,bubble_ret

    # compare and swap if needed
    move $a0,$s0
    add  $a1,$s0,$s2
    jalr $s3
    blez $v0,no_swap

    lw   $t2,0($s0)
    lw   $t3,0($a1)
    sw   $t2,0($a1)
    sw   $t3,0($s0)
no_swap:
    # move to next element
    add  $s0,$s0,$s2
    addi $s1,$s1,-1
    move $a0,$s0
    move $a1,$s1
    move $a2,$s2
    move $a3,$s3
    jal  bubble
bubble_ret:
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

# ===== Compare Floats =====nfloat_cmp:
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