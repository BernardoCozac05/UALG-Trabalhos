##########################################
###############--| DATA |--###############
##########################################
.data
#myintsarray:      .word 8,7,32,8,6,11,9
myintsarray:      .word 42, 7, 91, 13, 65, 28, 84, 3, 76, 39, 50, 12, 97, 6, 71, 24, 59, 11, 36, 88
myintsarray_end:

#myfloatsarray:    .float 7.0,8.0,4.0,-13.1,67.3,9.7,4.1,-1.4
myfloatsarray:    .float 9.81, -3.5, 0.99, 4.2, 7.77, -1.23, 12.6, 5.5, 3.14, 8.88, 11.11, 1.01, -7.4, 6.66, 2.71, 0.01, 10.5, -2.5, 13.37, 0.0
myfloatsarray_end:

##########################################
##########################################

.text

##########################################
##############--| Macros |--##############
##########################################

.macro cnia %a %a_end %out     # Count number in array: (%a to %a_end), result in %out
la   $t0, %a_end                # load end address into $t0
la   $t1, %a                    # load start address into $t1
subu $t0, $t0, $t1              # byte size = end - start
srl  %out, $t0, 2               # divide by 4 to get int count
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

##########################################
##########################################

##########################################
###############--| MAIN |--###############
##########################################

main:
la  $s0, myintsarray
la  $s1, myfloatsarray

    # Print original array
move   $a0, $s0
cnia myintsarray, myintsarray_end, $a1
li   $a2, 0
jal  printIntArray

    # Call bsort
move   $a0, $s0
    #li $a1, 7
cnia myintsarray, myintsarray_end, $a1
li $a2, 4
la $a3, cmp_int
jal bsort

    # Print sorted array
move   $a0, $s0
cnia myintsarray, myintsarray_end, $a1
li   $a2, 0
jal  printIntArray


    # Print original array
move   $a0, $s1
cnia myfloatsarray, myfloatsarray_end, $a1
li   $a2, 0
jal  printFloatArray

    # Call bsort
move   $a0, $s1
    #li $a1, 7
cnia myfloatsarray, myfloatsarray_end, $a1
li $a2, 4
la $a3, cmp_float
jal bsort

    # Print sorted array
move   $a0, $s1
cnia myfloatsarray, myfloatsarray_end, $a1
li   $a2, 0
jal  printFloatArray


exit
    
##########################################
##########################################

##########################################
##########--| PRINT INT ARRAY |--##########
##########################################

printIntArray:
beq  $a2, $a1, printIntEnd
addi $sp, $sp, -16
sw   $ra, 12($sp)
sw   $a0, 8($sp)
sw   $a1, 4($sp)
sw   $a2, 0($sp)

sll  $t0, $a2, 2
add  $t1, $a0, $t0
lw   $a0, 0($t1)
li   $v0, 1
syscall
PSPACE

lw   $a0, 8($sp)
lw   $a1, 4($sp)
lw   $a2, 0($sp)
addi $a2, $a2, 1
jal  printIntArray

lw   $ra, 12($sp)
addi $sp, $sp, 16
jr   $ra

printIntEnd:
PNL
jr   $ra



#############################################
##########--| PRINT FLOAT ARRAY |--##########
#############################################

# $a0 = endereço do array
# $a1 = tamanho do array
# $a2 = índice atual

printFloatArray:
beq  $a2, $a1, printFloatEnd

# Guardar contexto antes da chamada recursiva
addi $sp, $sp, -16
sw   $ra, 12($sp)
sw   $a0, 8($sp)
sw   $a1, 4($sp)
sw   $a2, 0($sp)

# imprimir arr[i]
sll  $t0, $a2, 2
add  $t1, $a0, $t0
lwc1 $f12, 0($t1)
li   $v0, 2
syscall

PSPACE

# restaurar parâmetros e preparar próxima chamada
lw   $a0, 8($sp)
lw   $a1, 4($sp)
lw   $a2, 0($sp)
addi $a2, $a2, 1
jal  printFloatArray

# restaurar ra e stack
lw   $ra, 12($sp)
addi $sp, $sp, 16
jr   $ra

printFloatEnd:
PNL
jr   $ra

#############################################
#############################################


#############################################
############--| COMPARADOR INT |--###########
#############################################

# -------------------------------------------
# int cmp_int(const void* p1,const void* p2)
# -------------------------------------------
cmp_int:
lw  $v0,0($a0)
lw  $t0,0($a1)
subu $v0,$v0,$t0
jr  $ra

#############################################
#############################################

###############################################
############--| COMPARADOR FLOAT |--###########
###############################################

#--------------------------
# int cmp_float(const void*,const void*)
# devolve -1/0/+1 tal como strcmp
#--------------------------
cmp_float:
lwc1 $f0,0($a0)
lwc1 $f1,0($a1)
c.lt.s $f0,$f1      # f0<f1 ?
bc1t cf_lt
c.lt.s $f1,$f0      # f1<f0 ?
bc1t cf_gt
li $v0,0            # iguais
jr $ra
cf_lt:
li $v0,-1
jr $ra
cf_gt:
li $v0,1
jr $ra

#############################################
#############################################


#############################################
################--| BSORT |--################
#############################################

bsort:
li   $t0, 1
ble  $a1, $t0, bsort_end

addi $sp, $sp, -32
sw   $ra, 28($sp)
sw   $a0, 0($sp)
sw   $a1, 4($sp)
sw   $a2, 8($sp)
sw   $a3, 12($sp)

li   $t4, 0
li   $t1, 0

bsort_loop:
lw   $t2, 4($sp)
addi $t2, $t2, -1
bge  $t1, $t2, bsort_check_change

lw   $t3, 8($sp)
mult $t1, $t3
mflo $t5
lw   $t6, 0($sp)
add  $t7, $t6, $t5
add  $t8, $t7, $t3

move $a0, $t7
move $a1, $t8
lw   $t9, 12($sp)
jalr $t9

blez $v0, bsort_no_swap

lw $t2, 0($t7)
lw $t3, 0($t8)
sw $t3, 0($t7)
sw $t2, 0($t8)

li $t4, 1

bsort_no_swap:
addi $t1, $t1, 1
j bsort_loop

bsort_check_change:
beqz $t4, bsort_restore

lw   $a0, 0($sp)
lw   $a1, 4($sp)
addi $a1, $a1, -1
lw   $a2, 8($sp)
lw   $a3, 12($sp)
jal  bsort

bsort_restore:
lw   $ra, 28($sp)
addi $sp, $sp, 32
bsort_end:
jr   $ra

#############################################
#############################################
