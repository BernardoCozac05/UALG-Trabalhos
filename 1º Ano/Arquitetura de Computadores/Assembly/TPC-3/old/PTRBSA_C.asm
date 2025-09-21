
##########################################
###############--| DATA |--###############
##########################################
.data
buffer:		.space 1024
buffer2:	.space 1024
floatBuffer:	.space 64
myintsarray:	.space 400
myfloatsarray:	.space 400

##########################################
##########################################

.text

##########################################
##############--| Macros |--##############
##########################################

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
# Ler inteiros
li $v0, 8
la $a0, buffer
li $a1, 1024
syscall

la $a0, buffer
la $a1, myintsarray
jal parseIntegers
move $s0, $v0

la $a0, myintsarray
move $a1, $s0
move $t0, $s0
beq $t0, $zero, skip_int_sort

jal bubbleSortInt
skip_int_sort:

la $a0, myintsarray
move $a1, $s0
li $a2, 0
jal printIntArray

# Ler floats
li $v0, 8
la $a0, buffer2
li $a1, 1024
syscall

la $a0, buffer2
la $a1, myfloatsarray
jal parseFloatsManual
move $s1, $v0

la $a0, myfloatsarray
move $a1, $s1
move $t1, $s1
beq $t1, $zero, skip_float_sort

jal bubbleSortFloat
skip_float_sort:

la $a0, myfloatsarray
move $a1, $s1
li $a2, 0
jal printFloatArray

exit

###########################################
###########################################

###########################################
#########--| Manual Parse Ints |--#########
###########################################

parseIntegers:
li $t0, 0
li $t1, 0
li $t4, 0
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

###########################################
###########################################

###########################################
########--| Manual Parse Floats |--########
###########################################

parseFloatsManual:
li $t0, 0
move $s1, $a0

next_float_token:
lb $t1, 0($s1)
beqz $t1, end_parse_floats
la $t2, floatBuffer

extract_loop:
lb $t3, 0($s1)
beqz $t3, end_extract
li $t4, 32
beq $t3, $t4, end_extract
sb $t3, 0($t2)
addiu $t2, $t2, 1
addiu $s1, $s1, 1
j extract_loop

end_extract:
sb $zero, 0($t2)
beq $t3, $zero, skip_space
addiu $s1, $s1, 1

skip_space:
la $t5, floatBuffer
li $t6, 0      # parte inteira
li $t7, 0      # parte decimal
li $t8, 0      # número de casas
li $t9, 0      # modo decimal
li $s0, 0      # negativo?
lb $t2, 0($t5)
beq $t2, 45, mark_negativef
j convert_loop

mark_negativef:
li $s0, 1
addiu $t5, $t5, 1

convert_loop:
lb $t2, 0($t5)
beqz $t2, finish_convert
li $t3, 46
beq $t2, $t3, switch_to_decimal
li $t3, 48
li $t4, 57
blt $t2, $t3, next_digit
bgt $t2, $t4, next_digit
sub $t2, $t2, 48
beq $t9, 0, to_integer_part
mul $t7, $t7, 10
add $t7, $t7, $t2
addiu $t8, $t8, 1
j next_digit

to_integer_part:
mul $t6, $t6, 10
add $t6, $t6, $t2
j next_digit

switch_to_decimal:
li $t9, 1

next_digit:
addiu $t5, $t5, 1
j convert_loop

finish_convert:
mtc1 $t6, $f2
cvt.s.w $f2, $f2
beq $t8, $zero, store_float_final
li $t5, 1
move $t3, $t8

pow10_loop:
li $t4, 10
mul $t5, $t5, $t4
addi $t3, $t3, -1
bgtz $t3, pow10_loop
mtc1 $t7, $f3
cvt.s.w $f3, $f3
mtc1 $t5, $f4
cvt.s.w $f4, $f4
div.s $f3, $f3, $f4
add.s $f2, $f2, $f3

store_float_final:
beq $s0, 0, store_positive
neg.s $f2, $f2

store_positive:
swc1 $f2, 0($a1)
addiu $a1, $a1, 4
addiu $t0, $t0, 1
j next_float_token

end_parse_floats:
move $v0, $t0
jr $ra

###########################################
###########################################

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

###########################################
###########################################

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

###########################################
###########################################

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

###########################################
###########################################

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

###########################################
###########################################
