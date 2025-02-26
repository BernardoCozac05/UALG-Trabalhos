.data
ask_n1: .asciiz "Enter first number: "
ask_n2: .asciiz "Enter second number: "
equal_msg: .asciiz "Numbers are equal\n"
nequal_msg: .asciiz "Numbers are different\n"

.text

main:

# Ask for first number
li $v0, 4            # Print string syscall
la $a0, ask_n1       # Load address of ask_n1
syscall              # Call syscall

# Read the first integer
li $v0, 5            # Read integer syscall
syscall
move $t0, $v0        # Store number in $t0

# Ask for second number
li $v0, 4
la $a0, ask_n2
syscall

# Read the second integer
li $v0, 5
syscall
move $t1, $v0

# Compare numbers
beq $t0, $t1, numbers_equal  # If $t0 == $t1, go to numbers_equal

numbers_nequal:
# Print "Numbers are different"
li $v0, 4    # Print string syscall
la $a0, nequal_msg  
syscall
j end        # Jump to end

numbers_equal:
# Print "Numbers are equal"
li $v0, 4    # Print string syscall
la $a0, equal_msg  
syscall

end:
li $v0, 10   # Exit syscall
syscall
