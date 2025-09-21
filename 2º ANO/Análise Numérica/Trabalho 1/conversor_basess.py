from fractions import Fraction

# Mapping of characters to values for digits (0-9, A-Z, a-z)
digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"

# Read inputs
num_str = input("Número a converter: ").strip()
base_from = int(input("Base de origem (2-62): ").strip())
base_to = int(input("Base de destino (2-62): ").strip())

# Validate base ranges
if not (2 <= base_from <= 62 and 2 <= base_to <= 62):
    print("Erro: As bases de origem e destino devem estar entre 2 e 62.")
    exit()

# Handle optional sign
sign = 1
if num_str.startswith(('+', '-')):
    if num_str[0] == '-':
        sign = -1
    num_str = num_str[1:]

# Identify and handle decimal separator
if num_str.count('.') + num_str.count(',') > 1:
    print("Erro: Número inválido (mais de um separador decimal).")
    exit()
sep = '.' if '.' in num_str else (',' if ',' in num_str else '')
if sep:
    int_part_str = num_str.split(sep)[0] or '0'
    frac_part_str = num_str.split(sep)[1] if sep in num_str and num_str.split(sep)[1] != '' else ''
else:
    int_part_str = num_str or '0'
    frac_part_str = ''

# Validate that all digits are valid in the source base
for ch in int_part_str + frac_part_str:
    if ch not in digits or digits.index(ch) >= base_from:
        print(f"Erro: Dígito '{ch}' inválido na base {base_from}.")
        exit()

# Convert integer part from base_from to a numeric value
int_value = 0
for ch in int_part_str:
    int_value = int_value * base_from + digits.index(ch)

# Convert fractional part from base_from to a Fraction
frac_value = Fraction(0, 1)
if frac_part_str:
    numerator = 0
    for ch in frac_part_str:
        numerator = numerator * base_from + digits.index(ch)
    denominator = base_from ** len(frac_part_str)
    frac_value = Fraction(numerator, denominator)

# Convert integer value to target base (base_to)
if int_value == 0:
    int_result = "0"
else:
    int_result = ""
    value = int_value
    while value > 0:
        value, rem = divmod(value, base_to)
        int_result = digits[rem] + int_result

# Convert fractional value to target base (up to 8 digits)
frac_result = ""
fraction = frac_value
for _ in range(8):
    if fraction == 0:
        break
    fraction *= base_to
    digit = int(fraction)           # integer part of (fraction * base_to)
    frac_result += digits[digit]    # next digit in the fractional part
    fraction -= digit               # remove the integer part
# (If fraction is not zero here, it means the representation would continue; we truncate at 8 digits)

# Assemble the final result string
if frac_result:
    sep_out = sep if sep else '.'
    result = int_result + sep_out + frac_result
else:
    result = int_result
if sign == -1 and result != "0":
    result = "-" + result

# Print the converted number in the new base
print(f"Número em base {base_to}: {result}")
