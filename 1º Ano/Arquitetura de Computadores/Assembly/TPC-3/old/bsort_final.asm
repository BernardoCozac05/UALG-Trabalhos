##########################################
#  Bubble?sort demo — corrected version  #
#  Sorts an integer array in ascending   #
#  order and prints it before and after. #
##########################################

        .data
        .align 2                  # ensure correct alignment for both words and floats

myintsarray:      .word 8,7,32,8,6,11,9
myintsarray_end:

myfloatsarray:    .float 7.0,8.0,4.0,-13.1,67.3,9.7,4.1,-1.4
myfloatsarray_end:

##########################################
        .text
        .globl main
##########################################
##############--| Macros |--##############
##########################################

.macro print_int %r            # print integer in register %r
        move $a0,%r
        li   $v0,1
        syscall
.end_macro

.macro print_fp %p             # print float at address %p
        lwc1 $f12,0(%p)
        li   $v0,2
        syscall
.end_macro

.macro cnia %a %a_end %out     # Count number of ints in array: (%a .. %a_end) ? %out
        la   $t0,%a_end
        la   $t1,%a
        subu $t0,$t0,$t1        # total byte size
        srl  %out,$t0,2         # ÷4 ? element count
.end_macro

.macro PNL                     # print newline
        li $v0,11
        li $a0,10
        syscall
.end_macro

.macro PSPACE                  # print space
        li $v0,11
        li $a0,32
        syscall
.end_macro

.macro exit
        li $v0,10
        syscall
.end_macro

##########################################
##############--|  MAIN  |--##############
##########################################

main:
        # Print original array
        la   $a0,myintsarray
        cnia myintsarray,myintsarray_end,$a1   # $a1 ? element count
        li   $a2,0                             # index 0
        jal  printIntArray

        # Call generic bubble?sort
        la   $a0,myintsarray
        cnia myintsarray,myintsarray_end,$a1
        li   $a2,4                             # element size (bytes)
        la   $a3,cmp_int                       # comparator fn
        jal  bsort

        # Print sorted array
        la   $a0,myintsarray
        cnia myintsarray,myintsarray_end,$a1
        li   $a2,0
        jal  printIntArray

        exit

##########################################
##########--| PRINT INT ARRAY |--##########
##########################################

# Arguments:
#   $a0 – base address
#   $a1 – number of elements
#   $a2 – current index (starts at 0)
printIntArray:
        beq  $a2,$a1,printIntEnd              # finished?
        addi $sp,$sp,-16
        sw   $ra,12($sp)
        sw   $a0,8($sp)
        sw   $a1,4($sp)
        sw   $a2,0($sp)

        sll  $t0,$a2,2                        # offset = index * 4
        add  $t1,$a0,$t0
        lw   $a0,0($t1)
        li   $v0,1
        syscall
        PSPACE

        lw   $a0,8($sp)
        lw   $a1,4($sp)
        lw   $a2,0($sp)
        addi $a2,$a2,1
        jal  printIntArray

        lw   $ra,12($sp)
        addi $sp,$sp,16
        jr   $ra

printIntEnd:
        PNL
        jr   $ra

##########################################
##########--| INT COMPARATOR |--##########
##########################################

# int cmp_int(const void *lhs,const void *rhs)
# returns (lhs - rhs)
cmp_int:
        lw  $t0,0($a0)
        lw  $t1,0($a1)
        subu $v0,$t0,$t1                      # unsigned subtract avoids overflow trap
        jr  $ra

##########################################
##############--| GENERIC BSORT |--##############
##########################################
# Bubble?sort:  void bsort(void *base,int n,int size,int (*cmp)(void*,void*))
# Arguments:
#   $a0 – base address
#   $a1 – number of elements
#   $a2 – element size (bytes)
#   $a3 – comparator function pointer
bsort:
        li   $t0,1
        ble  $a1,$t0,bsort_end                # 0 or 1 element ? nothing to do

        addi $sp,$sp,-32
        sw   $ra,28($sp)
        sw   $a0,0($sp)
        sw   $a1,4($sp)
        sw   $a2,8($sp)
        sw   $a3,12($sp)

        li   $t4,0                            # swap flag
        li   $t1,0                            # i = 0

bsort_loop:
        lw   $t2,4($sp)
        addi $t2,$t2,-1                       # n?1
        bge  $t1,$t2,bsort_check_change

        lw   $t3,8($sp)                       # size
        mul  $t5,$t1,$t3                      # offset = i*size
        lw   $t6,0($sp)                       # base
        add  $t7,$t6,$t5                      # addr[i]
        add  $t8,$t7,$t3                      # addr[i+1]

        move $a0,$t7
        move $a1,$t8
        lw   $t9,12($sp)
        jalr $t9                              # call comparator

        blez $v0,bsort_no_swap               # already in order?

        lw   $t2,0($t7)                       # swap *addr[i] and *addr[i+1]
        lw   $t3,0($t8)
        sw   $t3,0($t7)
        sw   $t2,0($t8)

        li   $t4,1                            # mark change

bsort_no_swap:
        addi $t1,$t1,1
        j     bsort_loop

bsort_check_change:
        beqz $t4,bsort_restore               # no changes ? sorted

        lw   $a0,0($sp)                      # recurse with n?1
        lw   $a1,4($sp)
        addi $a1,$a1,-1
        lw   $a2,8($sp)
        lw   $a3,12($sp)
        jal  bsort

bsort_restore:
        lw   $ra,28($sp)
        addi $sp,$sp,32
bsort_end:
        jr   $ra
