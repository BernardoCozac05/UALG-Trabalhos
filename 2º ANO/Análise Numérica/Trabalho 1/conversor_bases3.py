from fractions import Fraction

# Mapeamento global de caractere -> valor numérico
CHAR_TO_VAL = {**{str(i): i for i in range(10)}, **{chr(ord('A')+i): 10+i for i in range(26)},**{chr(ord('a')+i): 36+i for i in range(26)}}

# Mapeamento inverso de valor numérico -> caractere (para saída)
VAL_TO_CHAR = {v: k for k, v in CHAR_TO_VAL.items()}

def parse_input_number(num_str):
    num_str = num_str.strip()  # remove espaços em branco nas extremidades
    sign = 1
    # Verifica sinal
    if num_str.startswith(('+', '-')):
        if num_str[0] == '-':
            sign = -1
        num_str = num_str[1:]  # remove o sinal da string para facilitar

    # Identifica separador decimal se presente
    sep_char = None
    if '.' in num_str and ',' in num_str:
        # Se por acaso contiver ambos '.' e ',', é inválido ou ambíguo
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

    # Trata casos especiais: se nada antes ou depois do separador
    if int_part_str == '':
        int_part_str = '0'
    if frac_part_str is None or frac_part_str == '':
        frac_part_str = ''
    return sign, int_part_str, frac_part_str, sep_char

def validate_digits(int_part_str, frac_part_str, base):
    max_value = base - 1
    # Determina o caractere máximo permitido na base (por exemplo, base 16 -> 'F' ou base 36 -> 'Z', base 62 -> 'z')
    # Itera sobre todos os dígitos das duas partes
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
        # Acumula valor na base de origem
        value = value * base + val
    return value

def convert_frac_part_to_fraction(frac_part_str, base):
    if frac_part_str == '':
        # Sem parte fracionária, podemos representar como 0/1
        return Fraction(0, 1)
    numerator = 0
    for ch in frac_part_str:
        val = CHAR_TO_VAL[ch]
        numerator = numerator * base + val
    # Denominador é base^len(frac_part_str)
    denominator = base ** len(frac_part_str)
    return Fraction(numerator, denominator)  # Fraction simplifica automaticamente

def convert_value_to_base_int(value, base):
    if value == 0:
        return '0'
    digits = []
    # Realiza divisões sucessivas
    while value > 0:
        digit_val = value % base
        value //= base
        digits.append(VAL_TO_CHAR[digit_val])
    # Como acumulamos do menos significativo para o mais significativo, invertamos:
    digits.reverse()
    return ''.join(digits)

def convert_value_to_base_frac(frac_fraction, base, max_digits=8):
    if frac_fraction.numerator == 0:
        return ''  # não há parte fracionária
    result_digits = []
    frac = frac_fraction  # copia Fraction para uso (mantemos numerator/denom)
    for _ in range(max_digits):
        # Multiplica a fração pela base de destino
        frac *= base
        # Parte inteira do resultado multiplicado (próximo dígito na base de destino)
        digit_val = int(frac)  # isso equivale a frac.numerator // frac.denominator
        result_digits.append(VAL_TO_CHAR[digit_val])
        # Atualiza a fração restante (parte fracionária após extrair o dígito)
        frac -= digit_val  # equivale a frac = Fraction(frac.numerator % frac.denominator, frac.denominator)
        if frac == 0:
            break  # representação exata; encerra antes de atingir max_digits
    return ''.join(result_digits)

def main():
    try:
        # Solicita entradas do usuário
        num_str = input("Número a converter: ").strip()
        base_origem = int(input("Base de origem (2-62): ").strip())
        base_destino = int(input("Base de destino (2-62): ").strip())

        # Valida bases
        if not (2 <= base_origem <= 62 and 2 <= base_destino <= 62):
            raise ValueError("As bases de origem e destino devem estar entre 2 e 62.")

        # Parsing do número de entrada
        sign, int_part_str, frac_part_str, sep_char = parse_input_number(num_str)
        # Validação dos dígitos conforme a base de origem
        validate_digits(int_part_str, frac_part_str, base_origem)

        # Conversão da parte inteira e fracionária da base de origem para valores numéricos
        int_value = convert_int_part_to_value(int_part_str, base_origem)
        frac_fraction = convert_frac_part_to_fraction(frac_part_str, base_origem)

        # Converte esses valores para a base de destino
        int_result_str = convert_value_to_base_int(int_value, base_destino)
        frac_result_str = convert_value_to_base_frac(frac_fraction, base_destino, max_digits=8)

        # Remonta o resultado formatado
        # Usa o mesmo separador da entrada; se nenhum fornecido, adota '.' por padrão quando há parte fracionária.
        if sep_char is None:
            sep_char = '.'
        if frac_result_str:
            converted_number = int_result_str + sep_char + frac_result_str
        else:
            converted_number = int_result_str  # sem parte fracionária no resultado

        # Reinserir sinal se necessário
        if sign < 0 and converted_number != "0":
            converted_number = "-" + converted_number

        # Exibe o resultado
        print(f"Número em base {base_destino}: {converted_number}")

    except Exception as e:
        # Em caso de erro (por exemplo, entrada inválida), exibe a mensagem de exceção
        print(f"Erro: {e}")

# Executa o programa principal se este script for executado diretamente
if __name__ == "__main__":
    main()
