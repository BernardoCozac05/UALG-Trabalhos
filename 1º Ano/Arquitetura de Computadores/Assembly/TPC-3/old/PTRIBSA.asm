##########################################
###############--| DATA |--###############
##########################################
.data
buffer:         .space 1024
myintsarray:    .space 400
myfloatsarray:  .float 9.81, -3.5, 0.99, 4.2, 7.77, -1.23, 12.6, 5.5, 3.14, 8.88, 11.11, 1.01, -7.4, 6.66, 2.71, 0.01, 10.5, -2.5, 13.37, 0.0
myfloatsarray_end:

##########################################
##########################################

.text

##########################################
##############--| Macros |--##############
##########################################

.macro print_int %r
move $a0, %r
li $v0, 1
syscall
.end_macro

.macro print_fp %p
lwc1 $f12, 0(%p)
li $v0, 2
syscall
.end_macro

.macro cnia %a %a_end %out
la $t0, %a_end
la $t1, %a
subu $t0, $t0, $t1
srl  %out, $t0, 2
.end_macro

.macro PNL
li $v0, 11
li $a0, 10
syscall
.end_macro

.macro PSPACE
li $v0, 11
li $a0, 32
syscall
.end_macro

.macro exit
li $v0, 10
syscall
.end_macro

##########################################
##########################################

##########################################
###############--| MAIN |--###############
##########################################

main:
# Ler linha de inteiros
li $v0, 8
la $a0, buffer
li $a1, 1024
syscall

# Parse Inteiros
la $a0, buffer
la $a1, myintsarray
jal parseIntegers
move $s0, $v0

# Ordenar e imprimir inteiros
la $a0, myintsarray
move $a1, $s0
jal bubbleSortInt

la $a0, myintsarray
move $a1, $s0
li $a2, 0
jal printIntArray

# Ordenar e imprimir floats
la $a0, myfloatsarray
cnia myfloatsarray, myfloatsarray_end, $a1
li $a2, 0
jal printFloatArray

la $a0, myfloatsarray
cnia myfloatsarray, myfloatsarray_end, $a1
jal bubbleSortFloat

la $a0, myfloatsarray
cnia myfloatsarray, myfloatsarray_end, $a1
li $a2, 0
jal printFloatArray

exit
    
##########################################
##########################################

###########################################
#########--| Manual Parse Ints |--#########
###########################################

parseIntegers:
li $t0, 0      # contador
li $t1, 0      # acumulador
li $t4, 0      # sinal

parse_int_loop:
lb $t2, 0($a0)
beqz $t2, end_parse
li $t3, 32
beq $t2, $t3, store_int
li $t3, 45
beq $t2, $t3, mark_negative
li $t3, 48
li $t5, 57
blt $t2, $t3, next_char
bgt $t2, $t5, next_char
mul $t1, $t1, 10
sub $t2, $t2, 48
add $t1, $t1, $t2
j next_char

mark_negative:
li $t4, 1
j next_char

store_int:
beq $t4, 0, store_pos
sub $t1, $zero, $t1
store_pos:
sw $t1, 0($a1)
addiu $a1, $a1, 4
li $t1, 0
li $t4, 0
addi $t0, $t0, 1
next_char:
addiu $a0, $a0, 1
j parse_int_loop

end_parse:
beq $t1, $zero, done_parse
beq $t4, $zero, store_final
sub $t1, $zero, $t1
store_final:
sw $t1, 0($a1)
addi $t0, $t0, 1
done_parse:
move $v0, $t0
jr $ra

##########################################
##########################################

###########################################
##########--| Bubble Sort Int |--##########
###########################################

bubbleSortInt:
addi $sp, $sp, -8
sw $ra, 4($sp)
sw $a1, 0($sp)
li $t0, 1
beq $a1, $t0, bubbleIntEnd
li $t1, 0

bubbleIntLoop:
sub $t2, $a1, $t1
addi $t2, $t2, -2
bltz $t2, bubbleIntRecurse
sll $t3, $t1, 2
add $t4, $a0, $t3
lw $t5, 0($t4)
lw $t6, 4($t4)
ble $t5, $t6, skipSwapInt
sw $t6, 0($t4)
sw $t5, 4($t4)
skipSwapInt:
addi $t1, $t1, 1
j bubbleIntLoop

bubbleIntRecurse:
lw $a1, 0($sp)
addi $a1, $a1, -1
jal bubbleSortInt

bubbleIntEnd:
lw $ra, 4($sp)
addi $sp, $sp, 8
jr $ra

##########################################
##########################################

###########################################
##########--| Print Int Array |--##########
###########################################

printIntArray:
beq $a2, $a1, printIntEnd
addi $sp, $sp, -16
sw $ra, 12($sp)
sw $a0, 8($sp)
sw $a1, 4($sp)
sw $a2, 0($sp)
sll $t0, $a2, 2
add $t1, $a0, $t0
lw $a0, 0($t1)
li $v0, 1
syscall
PSPACE
lw $a0, 8($sp)
lw $a1, 4($sp)
lw $a2, 0($sp)
addi $a2, $a2, 1
jal printIntArray
lw $ra, 12($sp)
addi $sp, $sp, 16
jr $ra

printIntEnd:
PNL
jr $ra

##########################################
##########################################

###########################################
#########--| Bubble Sort Float |--#########
###########################################

bubbleSortFloat:
addi $sp, $sp, -8
sw $ra, 4($sp)
sw $a1, 0($sp)
li $t0, 1
beq $a1, $t0, bubbleFloatEnd
li $t1, 0

bubbleFloatLoop:
sub $t2, $a1, $t1
addi $t2, $t2, -2
bltz $t2, bubbleFloatRecurse
sll $t3, $t1, 2
add $t4, $a0, $t3
lwc1 $f0, 0($t4)
lwc1 $f1, 4($t4)
c.le.s $f0, $f1
bc1t skipSwapFloat
swc1 $f1, 0($t4)
swc1 $f0, 4($t4)
skipSwapFloat:
addi $t1, $t1, 1
j bubbleFloatLoop

bubbleFloatRecurse:
lw $a1, 0($sp)
addi $a1, $a1, -1
jal bubbleSortFloat

bubbleFloatEnd:
lw $ra, 4($sp)
addi $sp, $sp, 8
jr $ra

##########################################
##########################################

###########################################
#########--| Print Float Array |--#########
###########################################

printFloatArray:
beq $a2, $a1, printFloatEnd
addi $sp, $sp, -16
sw $ra, 12($sp)
sw $a0, 8($sp)
sw $a1, 4($sp)
sw $a2, 0($sp)
sll $t0, $a2, 2
add $t1, $a0, $t0
lwc1 $f12, 0($t1)
li $v0, 2
syscall
PSPACE
lw $a0, 8($sp)
lw $a1, 4($sp)
lw $a2, 0($sp)
addi $a2, $a2, 1
jal printFloatArray
lw $ra, 12($sp)
addi $sp, $sp, 16
jr $ra

printFloatEnd:
PNL
jr $ra
    
##########################################
##########################################
