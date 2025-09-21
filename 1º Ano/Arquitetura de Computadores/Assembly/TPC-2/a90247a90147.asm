# nome: Diogo Alexandre Botas Carvalho, número: a90247
# nome: Diogo Coelho de Freitas, número: a90147
#
# ======================== MIPS Assembly DNA Paternity Checker ========================	#
#											#
# ---------------------------------- Program Purpose ----------------------------------	#
# This MIPS Assembly program reads a file named "dna.txt" containing alternating lines	#
# of names and corresponding DNA sequences. The program searches for a predefined	#
# child’s DNA substring (stored in "searchstring") within each DNA sequence.		#
#											#
# The process for each person is as follows:						#
# 1. Read and store the person's name.							#
# 2. Print the name to the console.							#
# 3. Read and store the person's DNA sequence.						#
# 4. Check if the DNA sequence contains the child's DNA using a manual substring search.#
# 5. If a match is found:								#
#    - Print ">>>> FOUND!"								#
#    - Terminate the program.								#
# 6. If the entire file is read with no matches:					#
#    - Print "NOT FOUND!!!!"								#
#											#
# The program performs a character-by-character file reading using a 1-byte		#
# buffer ("char"). Names and DNA strings are stored as null-terminated sequences.	#
# A nested loop manually implements the functionality of the "strstr" function from C.	#
#											#
# ------------------------------------ File Format ------------------------------------	#
# dna.txt format:									#
#   name										#
#   DNA sequence									#
#   name										#
#   DNA sequence									#
#   ...	etc.										#
#											#
# Each name is followed by its DNA sequence.						#
#											#
# --------------------------------- Key Memory Areas ---------------------------------- #
# - filename: Name of the file to read							#
# - searchstring: DNA sequence to find							#
# - foundstring: Message for successful match						#
# - notfoundstring: Message for no match						#
# - char: 1-byte buffer for reading							#
# - name: Buffer to store names								#
# - dna: Buffer to store DNA sequences							#
#											#
# ---------------------------------- Core Registers -----------------------------------	#
# - $s0: File descriptor (holds the result of opening the file)				#
# - $s1: Match flag (0 = not found, 1 = found)						#
# - $t0-$t8: Temporaries used for reading strings, comparisons, and indexing		#
#										       	#
# =====================================================================================	#

.data
filename:       .asciiz "dna.txt"						# Input file name
searchstring:   .asciiz "TGGGTGCGTTTGCCACACGCCTTTTATTTGTCTAGCCGTAACCAGAA"	# Child's DNA sequence to search
foundstring:    .asciiz ">>>> FOUND!"						# Message to print if match found
notfoundstring: .asciiz "NOT FOUND!!!!"						# Message to print if no match found

char:   .byte 0		# 1-byte buffer for file reading
name:   .space 128	# Buffer to store name string
dna:    .space 512	# Buffer to store DNA sequence

.text
main:
# ================================== Open the file ====================================	#
li $v0, 13			# Load syscall code for 'open file'
la $a0, filename		# Load address of file name string
li $a1, 0			# File mode 0 = read-only
li $a2, 0			# Mode argument unused for read
syscall				# Perform syscall to open file
move $s0, $v0			# Save file descriptor returned in $v0 to $s0
				# ($s0 now holds the open file handle)

li $s1, 0			# Initialize match flag to 0 (not found)

# =========================== Read next person (name + DNA) ===========================	#
read_next_person:
la $t0, name			# $t0 points to the start of 'name' buffer

# ---------------------------------- Read name line -----------------------------------	#
read_name_loop:
li $v0, 14			# Load syscall code for 'read from file'
move $a0, $s0			# File descriptor in $a0
la $a1, char			# Address of 1-byte input buffer
li $a2, 1			# Number of bytes to read = 1
syscall				# Perform read syscall
beqz $v0, end			# If $v0 == 0, EOF reached ? go to end

lb $t1, char			# Load read byte (character) from buffer to $t1
li $t2, 10			# ASCII value of newline ('\n')
beq $t1, $t2, name_done		# If newline, stop reading name

sb $t1, 0($t0)			# Store character into name buffer
addiu $t0, $t0, 1		# Move to next position in name buffer
j read_name_loop		# Repeat loop to read next character

name_done:
sb $zero, 0($t0)		# Null-terminate the name string (store 0)

# ------------------------------------ Print name -------------------------------------	#
li $v0, 4			# Syscall to print a string
la $a0, name			# Address of the null-terminated name string
syscall

# ----------------------------- Print newline after name ------------------------------	#
li $v0, 11			# Syscall to print a character
li $a0, 10			# ASCII code for newline '\n'
syscall

# ----------------------------------- Read DNA line -----------------------------------	#
la $t0, dna			# $t0 points to the start of 'dna' buffer
read_dna_loop:
li $v0, 14			# Read syscall
move $a0, $s0			# File descriptor
la $a1, char			# Buffer to read 1 byte into
li $a2, 1			# Read 1 byte
syscall
beqz $v0, end			# If read returns 0 ? EOF

lb $t1, char			# Load character read into $t1
li $t2, 10			# Newline character
beq $t1, $t2, dna_done		# End of line ? finish DNA reading

sb $t1, 0($t0)			# Store read character into dna buffer
addiu $t0, $t0, 1		# Move to next position in dna buffer
j read_dna_loop			# Repeat loop

dna_done:
sb $zero, 0($t0)		# Null-terminate DNA string with 0

# ========================= Manual strstr (substring search) ==========================	#
la $t3, dna			# $t3 = pointer to start of DNA string

strstr_outer_loop:
lb $t4, 0($t3)			# Load current character from DNA
beqz $t4, read_next_person	# If end of string, go read next person

move $t5, $t3			# $t5 = candidate starting position in DNA
la $t6, searchstring		# $t6 = pointer to start of child's DNA (searchstring)

strstr_inner_loop:
lb $t7, 0($t5)			# Current character from DNA
lb $t8, 0($t6)			# Current character from searchstring

beqz $t8, match_found		# End of searchstring reached ? match found
beqz $t7, try_next_pos		# End of DNA reached before match ? fail
bne $t7, $t8, try_next_pos	# Characters don't match ? try next position

addiu $t5, $t5, 1		# Advance DNA pointer
addiu $t6, $t6, 1		# Advance searchstring pointer
j strstr_inner_loop		# Continue comparing next characters

try_next_pos:
addiu $t3, $t3, 1		# Move to next character in DNA for new attempt
j strstr_outer_loop		# Restart search from new DNA position

match_found:
li $s1, 1			# Set match flag to 1 (match was found)
li $v0, 4			# Syscall to print string
la $a0, foundstring		# Message to print: ">>>> FOUND!"
syscall
j close_file			# Exit early since match was found

# ==================================== End of file ====================================	#
end:
beqz $s1, print_notfoundstring	# If match flag still 0, print "NOT FOUND!!!!"
j close_file			# Otherwise, just close file

print_notfoundstring:
li $v0, 4			# Syscall to print string
la $a0, notfoundstring		# Message: "NOT FOUND!!!!"
syscall

# ============================== Close the file and exit ==============================	#
close_file:
li $v0, 16			# Syscall code to close file
move $a0, $s0			# Provide file descriptor to close
syscall

li $v0, 10			# Syscall code to exit program
syscall
