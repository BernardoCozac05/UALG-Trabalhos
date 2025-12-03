# TRABALHO 2
# João Andréz - 61066
# Carlos Ferreira - 71319
# Diogo Freitas - 90147
# Diogo Carvalho - 90247
import random

from sympy import *
import bissection, newton, secant

def bolzanoTheorem(f, a: float, b: float, x):
    fa = f.evalf(subs={x: a})
    fb = f.evalf(subs={x: b})

    if fa * fb < 0:
        return true
    else:
        return false

if __name__ == '__main__':
    while(True):
        user_selection = str(input("Escolha um método:\n  1) Método da Bisseção\n  2) Método de Newton\n  3) Método da Secante\n  4) Sair\n-->"))

        # Method selection
        if user_selection == "4":
            break
        elif (user_selection not in ["1", "2", "3"]):
            print("Selecione uma opção válida! (1,2,3 ou 4)")
            continue

        while(True):
            x = symbols("x")
            expression = sympify(str(input("Introduza a função (f(x)): ")))

            #### IMPLEMENTAR PARA SUBSTITUIR BOUNDS??
            # interval = input("Introduza o intervalo a explorar: ")

            lower_bound = float(input("Introduza o limite inferior de x (a): "))
            upper_bound = float(input("Introduza o limite superior de x (b): "))

            # Input checks
            if lower_bound > upper_bound:
                print("O limite inferior tem de ser menor que o superior!")
                continue

            if not bolzanoTheorem(expression, lower_bound, upper_bound, x):
                user_continue = input(
                    f"O intervalo pode não conter uma solução para função!\nPretende continuar? (Y/N): ")
                if user_continue.upper() == "N":
                    continue

            if (user_selection == "1"): # Bissection
                print(bissection.solve(expression, lower_bound, upper_bound, x))
            elif (user_selection == "2"): # Newton
                fl = diff(expression, x).subs(x, lower_bound)
                fu = diff(expression, x).subs(x, upper_bound)
                # Derivatives
                f2l = diff(expression, x, 2).subs(x, lower_bound)
                f2u = diff(expression, x, 2).subs(x, upper_bound)
                # Check for best r0
                if (fl * f2l > 0):
                    initial_aproximation = lower_bound
                elif (fu * f2u > 0):
                    initial_aproximation = upper_bound
                else:
                    initial_aproximation = lower_bound

                print(newton.solve(expression, initial_aproximation, x))
            elif (user_selection == "3"): # Secant
                print(secant.solve(expression, lower_bound, upper_bound, x))

            #### OUTSIDE
            # Loop if user doesn't input "N"
            user_continue = input(f"Pretende continuar com este método? (Y/N): ")
            if user_continue.upper() == "N":
                break