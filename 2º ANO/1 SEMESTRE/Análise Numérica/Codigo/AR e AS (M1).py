def count_relevant(xbar_str):
    s = xbar_str.strip()
    if s and (s[0] == '+' or s[0] == '-'):
        s = s[1:]

    digits = ""
    for ch in s:
        if '0' <= ch <= '9':
            digits += ch

    i = 0
    while i < len(digits) and digits[i] == '0':
        i += 1

    if i == len(digits):
        return 1
    return len(digits) - i

def int_digits(x):
    if x < 0:
        x = -x
    n = int(x)
    if n == 0:
        return 1
    k = 0
    while n > 0:
        n //= 10
        k += 1
    return k

def max_significant_correct(xbar, eps):
    if eps < 0:
        eps = -eps
    if eps == 0:
        return 99

    k = int_digits(xbar)

    best = 0
    for p in range(1, 21):
        bound = 0.5 * (10 ** (k - p))
        if eps <= bound:
            best = p
    return best

xbar_str = input("X: ")
eps = float(input("Erro: "))

xbar = float(xbar_str)

relev = count_relevant(xbar_str)
sig = max_significant_correct(xbar, eps)

print("\nResultado para x = {} ± {}:".format(xbar_str, eps))
print("Alg. rel. =", relev)
print("Alg. sign. =", sig)
