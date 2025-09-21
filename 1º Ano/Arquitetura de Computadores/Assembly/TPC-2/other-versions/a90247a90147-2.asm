# Diogo Alexandre Botas Carvalho - a90247
# Diogo Coelho Freitas - a90147
#	
# MIPS Assembly program that:
# Reads the file "dna.txt", with the format:
# name
# DNA sequence
# name
# DNA sequence
# ...etc.
#
# For each person, the program:
# 1. Reads and stores the name.
# 2. Prints the name to the console.
# 3. Reads and stores the associated DNA sequence.
# 4. Checks if the DNA sequence contains the predefined child's DNA (searchstring).
# 5. If a match is found:
#    - Prints ">>>> FOUND!"
#    - Stops processing the file.
# 6. If no match is found after reading the entire file:
#    - Prints "NOT FOUND!!!!"
#
# The program uses manual character-by-character reading,
# simple string handling (null-terminated strings),
# and a nested loop to simulate a strstr (substring search) function.
#
# ============================================================================= 

.data
filename:	.asciiz	"dna.txt"						# Name of the input file containing names and DNA sequences
searchstring:	.asciiz	"TGGGTGCGTTTGCCACACGCCTTTTATTTGTCTAGCCGTAACCAGAA"	# The child’s DNA string to search for
foundstring:	.asciiz	">>>> FOUND!"						# Message to print if a match is found
notfoundstring:	.asciiz	"NOT FOUND!!!!"						# Message to print if no match is found

char:	.byte 0		# 1-byte buffer for reading characters from the file
name:	.space 128	# Temporary space to store the player's name (up to 128 characters)
dna:	.space 512	# Temporary space to store the player's DNA sequence (up to 512 characters)

.text
main:

#################--Open the file for reading--#################
li $v0, 13		# Syscall 13 = open file
la $a0, filename	# Load address of filename
li $a1, 0		# Mode 0 = read-only
li $a2, 0		# Unused argument for read mode
syscall
move $s0, $v0		# Save the file descriptor in $s0 for later use

li $s1, 0		# Initialize a flag in $s1: 0 = not found, 1 = found

# === Main loop to process each person (name + DNA sequence) ===
read_next_person:
la $t0, name		# $t0 will point to where we store the current name

# --- Read one line into name ---
read_name_loop:
li $v0, 14		# Syscall 14 = read from file
move $a0, $s0		# File descriptor
la $a1, char		# Load address of 1-byte buffer
li $a2, 1		# Read 1 byte
syscall
beqz $v0, end		# If $v0 == 0, end-of-file -> exit loop

lb $t1, char		# Load read character into $t1
li $t2, 10		# ASCII code for newline ('\n')
beq $t1, $t2, name_done	# If newline found, stop reading name

sb $t1, 0($t0)		# Store character into name
addiu $t0, $t0, 1	# Advance pointer in name
j read_name_loop	# Repeat reading

name_done:
sb $zero, 0($t0)	# Null-terminate the name string for printing

# --- Print the player's name ---
li $v0, 4		# Syscall 4 = print string
la $a0, name		# Address of the name to print
syscall

# --- Print a newline after the name ---
li $v0, 11		# Syscall 11 = print character
li $a0, 10		# ASCII 10 = '\n'
syscall

# --- Read the DNA sequence ---
la $t0, dna		# $t0 will now point to the DNA buffer

read_dna_loop:
li $v0, 14		# Read 1 byte from file
move $a0, $s0
la $a1, char
li $a2, 1
syscall
beqz $v0, end		# EOF? Then exit

lb $t1, char		# Load the character
li $t2, 10		# Newline
beq $t1, $t2, dna_done	# If newline, end of DNA sequence

sb $t1, 0($t0)		# Store character into dna
addiu $t0, $t0, 1	# Advance buffer pointer
j read_dna_loop		# Continue reading

dna_done:
sb $zero, 0($t0)	# Null-terminate the DNA sequence

# === Perform a manual strstr (substring search) ===
la $t3, dna		# Outer pointer: starting position in DNA sequence

strstr_outer_loop:
lb $t4, 0($t3)			# Load character from DNA
beqz $t4, read_next_person	# End of DNA string? Try next person

move $t5, $t3			# $t5 = candidate position to start comparing
la $t6, searchstring		# Reset searchstring pointer

strstr_inner_loop:
lb $t7, 0($t5)			# Current DNA character
lb $t8, 0($t6)			# Current searchstring character

beqz $t8, match_found		# If searchstring ended -> full match found
beqz $t7, try_next_pos		# If DNA ends too soon -> no match
bne $t7, $t8, try_next_pos	# Mismatch? Try next position in DNA

addiu $t5, $t5, 1		# Advance DNA pointer
addiu $t6, $t6, 1		# Advance searchstring pointer
j strstr_inner_loop		# Keep comparing characters

try_next_pos:
addiu $t3, $t3, 1		# Move to next starting position in DNA
j strstr_outer_loop		# Try matching again from the new position

match_found:
li $s1, 1			# Set "found" flag to true
li $v0, 4
la $a0, foundstring		# Print FOUND message
syscall
j close_file			# Early exit: we found a match

# === End of file reached ===
end:
beqz $s1, print_notfoundstring	# If nothing found so far, print NOT FOUND
j close_file			# Otherwise, just close the file

# === If no match was found after processing all players ===
print_notfoundstring:
li $v0, 4
la $a0, notfoundstring
syscall

################--Close the file and exit the program--################

close_file:
li $v0, 16			# Syscall 16 = close file
move $a0, $s0			# Provide file descriptor
syscall

#################-Exit Program-#################
	
li $v0, 10	# Syscall 10 = exit
syscall

