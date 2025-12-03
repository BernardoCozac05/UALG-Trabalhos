"""
Conversor de Bases (2..62)
--------------------------
• Converte qualquer número real (com sinal) entre bases 2..62.
• Algarismos: 0-9, A-Z (10–35), a-z (36–61).
• Entrada pode ter '.' ou ',' como separador.
• Saída usa sempre '.' como separador decimal.
• Precisão máxima: 8 casas decimais na parte fracionária.
"""

from fractions import Fraction

DIGITS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
DIGIT_TO_VAL = {ch: i for i, ch in enumerate(DIGITS)}


class BaseConversionError(ValueError):
    pass


def _split_number(s: str):
    """Divide número em (sinal, parte inteira, parte fracionária)."""
    s = s.strip()
    if not s:
        raise BaseConversionError("Entrada vazia.")

    sign = 1
    if s[0] in "+-":
        sign = -1 if s[0] == "-" else 1
        s = s[1:]

    s = s.replace(",", ".")
    if s.count(".") > 1:
        raise BaseConversionError("Número inválido: mais de um separador decimal.")

    if "." in s:
        int_part, frac_part = s.split(".", 1)
    else:
        int_part, frac_part = s, ""

    if not int_part:
        int_part = "0"

    return sign, int_part, frac_part


def _validate_digits(int_part, frac_part, base_from):
    for ch in int_part + frac_part:
        if ch not in DIGIT_TO_VAL:
            raise BaseConversionError(f"Caractere inválido: {ch}")
        if DIGIT_TO_VAL[ch] >= base_from:
            raise BaseConversionError(f"Dígito '{ch}' não existe na base {base_from}.")


def _int_to_value(int_part, base_from):
    value = 0
    for ch in int_part:
        value = value * base_from + DIGIT_TO_VAL[ch]
    return value


def _frac_to_fraction(frac_part, base_from):
    frac = Fraction(0, 1)
    base_power = base_from
    for ch in frac_part:
        frac += Fraction(DIGIT_TO_VAL[ch], base_power)
        base_power *= base_from
    return frac


def _value_to_base_int(n, base_to):
    if n == 0:
        return "0"
    out = []
    while n > 0:
        n, r = divmod(n, base_to)
        out.append(DIGITS[r])
    return "".join(reversed(out))


def _value_to_base_frac(frac, base_to, max_digits=8):
    if frac <= 0:
        return ""
    out = []
    f = frac
    for _ in range(max_digits):
        f *= base_to
        digit = int(f)
        out.append(DIGITS[digit])
        f -= digit
        if f == 0:
            break
    return "".join(out)


def changeBase(num: str, base_from: int, base_to: int, max_digits: int = 8) -> str:
    if not (2 <= base_from <= 62 and 2 <= base_to <= 62):
        raise BaseConversionError("Bases válidas: 2 a 62.")

    sign, int_part, frac_part = _split_number(num)
    _validate_digits(int_part, frac_part, base_from)

    int_val = _int_to_value(int_part, base_from)
    frac_val = _frac_to_fraction(frac_part, base_from)

    int_out = _value_to_base_int(int_val, base_to)
    frac_out = _value_to_base_frac(frac_val, base_to, max_digits)

    result = int_out + ("." + frac_out if frac_out else "")
    if sign < 0 and result != "0":
        result = "-" + result
    return result


# Interface simples para terminal
if __name__ == "__main__":
    print("=== Conversor de Bases (2..62) ===")
    num = input("Número: ").strip()
    b_from = int(input("Base de origem(2-62): ").strip())
    b_to = int(input("Base de destino(2-62): ").strip())
    print("Resultado:", changeBase(num, b_from, b_to))
