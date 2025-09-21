.data
give_num: .asciiz "Give a number:"
benfg: .asciiz "Benfica, o Glorioso!\n"

.text

main:
    # Print prompt for the number
    li $v0, 4
    la $a0, give_num
    syscall

    # Read integer input
    li $v0, 5
    syscall
    move $t0, $v0  # Store the number in $t0

    # Initialize counter
    li $t1, 0  # Counter = 0

print_loop:
    beq $t1, $t0, end_program  # If counter == number, exit

    # Print "Benfica"
    li $v0, 4
    la $a0, benfg
    syscall

    addi $t1, $t1, 1  # Increment counter
    j print_loop  # Repeat

end_program:
    li $v0, 10  # Exit program
    syscall
