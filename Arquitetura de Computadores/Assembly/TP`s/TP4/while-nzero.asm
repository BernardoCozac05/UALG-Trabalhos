.data
gnum: .asciiz "Give a number: "

.text

main:
rst: #Repeat string "give number"
# Print prompt to ask for the number
li $v0, 4
la $a0, gnum
syscall

# Read the number
li $v0, 5
syscall
move $t0, $v0

#While the number is not 0, repeat
bne $t0, $zero, rst

#Terminate program
end_program:
li $v0, 10
syscall