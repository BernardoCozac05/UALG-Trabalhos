from fractions import Fraction

CHAR_TO_VAL = {**{str(i): i for i in range(10)}, **{chr(ord('A')+i): 10+i for i in range(26)},**{chr(ord('a')+i): 36+i for i in range(26)}}

VAL_TO_CHAR = {v: k for k, v in CHAR_TO_VAL.items()}

def parse_input_number(num_str):
    num_str = num_str.strip()
    sign = 1
    if num_str.startswith(('+', '-')):
        if num_str[0] == '-':
            sign = -1
        num_str = num_str[1:]

    sep_char = None
    if '.' in num_str and ',' in num_str:
        raise ValueError("Número inválido: contém ambos '.' e ',' como separador.")
    elif '.' in num_str:
        sep_char = '.'
    elif ',' in num_str:
        sep_char = ','

    if sep_char:
        int_part_str, frac_part_str = num_str.split(sep_char)
    else:
        int_part_str = num_str
        frac_part_str = ''

    if int_part_str == '':
        int_part_str = '0'
    if frac_part_str is None or frac_part_str == '':
        frac_part_str = ''
    return sign, int_part_str, frac_part_str, sep_char

def validate_digits(int_part_str, frac_part_str, base):
    max_value = base - 1
    for part_str in (int_part_str, frac_part_str):
        for ch in part_str:
            if ch not in CHAR_TO_VAL:
                raise ValueError(f"Dígito inválido '{ch}' encontrado.")
            val = CHAR_TO_VAL[ch]
            if val > max_value:
                raise ValueError(f"Dígito '{ch}' (valor {val}) não é válido na base {base}.")
    return True

def convert_int_part_to_value(int_part_str, base):
    value = 0
    for ch in int_part_str:
        val = CHAR_TO_VAL[ch]
        value = value * base + val
    return value

def convert_frac_part_to_fraction(frac_part_str, base):
    if frac_part_str == '':
        return Fraction(0, 1)
    numerator = 0
    for ch in frac_part_str:
        val = CHAR_TO_VAL[ch]
        numerator = numerator * base + val
    denominator = base ** len(frac_part_str)
    return Fraction(numerator, denominator)

def convert_value_to_base_int(value, base):
    if value == 0:
        return '0'
    digits = []
    while value > 0:
        digit_val = value % base
        value //= base
        digits.append(VAL_TO_CHAR[digit_val])
    digits.reverse()
    return ''.join(digits)

def convert_value_to_base_frac(frac_fraction, base, max_digits=8):
    if frac_fraction.numerator == 0:
        return ''
    result_digits = []
    frac = frac_fraction
    for _ in range(max_digits):
        frac *= base
        digit_val = int(frac)
        result_digits.append(VAL_TO_CHAR[digit_val])
        frac -= digit_val
        if frac == 0:
            break
    return ''.join(result_digits)

def main():
    try:
        num_str = input("Número a converter: ").strip()
        base_origem = int(input("Base de origem (2-62): ").strip())
        base_destino = int(input("Base de destino (2-62): ").strip())

        if not (2 <= base_origem <= 62 and 2 <= base_destino <= 62):
            raise ValueError("As bases de origem e destino devem estar entre 2 e 62.")

        sign, int_part_str, frac_part_str, sep_char = parse_input_number(num_str)
        validate_digits(int_part_str, frac_part_str, base_origem)

        int_value = convert_int_part_to_value(int_part_str, base_origem)
        frac_fraction = convert_frac_part_to_fraction(frac_part_str, base_origem)

        int_result_str = convert_value_to_base_int(int_value, base_destino)
        frac_result_str = convert_value_to_base_frac(frac_fraction, base_destino, max_digits=8)

        if sep_char is None:
            sep_char = '.'
        if frac_result_str:
            converted_number = int_result_str + sep_char + frac_result_str
        else:
            converted_number = int_result_str

        if sign < 0 and converted_number != "0":
            converted_number = "-" + converted_number

        print(f"Número em base {base_destino}: {converted_number}")

    except Exception as e:
        print(f"Erro: {e}")

if __name__ == "__main__":
    main()
