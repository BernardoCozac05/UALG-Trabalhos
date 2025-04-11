# name: Diogo Alexandre Botas Carvalho, number: a90247
# name: Diogo Coelho Freitas, number: a90147

.data
filename: .asciiz "dna.txt"                 # Name of the input file containing player names and DNA sequences
searchstring: .asciiz "TGGGTGCGTTTGCCACACGCCTTTTATTTGTCTAGCCGTAACCAGAA"  # Child's DNA sequence to search for
found_msg: .asciiz ">>>> FOUND!"           # Message to display if a match is found
notfound_msg: .asciiz "NOT FOUND!!!!"      # Message to display if no match is found

char_buffer: .byte 0                       # 1-byte buffer to read the file character by character
name_buffer: .space 128                    # Buffer to temporarily store each player's name (one line)
dna_buffer: .space 512                     # Buffer to temporarily store each player's DNA sequence

.text
main:
    # === Open the file for reading ===
    li $v0, 13                  # Syscall 13 = open file
    la $a0, filename            # Load address of filename
    li $a1, 0                   # Open in read-only mode
    li $a2, 0                   # Unused for read
    syscall
    move $s0, $v0               # Save the file descriptor in $s0

    li $s1, 0                   # $s1 will act as a flag: 0 = not found, 1 = found

# === Main loop to read each player (name + DNA) ===
read_next_person:
    # --- Read a full line into name_buffer ---
    la $t0, name_buffer         # $t0 points to the current position in name_buffer

read_name_loop:
    li $v0, 14                  # Syscall 14 = read from file
    move $a0, $s0               # File descriptor
    la $a1, char_buffer         # Read one byte into char_buffer
    li $a2, 1                   # Number of bytes to read = 1
    syscall
    beqz $v0, end               # If $v0 == 0 ? end of file ? finish program
    lb $t1, char_buffer         # Load the read byte
    li $t2, 10                  # Linefeed character (ASCII 10) indicates end of line
    beq $t1, $t2, name_done     # If newline, finish reading name
    sb $t1, 0($t0)              # Store character in name_buffer
    addiu $t0, $t0, 1           # Move to next byte in name_buffer
    j read_name_loop            # Continue reading name

name_done:
    sb $zero, 0($t0)            # Null-terminate the name string (for printing)

    # --- Print the name ---
    li $v0, 4                   # Syscall 4 = print string
    la $a0, name_buffer         # Address of the name to print
    syscall

    # --- Print a newline after the name ---
    li $v0, 11                  # Syscall 11 = print character
    li $a0, 10                  # ASCII 10 = '\n'
    syscall

    # --- Read a full DNA sequence into dna_buffer (next line) ---
    la $t0, dna_buffer          # $t0 points to the current position in dna_buffer

read_dna_loop:
    li $v0, 14                  # Syscall 14 = read from file
    move $a0, $s0               # File descriptor
    la $a1, char_buffer         # Read one byte into char_buffer
    li $a2, 1                   # Read only 1 byte
    syscall
    beqz $v0, end               # End of file = finish
    lb $t1, char_buffer         # Load the character
    li $t2, 10                  # Check for newline
    beq $t1, $t2, dna_done      # If newline, finish reading sequence
    sb $t1, 0($t0)              # Store byte in dna_buffer
    addiu $t0, $t0, 1           # Advance to next byte in dna_buffer
    j read_dna_loop             # Continue reading DNA

dna_done:
    sb $zero, 0($t0)            # Null-terminate the DNA sequence

    # === Perform manual strstr(dna_buffer, searchstring) ===
    la $t3, dna_buffer          # Outer pointer to DNA sequence (starting position of candidate)

strstr_outer_loop:
    lb $t4, 0($t3)              # Load character from DNA
    beqz $t4, read_next_person  # If end of string, check next person

    move $t5, $t3               # Save current position to test from
    la $t6, searchstring        # Reset searchstring pointer

strstr_inner_loop:
    lb $t7, 0($t5)              # DNA character at current position
    lb $t8, 0($t6)              # Searchstring character at current position
    beqz $t8, match_found       # If reached end of searchstring ? match found
    beqz $t7, try_next_position # If DNA ends before match ? no match
    bne $t7, $t8, try_next_position # If mismatch ? try next starting point
    addiu $t5, $t5, 1           # Move to next DNA character
    addiu $t6, $t6, 1           # Move to next searchstring character
    j strstr_inner_loop         # Continue comparing

try_next_position:
    addiu $t3, $t3, 1           # Move outer pointer one position forward
    j strstr_outer_loop         # Try from the next character

match_found:
    li $s1, 1                   # Set found flag to true
    li $v0, 4
    la $a0, found_msg           # Print FOUND! message
    syscall
    j close_file                # Exit program early

# === End of file reached ===
end:
    beqz $s1, print_not_found   # If not found flag is still 0 ? print NOT FOUND
    j close_file

# === Print NOT FOUND message if no DNA matched ===
print_not_found:
    li $v0, 4
    la $a0, notfound_msg
    syscall

# === Close the file and exit ===
close_file:
    li $v0, 16                  # Syscall 16 = close file
    move $a0, $s0               # File descriptor
    syscall

    li $v0, 10                  # Syscall 10 = exit
    syscall
