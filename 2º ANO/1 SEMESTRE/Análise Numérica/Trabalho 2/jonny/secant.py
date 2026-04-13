# TRABALHO 2 - Método da Secante
# João Andréz - 61066
# Carlos Ferreira - 71319
# Diogo Freitas - 90147
# Diogo Carvalho - 90247

from sympy import * # type: ignore

MAX_ITERATIONS = 10
MAX_RELATIVE_TOLERANCE = 10**(-9)

# Function for finding f(x) = 0 where:
# f: mathematical expression for the given function, where the variable is x
# r_minus1: initial aproximation of x where f(x) = 0
# r0: initial aproximation of x where f(x) = 0
# x: symbol for expression variable
# returns an aproximation of x where f(x) = 0
def solve(f , r_minus1: float, r0: float, x):
    r = float()
    k = int(0)          # iteration counter
    while(k < MAX_ITERATIONS):
        fr_minus1 = f.evalf(subs={x: r_minus1})
        fr0 = f.evalf(subs={x: r0})
        r = r0 - fr0 * (r0 - r_minus1) / (fr0 - fr_minus1)  # new solution aproximation
        
        relative_tolerance = abs(r - r0) / r     # check relative tolerance
        if relative_tolerance < MAX_RELATIVE_TOLERANCE:
            break
        
        fr = f.evalf(subs={x: r})
        if fr == 0:     # check for possible solution
            break
        
        r_minus1 = r0       # value update
        r0 = r              # value update
        
        k += 1
        
    return r