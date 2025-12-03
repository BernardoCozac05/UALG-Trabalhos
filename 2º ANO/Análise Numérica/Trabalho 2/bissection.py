# TRABALHO 2 - Método da Bisseção
# João Andréz - 61066
# Carlos Ferreira - 71319
# Diogo Freitas - 90147
# Diogo Carvalho - 90247

from sympy import * # type: ignore

MAX_ITERATIONS = 20
MAX_RELATIVE_TOLERANCE = 10**(-5)

# Function for finding f(x) = 0 where:
# f: mathematical expression for the given function, where the variable is x
# a: lower bound for x
# b: upper bound for x
# x: symbol for expression variable
# returns an aproximation of x where f(x) = 0
def solve(f , a: float, b: float, x):
    r = 0       # the median point for the given interval
    k = int(0)  # iteration counter
    while(k < MAX_ITERATIONS):
        r = (a + b) / 2     # get the median point of [a,b]
        relative_tolerance = (r - a) / r    # check relative tolerance
        if relative_tolerance < MAX_RELATIVE_TOLERANCE:
            break
        
        fa = f.evalf(subs={x: a})
        fb = f.evalf(subs={x: b})
        fr = f.evalf(subs={x: r})
        if fr == 0:     # check for possible solution
            break
        
        if fa * fr < 0: # check if f(x)=0 is in [a,r]...
            b = r
        else:           # ... or in [r,b]
            a = r
        
        k += 1
        
    return r