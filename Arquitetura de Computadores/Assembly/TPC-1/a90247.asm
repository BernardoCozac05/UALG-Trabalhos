# Diogo Alexandre Botas Carvalho - a90247					#
#										#
# S(a,b,c,d) = ?(n=a...b) ?(m=c...d) [(-1)^n * (n - m)^2]			#
#										#
# obs: The Character ? might not load, its the letter Sigma, represents the sum	#
#										#
# MIPS Assembly program that:							#
#										#
# Reads four integers a, b, c, d from the user, and calculates S(a,b,c,d)	#
#										#
# ============================================================================= #

.data
promptA: .asciiz "Enter a: "			# Prompt to request 'a'
promptB: .asciiz "Enter b: "			# Prompt to request 'b'
promptC: .asciiz "Enter c: "			# Prompt to request 'c'
promptD: .asciiz "Enter d: "			# Prompt to request 'd'
msgRes:  .asciiz "\nThe Series Sum is: "	# Message before printing the final result

.text

# --------------------------------------------------------------------- #
# main:									#
#									#
#  - Prints prompts and reads a, b, c, d from the user.			#
#									#
#  - Initializes sum = 0 in register $s4.				#
#									#
#  - If (a > b) or (c > d), prints sum=0 immediately.			#
#									#
#  - Otherwise, loops over n = a...b and m = c...d to accumulate:	#
#	sum += [(-1)^n * (n - m)^2]					#
#									#
#  - Finally prints sum and exits.					#
# --------------------------------------------------------------------- #

main:
# Print "Enter a: " and read integer => store in $s0
la $a0, promptA		# Load address of prompt "Enter a: "
li $v0, 4		# Syscall to print string
syscall
li $v0, 5		# Syscall to read integer
syscall
move $s0, $v0		# $s0 <- a, Stores "a" in $s0

# Print "Enter b: " and read integer => store in $s1
la $a0, promptB		# Load address of prompt "Enter b: "
li $v0, 4		# Syscall to print string
syscall
li $v0, 5		# Syscall to read integer
syscall
move $s1, $v0		# $s1 <- b, Stores "b" in $s1

# Print "Enter c: " and read integer => store in $s2
la $a0, promptC		# Load address of prompt "Enter c: "
li $v0, 4		# Syscall to print string
syscall
li $v0, 5		# Syscall to read integer
syscall
move $s2, $v0		# $s2 <- c, Stores "c" in $s2

# Print "Enter d: " and read integer => store in $s3
la $a0, promptD		# Load address of prompt "Enter d: "
li $v0, 4		# Syscall to print string
syscall
li $v0, 5		# Syscall to read integer
syscall
move $s3, $v0		# $s3 <- d, Stores "d" in $s3

# Initialize sum = 0 in $s4
li $s4, 0

# -------------------------------------------------------------	#
# If a > b or c > d, jump to print sum=0 (empty sum convention)	#
# -------------------------------------------------------------	#
bgt $s0, $s1, printResult	# If a > b, go print sum = 0
bgt $s2, $s3, printResult	# If c > d, go print sum = 0

# -----------------------------------------------------	#
#  Otherwise, compute S(a,b,c,d) with two nested loops	#
#	for n = a..b and for m = c..d:			#
#	sum += [(-1)^n * (n - m)^2]			#
# -----------------------------------------------------	#
move $t0, $s0	# $t0 will loop n from a..b

loop_n:
  bgt $t0, $s1, printResult	# if n > b, exit the outer loop

  move $t1, $s2			# $t1 will loop m from c..d
loop_m:
  bgt $t1, $s3, end_loop_m	# if m > d, exit the inner loop

  # Determine (-1)^n:
  # if n is odd => factor = -1
  # if n is even => factor = +1
  andi $t2, $t0, 1		# check the least significant bit of n
  # andi $t2, $t0, 1 bitwises AND of $t0 with 1
  # If n is even, its binary form ends in 0, so (n & 1) == 0.
  # If n is odd, its binary form ends in 1, so (n & 1) == 1.
  beq $t2, $zero, n_is_even	# checks if $t2 equals 0, if thats the case n is even and we skip to "n_is_even", otherwise n is odd, so we load -1 in $t2
  li $t2, -1			# n is odd
  j calc_expr			# Jump to "calc_expr"
n_is_even:
  li $t2, 1			# n is even

calc_expr:
  sub $t3, $t0, $t1		# t3 = (n - m)
  mul $t4, $t3, $t3		# t4 = (n - m)^2
  mul $t4, $t4, $t2		# t4 = t4 * factor = [(-1)^n * (n - m)^2]
  add $s4, $s4, $t4		# sum += t4

  addi $t1, $t1, 1		# m++
  j loop_m			# Jump to "loop_m"

end_loop_m:
  addi $t0, $t0, 1		# n++
  j loop_n			# Jump to "loop_n"

# -------------------------------------	#
# Print the final sum in $s4 and exit	#
# ------------------------------------- #
printResult:
la $a0, msgRes		# Load address of "\nResult: "
li $v0, 4		# Print string syscall
syscall

move $a0, $s4		# Move sum into $a0
li $v0, 1		# Print integer syscall
syscall

# Print a newline (ASCII 10) after the result (This is just to end the program with a line break)
li $a0, 10	# Loads the 10 ASCII control character, linefeed(LF)
li $v0, 11	# Print the newline
syscall

# Syscall to exit program
li $v0, 10
syscall
