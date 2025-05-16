# nome: Diogo Alexandre Botas Carvalho, número: a90247
# nome: Diogo Coelho de Freitas, número: a90147
##########################################
#          Bubble-sort in MIPS           #
# -------------------------------------- #
# This program demonstrates a recursive  #
# bubble-sort that can operate on either #
# an integer or a floating-point array.  #
# A comparator function pointer is       #
# passed to the sorting routine so that  #
# the exact same algorithm can be reused #
# for different data types.              #
#                                        #
##########################################

##########################################
###############--| DATA |--###############
##########################################
.data
# =====================================================	#
# Integer and floating-point test arrays.		#
# The “_end” labels are sentinel addresses used only	#
# to compute the number of elements — they do *not*	#
# contain data themselves.				#
# =====================================================	#
myintsarray: .word 8 7 32 8 6 11 9
myintsarray_end:

myfloatsarray: .float 7.0 8.0 4.0 -13.1 67.3 9.7 4.1 -1.4
myfloatsarray_end:

##########################################
##########################################

.text

##########################################
##############--| Macros |--##############
##########################################

# ============================================ #	
#        cnia  (Count Number In Array)         #
# ============================================ #
# Calculates the number of 32-bit elements     #
# in the half-open interval  [a, a_end).       #
# Parameters (pseudo-register names):          #
#   %a      – start label / address            #
#   %a_end  – end   label / address            #
#   %out    – destination register for         #
#              the element count               #
# Usage: cnia myarray, myarray_end, $a1	       #
# ============================================ #
.macro cnia %a %a_end %out	# Count number in array
la $t0, %a_end			# load end address into $t0
la $t1, %a			# load start address into $t1
subu $t0, $t0, $t1		# byte size = end - start
srl  %out, $t0, 2		# divide by 4 ? element count
.end_macro

# ------------------ #
# Convenience macros #
# ------------------ #
.macro PNL	# print newline (\n)
li $v0,11
li $a0,10
syscall
.end_macro

.macro PSPACE	# print space (" ")
li $v0,11
li $a0,32
syscall
.end_macro

.macro exit	# terminate program
li $v0,10
syscall
.end_macro

##########################################
##########################################

##########################################
###############--| MAIN |--###############
##########################################

main:
la $s0,myintsarray     # $s0 = &int_array[0]
la $s1,myfloatsarray   # $s1 = &float_array[0]

################################################
# 1.  INTEGER ARRAY: print ? sort ? print again
################################################

# ------- INT array : print original -------
move $a0,$s0
cnia myintsarray,myintsarray_end,$a1
li $a2,0
jal printIntArray

# ------- INT array : sort in?place -------
move $a0,$s0           # base
cnia myintsarray,myintsarray_end,$a1
li $a2,4               # stride = 4 bytes
la $a3,cmp_int         # comparator
jal bsort

# ------- INT array : print sorted   -------
move $a0,$s0
cnia myintsarray,myintsarray_end,$a1
li $a2,0
jal printIntArray

################################################
# 2.  FLOAT ARRAY: print ? sort ? print again
################################################

# ------- FLOAT array : print original -----
move $a0,$s1
cnia myfloatsarray,myfloatsarray_end,$a1
li $a2,0
jal printFloatArray

# ------- FLOAT array : sort in?place  -----
move $a0,$s1
cnia myfloatsarray,myfloatsarray_end,$a1
li $a2,4
la $a3,cmp_float
jal bsort

# ------- FLOAT array : print sorted   -----
move $a0,$s1
cnia myfloatsarray,myfloatsarray_end,$a1
li $a2,0
jal printFloatArray

exit

##########################################
##########################################

##########################################
##########--| PRINT INT ARRAY |--##########
##########################################
# Recursively prints array elements separated by a space.
# Parameters: $a0=base, $a1=len, $a2=i
printIntArray:
beq  $a2, $a1, printIntEnd	# done?

# Save caller-saved context (RA + arguments) for recursion
addi $sp, $sp, -16
sw   $ra, 12($sp)
sw   $a0, 8($sp)
sw   $a1, 4($sp)
sw   $a2, 0($sp)

# load arr[i] and print it as integer
sll  $t0, $a2, 2	# offset = i * 4
add  $t1, $a0, $t0	# &arr[i]
lw   $a0, 0($t1)
li   $v0, 1		# print_int syscall
syscall
PSPACE

# Recursive call with i+1
lw   $a0, 8($sp)
lw   $a1, 4($sp)
lw   $a2, 0($sp)
addi $a2, $a2, 1
jal  printIntArray

