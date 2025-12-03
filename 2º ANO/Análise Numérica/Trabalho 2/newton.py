# TRABALHO 2 - Método de Newton
# João Andréz - 61066
# Carlos Ferreira - 71319
# Diogo Freitas - 90147
# Diogo Carvalho - 90247

from sympy import * # type: ignore

MAX_ITERATIONS = 10
MAX_RELATIVE_TOLERANCE = 10**(-9)

# Function for finding f(x) = 0 where:
# f: mathematical expression for the given function, where the variable is x
# r0: initial aproximation of x where f(x) = 0
# x: symbol for expression variable
# returns an aproximation of x where f(x) = 0
def solve(f , r0: float, x):
    r = r0              # aproximation of solution
    diff_f = f.diff(x)  # f'(x)
    k = int(0)          # iteration counter
    while(k < MAX_ITERATIONS):
        r_old = r       # auxiliary temp of r
        fr_old = f.evalf(subs={x: r_old})               # aux f(r_old)
        diff_fr_old = diff_f.evalf(subs={x: r_old})     # aux f'(r_old)
        r = r_old - (fr_old / diff_fr_old)              # get the new solution aproximation
        
        relative_tolerance = abs(r - r_old) / r         # check relative tolerance
        print("r: ", r, "old r: ", r_old)
        print("Relative tolerance:", relative_tolerance, "Max relative tolerance:", MAX_RELATIVE_TOLERANCE)
        if relative_tolerance < MAX_RELATIVE_TOLERANCE:
            break
        
        fr = f.evalf(subs={x: r})
        if fr == 0:     # check for possible solution
            break
        
        k += 1
        
    return r