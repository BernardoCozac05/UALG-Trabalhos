#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from fractions import Fraction

# -----------------------------
# Mapeamentos de dígitos
# -----------------------------
DIGITS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
VAL = {ch: i for i, ch in enumerate(DIGITS)}

def char_to_val(ch: str) -> int:
    if ch not in VAL:
        raise ValueError(f"Carácter inválido: {ch!r}")
    return VAL[ch]

def val_to_char(v: int) -> str:
    if not (0 <= v < 62):
        raise ValueError(f"Valor de dígito fora do intervalo [0, 61]: {v}")
    return DIGITS[v]

# -----------------------------
# Validação e parsing
# -----------------------------
def validar_e_normalizar(num_str: str, base: int) -> tuple[str, str, str, int]:
    """
    Retorna (sign, int_part, frac_part, base) após validar que
    todos os dígitos pertencem à base dada.
    """
    if not (2 <= base <= 62):
        raise ValueError("A base deve estar entre 2 e 62.")

    s = num_str.strip()
    if not s:
        raise ValueError("Número vazio.")

    sign = ""
    if s[0] in "+-":
        sign, s = s[0], s[1:]
    if sign == "":
        sign = "+"

    if not s or s == ".":
        raise ValueError("Formato inválido de número.")

    # separa partes
    if "." in s:
        int_part, frac_part = s.split(".", 1)
    else:
        int_part, frac_part = s, ""

    # aceita '' como 0 na parte inteira/fracionária
    if int_part == "":
        int_part = "0"

    # valida dígitos
    for ch in int_part:
        if ch == "_":  # ignore underscores opcionais (se quiser permitir)
            continue
        v = char_to_val(ch)
        if v >= base:
            raise ValueError(f"Dígito {ch!r} não pertence à base {base}.")
    for ch in frac_part:
        if ch == "_":
            continue
        v = char_to_val(ch)
        if v >= base:
            raise ValueError(f"Dígito {ch!r} não pertence à base {base}.")

    return sign, int_part, frac_part, base

# -----------------------------
# Conversão: base -> Fração (base 10)
# -----------------------------
def base_to_fraction(int_part: str, frac_part: str, base: int) -> Fraction:
    # parte inteira
    n = 0
    for ch in int_part:
        if ch == "_":
            continue
        n = n * base + char_to_val(ch)

    # parte fracionária
    f = Fraction(0, 1)
    denom_power = 1
    for ch in frac_part:
        if ch == "_":
            continue
        denom_power *= base
        f += Fraction(char_to_val(ch), denom_power)

    return Fraction(n, 1) + f

# -----------------------------
# Conversão: Fração -> destino
# -----------------------------
def fraction_to_base(fr: Fraction, base: int, precision: int = 8) -> tuple[str, str]:
    # parte inteira
    n = int(fr)  # floor para fr >= 0
    frac = fr - n

    # converte inteira
    if n == 0:
        int_digits = "0"
    else:
        digs = []
        while n > 0:
            n, r = divmod(n, base)
            digs.append(val_to_char(r))
        int_digits = "".join(reversed(digs))

    # converte fracionária (truncando até precision)
    if frac == 0:
        frac_digits = ""
    else:
        digs = []
        for _ in range(precision):
            frac *= base
            d = int(frac)
            digs.append(val_to_char(d))
            frac -= d
            if frac == 0:
                break
        frac_digits = "".join(digs)

    return int_digits, frac_digits

# -----------------------------
# API principal
# -----------------------------
def converter_base(num_str: str, base_origem: int, base_destino: int, precision: int = 8) -> str:
    """
    Converte num_str (na base_origem) para uma string na base_destino.
    precision: nº máx. de casas fracionárias (truncamento).
    """
    sign, int_part, frac_part, b = validar_e_normalizar(num_str, base_origem)

    # trata sinal separadamente usando valor absoluto
    fr = base_to_fraction(int_part, frac_part, b)
    if sign == "-":
        fr = -fr

    # zero?
    if fr == 0:
        return "0"

    # trabalha com valor absoluto, recoloca sinal no fim
    neg = fr < 0
    fr_abs = -fr if neg else fr

    int_digits, frac_digits = fraction_to_base(fr_abs, base_destino, precision)

    out = ("-" if neg else "") + int_digits
    if frac_digits:
        out += "." + frac_digits
    return out

# -----------------------------
# Exemplos rápidos de uso
# -----------------------------
if __name__ == "__main__":
    num = input("Número: ")
    b1 = int(input("Base atual (2-62): "))
    b2 = int(input("Nova base (2-62): "))
    try:
        res = converter_base(num, b1, b2, precision=8)
        print(f"{num} (base {b1}) -> (base {b2}) = {res}")
    except Exception as e:
        print("Erro:", e)

