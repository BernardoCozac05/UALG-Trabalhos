# TRABALHO 1 
# João Andréz - 61066
# Carlos Ferreira - 71319
# Diogo Freitas - 90147
# Diogo Carvalho - 90247

import math

#Array of possible characters in numbers
CHAR_LIST = [ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
              "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
              "U", "V", "W", "X", "Y", "Z", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n",
              "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"]

# 00 | "0"   |   10 | "A"   20 | "K"    30 | "U"    40 | "e"    50 | "o"    60 | "y"
# 01 | "1"   |   11 | "B"   21 | "L"    31 | "V"    41 | "f"    51 | "p"    61 | "z"
# 02 | "2"   |   12 | "C"   22 | "M"    32 | "W"    42 | "g"    52 | "q"
# 03 | "3"   |   13 | "D"   23 | "N"    33 | "X"    43 | "h"    53 | "r"
# 04 | "4"   |   14 | "E"   24 | "O"    34 | "Y"    44 | "i"    54 | "s"
# 05 | "5"   |   15 | "F"   25 | "P"    35 | "Z"    45 | "j"    55 | "t"
# 06 | "6"   |   16 | "G"   26 | "Q"    36 | "a"    46 | "k"    56 | "u"
# 07 | "7"   |   17 | "H"   27 | "R"    37 | "b"    47 | "l"    57 | "v"
# 08 | "8"   |   18 | "I"   28 | "S"    38 | "c"    48 | "m"    58 | "w"
# 09 | "9"   |   19 | "J"   29 | "T"    39 | "d"    49 | "n"    59 | "x"

#Fractional part precision
PRECISION = 15

# Calls toBase10 or toBaseX, depending on the value of new_base
def changeBase(num: str, current_base: int, new_base: int):
    if new_base == 10:
        return toBase10(num, current_base)
    else:
        return toBaseX(num, current_base, new_base)

# Convert a number from current_base to new_base, up to base 62
# Expects number with "." separating integer from fractional part
def toBaseX(num: str, current_base: int, new_base: int):
    new_num = ""

    if current_base != 10:                  # Check if number is already in base 10
        num = toBase10(num, current_base)   
    else:
        num = float(num)
    
    #-----INTEGER PART-----
    integer_part = math.floor(num)

    if integer_part == 0:
        new_num = "0"
    else:
        while integer_part != 0:
            digit = CHAR_LIST[integer_part % new_base]   # Get digit from division remainder with new_base
            new_num = digit + new_num                    # Add digit to the BEGINNING of the string
            integer_part //= new_base                    # Divide integer_part by the new_base
        
    #------FRACTIONAL PART-------
    fractional_part = num % 1       # Get fraction part with division remainder with 1
    if fractional_part != 0:        # Has a fractional part
        new_num += "."              # Add a "." to the end of the string
        
    i = 0   # Counter for fractional part precision
    while fractional_part != 0 and i < PRECISION:
        integer_remainder = math.floor(fractional_part * new_base)          # Integer remainder
        digit = CHAR_LIST[integer_remainder]                                 # Get digit from CHAR_LIST
        fractional_part = fractional_part * new_base - integer_remainder     # Set new fractional part
        new_num += digit                                                     # Add the digit to the END of the string
        i += 1                                                               # Increment counter for precision
            
    return new_num  # string

# Convert any number up to base 62 back to base 10
# Expects number with "." separating integer from fractional part
def toBase10(num: str, base: int):
    new_num = float(0)              # Float for storing following values
    num = num.split(".")            # Split the integer part from the fractional part in a string list
    
    #-----INTEGER PART-----
    integer_part = num[0]
    for i, digit in enumerate(integer_part):        # Add each character's value multiplied by base^position to the total float value
        new_num += CHAR_LIST.index(digit) * base ** (len(integer_part) - i - 1)
        
    #-----FRACTIONAL PART------
    if len(num) > 1:                # Check if there's a fractional part
        fractional_part = num[1]
        for i, digit in enumerate(fractional_part): # Add each character's value multiplied by base^position to the total float value
            new_num += CHAR_LIST.index(digit) * base ** ((i + 1) * -1)
            
    return new_num  # float

# Number validation;
# Returns true for numbers with characters that belong to the base.
def numberValidation(base: int, num: str):
    num = num.replace(".", "")
    for digit in num:
        if CHAR_LIST.index(digit) >= base:
            return False
    return True

# Basic input function;
# Loops through current base, number and new base;
# Checks if 2 < bases < 63 (length of CHAR_LIST)
# Checks if number's characters belong to the current base
def inputNumber():
    # Input loop
    while True:
        negative_number = False
        
        # CURRENT BASE
        current_base = int(input("Base Inicial: "))
        if current_base > len(CHAR_LIST) or current_base < 2:
            print(f"Base ->{current_base}<- não está entre 2 e {len(CHAR_LIST)}")
            continue
        
        # NUMBER
        number = input(f"Número a converter da base {current_base}: ")
        if number.find("-") != -1:               # Check for negative number, saves state in flag negative_number
            negative_number = True
            number = number.replace("-", "")
        number = number.replace(",", ".")   # Force "." to separate integer from fractional part
        if not numberValidation(current_base, number):
            print(f"Número ->{number}<- inválido na base {current_base}")
            continue
        
        # NEW BASE
        new_base = int(input("Base Nova: "))
        if new_base > len(CHAR_LIST) or new_base < 2:
            print(f"Base ->{new_base}<- não está entre 2 e {len(CHAR_LIST)}")
            continue
        
        # Heavy lifting function call
        new_num = changeBase(number, current_base, new_base)
        if negative_number:               
            new_num = "-" + new_num
        print(f"Número convertido na base {new_base}: {new_num}")
        
        # Loop if user doesn't input "N"
        if input(f"Pretende continuar? (Y/N): ") == "N":
            break

if __name__ == '__main__':
    inputNumber()
