#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Unified Base Converter
----------------------
Estilo simples/"humano" inspirado no main.py, mas com a robustez e precisão do conversor_bases3.py.

• Suporta bases 2..62 (0-9, A-Z, a-z).
• Aceita sinal (+/-) e separador decimal '.' ou ',' na entrada.
• Valida dígitos por base com mensagens claras.
• Converte frações com precisão exata (fractions.Fraction), limitando a saída a no máx. N dígitos (padrão 8).
• Preserva o separador de entrada, a não ser que você force outro via parâmetro.
• Sem prints de debug e pensado para uso como biblioteca ou CLI simples.

Uso via código:
    from unified_base_converter import changeBase
    print(changeBase("1011.01", 2, 10))  # -> '11.25'

Uso via terminal:
    python unified_base_converter.py
"""

from fractions import Fraction

# Dígitos permitidos (base até 62)
DIGITS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
DIGIT_TO_VAL = {ch: i for i, ch in enumerate(DIGITS)}


class BaseConversionError(ValueError):
    """Erros claros de conversão/validação."""


def _normalize_input(num_str: str) -> str:
    if not isinstance(num_str, str):
        raise BaseConversionError("O número de entrada deve ser string.")
    s = num_str.strip()
    if s == "":
        raise BaseConversionError("Entrada vazia.")
    return s


def _parse(num_str: str):
    """
    Retorna: (sign, int_part_str, frac_part_str, sep_char)
    sign = +1 ou -1
    int_part_str (sem sinal), frac_part_str (pode ser "")
    sep_char = '.' | ',' | None
    """
    s = _normalize_input(num_str)

    # Sinal
    sign = 1
    if s[0] in "+-":
        sign = -1 if s[0] == "-" else 1
        s = s[1:]
        if s == "":
            raise BaseConversionError("Sinal sem número.")

    # Separador
    dot = s.count(".")
    comma = s.count(",")
    if dot + comma > 1:
        raise BaseConversionError("Use apenas um separador decimal ('.' ou ',').")

    sep_char = None
    if dot == 1 and comma == 0:
        sep_char = "."
    elif comma == 1 and dot == 0:
        sep_char = ","

    if sep_char is None:
        int_part_str = s
        frac_part_str = ""
    else:
        int_part_str, frac_part_str = s.split(sep_char, 1)

    # Normalizar partes vazias
    if int_part_str == "":
        int_part_str = "0"
    if frac_part_str is None:
        frac_part_str = ""

    return sign, int_part_str, frac_part_str, sep_char


def _validate_digits(int_part: str, frac_part: str, base_from: int):
    if not (2 <= base_from <= 62):
        raise BaseConversionError("Base de origem fora do intervalo suportado (2..62).")

    def check_chars(part: str, which: str):
        for ch in part:
            if ch == "_":  # permitir '_' visual? Aqui não: manter estrito.
                raise BaseConversionError("Caracter '_' não é permitido.")
            if ch not in DIGIT_TO_VAL:
                raise BaseConversionError(f"Caractere inválido '{ch}'. Use apenas 0-9, A-Z, a-z.")
            val = DIGIT_TO_VAL[ch]
            if val >= base_from:
                raise BaseConversionError(
                    f"Dígito '{ch}' não existe na base {base_from}."
                )

    if int_part != "":
        check_chars(int_part, "inteira")
    if frac_part != "":
        check_chars(frac_part, "fracionária")


def _int_part_to_value(int_part: str, base_from: int) -> int:
    """Converte parte inteira (string) para inteiro base-10."""
    n = 0
    for ch in int_part:
        n = n * base_from + DIGIT_TO_VAL[ch]
    return n


def _frac_part_to_fraction(frac_part: str, base_from: int) -> Fraction:
    """Converte parte fracionária (string) para Fraction em base-10."""
    frac = Fraction(0, 1)
    power = base_from
    for ch in frac_part:
        frac += Fraction(DIGIT_TO_VAL[ch], power)
        power *= base_from
    return frac


def _value_to_base_int(n: int, base_to: int) -> str:
    """Converte inteiro base-10 para string na base destino."""
    if n == 0:
        return "0"
    out = []
    x = n
    while x > 0:
        x, r = divmod(x, base_to)
        out.append(DIGITS[r])
    return "".join(reversed(out))


def _value_to_base_frac(frac: Fraction, base_to: int, max_digits: int) -> str:
    """
    Converte fração (0<= frac <1) para parte fracionária na base destino.
    Para quando:
      • resto fica zero (fração exata), ou
      • atinge max_digits.
    """
    if frac <= 0:
        return ""
    out = []
    f = frac
    for _ in range(max_digits):
        f *= base_to
        digit = int(f)  # parte inteira
        out.append(DIGITS[digit])
        f -= digit
        if f == 0:
            break
    return "".join(out).rstrip()  # não há espaços, apenas robustez


def changeBase(num: str, base_from: int, base_to: int, *, max_digits: int = 8, sep: str = "auto") -> str:
    """
    Converte número 'num' da base 'base_from' para 'base_to'.
    - max_digits: máximo de dígitos na parte fracionária de saída (padrão 8).
    - sep: 'auto' (preserva '.' ou ',' da entrada; se não houver, usa '.'),
           ou forçar '.' ou ','.

    Ex.: changeBase("FF.A", 16, 2)  -> '11111111.101'
    """
    if not (2 <= base_to <= 62):
        raise BaseConversionError("Base de destino fora do intervalo suportado (2..62).")
    if max_digits < 0:
        raise BaseConversionError("max_digits deve ser >= 0.")
    if sep not in ("auto", ".", ","):
        raise BaseConversionError("Parâmetro 'sep' deve ser 'auto', '.' ou ','.")

    sign, int_part_str, frac_part_str, sep_in = _parse(num)
    _validate_digits(int_part_str, frac_part_str, base_from)

    # Para base 10 interna: inteiro + fração exata
    int_value = _int_part_to_value(int_part_str, base_from)
    frac_value = _frac_part_to_fraction(frac_part_str, base_from)

    # Converter para base destino
    int_out = _value_to_base_int(int_value, base_to)
    frac_out = _value_to_base_frac(frac_value, base_to, max_digits=max_digits)

    # Montar separador
    if sep == "auto":
        sep_out = sep_in if sep_in in (".", ",") else "."
    else:
        sep_out = sep

    # Montar resultado
    if frac_out:
        result = f"{int_out}{sep_out}{frac_out}"
    else:
        result = int_out

    if sign < 0 and result != "0":
        result = "-" + result

    return result


def _cli():
    print("=== Conversor de Bases (2..62) ===")
    print("Dígitos: 0-9, A-Z, a-z")
    try:
        num = input("Número: ").strip()
        base_from = int(input("Base de origem (2..62): ").strip())
        base_to = int(input("Base de destino (2..62): ").strip())
        prefer = input("Separador de saída ['auto', '.', ','] (Enter=auto): ").strip() or "auto"
        md = input("Máximo de dígitos fracionários (Enter=8): ").strip()
        maxd = int(md) if md else 8
        out = changeBase(num, base_from, base_to, max_digits=maxd, sep=prefer)
        print("Resultado:", out)
    except BaseConversionError as e:
        print("Erro:", e)
    except Exception as e:
        print("Erro inesperado:", e)


if __name__ == "__main__":
    _cli()
