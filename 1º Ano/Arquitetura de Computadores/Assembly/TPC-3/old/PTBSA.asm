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

.macro print_int %r         # print integer in register %r
move $a0,%r
li   $v0,1
syscall
.end_macro

.macro print_fp %p          # print float at address %p
lwc1 $f12,0(%p)
li $v0,2
syscall
.end_macro

.macro cnia %a %a_end %out     # Count number of ints in array: (%a to %a_end), result in %out
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

#cnia myintsarray, myintsarray_end, $s0
#cnia myfloatsarray, myfloatsarray_end, $s1

# --- Print original array ---
la   $a0, myintsarray     # $a0 = &array
cnia myintsarray, myintsarray_end, $a1  # $a1 = length
li   $a2, 0            # $a2 = start index
jal  printIntArray     # print original array

# --- Sort the array ---
la   $a0, myintsarray     # $a0 = &array
cnia myintsarray, myintsarray_end, $a1     # $a1 = length
jal  bubbleSortInt     # sort array

# --- Print sorted array ---
la   $a0, myintsarray     # $a0 = &array
cnia myintsarray, myintsarray_end, $a1   # $a1 = length
li   $a2, 0            # $a2 = start index
jal  printIntArray     # print sorted array
    
#------------------------------------------------------------

 # --- Print original array ---
la   $a0, myfloatsarray  # Load address of float array
cnia myfloatsarray, myfloatsarray_end, $a1# Size
li   $a2, 0           # Start index = 0
jal  printFloatArray
    
 # --- Sort the array ---
la   $a0, myfloatsarray  # Load address of float array
cnia myfloatsarray, myfloatsarray_end, $a1 # Size of array
jal  bubbleSortFloat  # Call recursive bubble sort for floats
    
 # --- Print sorted array ---   
la   $a0, myfloatsarray  # Load address of float array
cnia myfloatsarray, myfloatsarray_end, $a1  # Size
li   $a2, 0           # Start index = 0
jal  printFloatArray



# Exit program
exit

##########################################
##########################################

##########################################
##########--| BUBLE SORT INT |--##########
##########################################

# $a0 = endereço do array
# $a1 = n (tamanho atual)

bubbleSortInt:
addi $sp, $sp, -8
sw   $ra, 4($sp)
sw   $a1, 0($sp)

li   $t0, 1
beq  $a1, $t0, bubbleIntEnd

li   $t1, 0  # j = 0

bubbleIntLoop:
sub  $t2, $a1, $t1
addi $t2, $t2, -2
bltz $t2, bubbleIntRecurse

sll  $t3, $t1, 2
add  $t4, $a0, $t3     # addr = &arr[j]
lw   $t5, 0($t4)       # arr[j]
lw   $t6, 4($t4)       # arr[j+1]

ble  $t5, $t6, skipSwapInt

sw   $t6, 0($t4)
sw   $t5, 4($t4)

skipSwapInt:
addi $t1, $t1, 1
j bubbleIntLoop

bubbleIntRecurse:
lw   $a1, 0($sp)
addi $a1, $a1, -1
jal  bubbleSortInt

bubbleIntEnd:
lw   $ra, 4($sp)
addi $sp, $sp, 8
jr   $ra
#############################################
#############################################
    
###########################################
##########--| PRINT INT ARRAY |--##########
###########################################

# $a0 = endereço do array
# $a1 = tamanho do array
# $a2 = índice atual

printIntArray:
# Caso base: se índice == tamanho, termina e imprime newline
beq  $a2, $a1, printIntEnd

# Guardar contexto antes da chamada recursiva
addi $sp, $sp, -16
sw   $ra, 12($sp)
sw   $a0, 8($sp)
sw   $a1, 4($sp)
sw   $a2, 0($sp)

# imprimir arr[i]
sll  $t0, $a2, 2
add  $t1, $a0, $t0
lw   $a0, 0($t1)
li   $v0, 1
syscall

PSPACE

# restaurar parâmetros e preparar próxima chamada
lw   $a0, 8($sp)
lw   $a1, 4($sp)
lw   $a2, 0($sp)
addi $a2, $a2, 1
jal  printIntArray

# restaurar ra e stack
lw   $ra, 12($sp)
addi $sp, $sp, 16
jr   $ra

printIntEnd:
PNL
jr   $ra
    
#############################################
#############################################


#############################################
###########--| BUBLE SORT FLOAT |--##########
#############################################



# $f12 = endereço do array (convertido para inteiro em $a0)
# $a1 = n

bubbleSortFloat:
addi $sp, $sp, -8
sw   $ra, 4($sp)
sw   $a1, 0($sp)

li   $t0, 1
beq  $a1, $t0, bubbleFloatEnd

li   $t1, 0

bubbleFloatLoop:
sub  $t2, $a1, $t1
addi $t2, $t2, -2
bltz $t2, bubbleFloatRecurse

sll  $t3, $t1, 2
add  $t4, $a0, $t3       # endereço arr[j]
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
lw   $a1, 0($sp)
addi $a1, $a1, -1
jal  bubbleSortFloat

bubbleFloatEnd:
lw   $ra, 4($sp)
addi $sp, $sp, 8
jr   $ra
    
#############################################
#############################################


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


