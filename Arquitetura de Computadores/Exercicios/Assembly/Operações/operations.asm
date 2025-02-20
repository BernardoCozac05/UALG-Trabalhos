#Programa assamble no MIPS que pede dois numero e imprime a sua soma, diferença produto e quociente

.data
ask_n1: .asciiz "Enter first number: "
ask_n2: .asciiz "Enter second number: "
sum_msg: .asciiz "The sum is: "
diff_msg: .asciiz "The difference is: "
prod_msg: .asciiz "The product is: "
quot_msg: .asciiz "The quotient is: "
plus_sign: .asciiz " + "
slash_sign: .asciiz "/"
newl: .asciiz "\n" #New line

.text
.globl main

main:

    add $s0, $t0, $t1
    # Ask for first number
    li $v0, 4 #Prints a string
    la $a0, ask_n1 #define what string to print
    syscall #terminate call
    
    # Read the first integer
    li $v0, 5  #Reads an number
    syscall
    move $t0, $v0  #Save the number
    
    # Ask for second number
    li $v0, 4
    la $a0, ask_n2
    syscall
    
    # Read the second integer
    li $v0, 5
    syscall
    move $t1, $v0
    
    # Sum operation
    add $t2, $t0, $t1 #SUM of 2 numbers: output, input 1, input 2
    li $v0, 4
    la $a0, sum_msg
    syscall
    li $v0, 1 #Print a number
    move $a0, $t2 #Define the number to print
    syscall
    li $v0, 4
    la $a0, newl #Get to the next line(same as in C "\n")
    syscall
    
    # Difference operation
    sub $t2, $t0, $t1 #Diference of 2 numbers: output, input 1, input 2
    li $v0, 4
    la $a0, diff_msg
    syscall
    li $v0, 1
    move $a0, $t2
    syscall
    li $v0, 4
    la $a0, newl
    syscall
    
    # Product operation
    mul $t2, $t0, $t1 #Product of 2 numbers: output, input 1, input 2
    li $v0, 4
    la $a0, prod_msg
    syscall
    li $v0, 1
    move $a0, $t2
    syscall
    li $v0, 4
    la $a0, newl
    syscall
    
# Quotient operation
    beq $t1, $zero, end #Check if $t1 is 0
    div $t0, $t1 #Quotient of 2 numbers: output, input 1, input 2
    mflo $t2  # Quotient
    mfhi $t3  # Remainder
    
    li $v0, 4
    la $a0, quot_msg
    syscall
    li $v0, 1
    move $a0, $t2
    syscall
    
    # Print remainder only if it's non-zero
    beq $t3, $zero, skip_fraction
    
    # Print " + "
    li $v0, 4
    la $a0, plus_sign
    syscall
    
    # Print remainder
    li $v0, 1
    move $a0, $t3
    syscall
    
    # Print "/"
    li $v0, 4
    la $a0, slash_sign
    syscall
    
    # Print divisor
    li $v0, 1
    move $a0, $t1
    syscall
    
skip_fraction:
    li $v0, 4
    la $a0, newl
    syscall

end:
    # Exit program
    li $v0, 10
    syscall