# Restore and return
lw   $ra, 12($sp)
addi $sp, $sp, 16
jr   $ra

printIntEnd:
PNL
jr   $ra

#############################################
##########--| PRINT FLOAT ARRAY |--##########
#############################################
# Same logic as printIntArray but using lwc1.
printFloatArray:
beq  $a2, $a1, printFloatEnd

# Save context for recursion
addi $sp, $sp, -16
sw $ra, 12($sp)
sw $a0, 8($sp)
sw $a1, 4($sp)
sw $a2, 0($sp)

# print arr[i] as float
sll $t0, $a2, 2
add $t1, $a0, $t0
lwc1 $f12, 0($t1)
li $v0, 2		# print_float syscall
syscall
PSPACE

# Recursive call with i+1
lw $a0, 8($sp)
lw $a1, 4($sp)
lw $a2, 0($sp)
addi $a2, $a2, 1
jal  printFloatArray

# Restore and return
lw $ra, 12($sp)
addi $sp, $sp, 16
jr $ra

printFloatEnd:
PNL
jr $ra

#############################################
#############################################

#############################################
############--| COMPARADOR INT |--###########
#############################################
# =============================================	#	
# int cmp_int(const void *p1, const void *p2)	#
# ============================================= #	
# Returns <0, 0, >0 according to (*p1 ? *p2).	#
# In : $a0 = ptr to int				#
#      $a1 = ptr to int				#
# Out: $v0 = arithmetic difference		#
# =============================================	#
cmp_int:
lw  $v0,0($a0)		# *p1 in $v0
lw  $t0,0($a1)		# *p2 in $t0
subu $v0,$v0,$t0	# difference
jr  $ra

#############################################
#############################################

###############################################
############--| COMPARADOR FLOAT |--###########
###############################################
# =============================================	#	
# int cmp_float(const void *p1, const void *p2)	#
# ============================================= #	
# return -1 if *p1 < *p2			#
# return  0 if *p1 == *p2			#
# return +1 if *p1 > *p2			#
# In : $a0=$f0_ptr, $a1=$f1_ptr			#
# Out: $v0 = -1 / 0 / +1			#
# =============================================	#	
cmp_float:
lwc1 $f0,0($a0)		# *p1
lwc1 $f1,0($a1)		# *p2
c.lt.s $f0,$f1		# if f0 < f1 ? branch true
bc1t cf_lt
c.lt.s $f1,$f0		# if f1 < f0 ? branch true
bc1t cf_gt
li $v0,0		# equal
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
# =====================================================	#
# void bsort(void *base, int len, int stride, cmp_f cmp)#
# -----------------------------------------------------	#
# Recursive bubble-sort implementation.			#
# In : 							#
# $a0 = base address (byte pointer)			#
# $a1 = len  (element count)				#
# $a2 = stride (bytes per element)			#
# $a3 = comparator function pointer			#
# Out: — (array sorted in?place)			#
# =====================================================	#
bsort:
li $t0,1
ble $a1,$t0,bsort_end		# len ?1 ? already sorted
addi $sp,$sp,-32
sw $ra,28($sp)
sw $a0,0($sp)
sw $a1,4($sp)
sw $a2,8($sp)
sw $a3,12($sp)

li $t4,0			# swap_occurred flag
li $t1,0			# i index

bsort_loop:
lw $t2,4($sp)			# len
addi $t2,$t2,-1			# len-1 (last comparable index)
bge $t1,$t2,bsort_check_change

lw $t3,8($sp)			# stride
mult $t1,$t3
mflo $t5			# i*stride
lw $t6,0($sp)			# base
add $t7,$t6,$t5			# ptrA
add $t8,$t7,$t3			# ptrB = ptrA+stride

move $a0,$t7
move $a1,$t8
lw $t9,12($sp)			# cmp ptr
jalr $t9			# v0 = cmp(A,B)

blez $v0,bsort_no_swap		# A ? B ? continue

# ---- swap *ptrA and *ptrB ----
lw $t2,0($t7)
lw $t3,0($t8)
sw $t3,0($t7)
sw $t2,0($t8)
li $t4,1

bsort_no_swap:
addi $t1,$t1,1
j bsort_loop

bsort_check_change:
beqz $t4,bsort_restore		# no swap ? already sorted

# recurse with len?1
lw $a0,0($sp)
lw $a1,4($sp)
addi $a1,$a1,-1
lw $a2,8($sp)
lw $a3,12($sp)
jal bsort

bsort_restore:
lw $ra,28($sp)
addi $sp,$sp,32
bsort_end:
jr $ra

#############################################
#############################################