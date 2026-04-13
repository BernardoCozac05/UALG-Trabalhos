# TRABALHO 2
# João Andréz - 61066
# Carlos Ferreira - 71319
# Diogo Freitas - 90147
# Diogo Carvalho - 90247

import bissection, newton, secant

if __name__ == '__main__':
    try:
        while True:
            user_selection = input("Escolha um método:\n  1) Método da Bisseção\n  2) Método de Newton\n  3) Método da Secante\n  4) Sair\n-->").strip()

            if user_selection == "1":
                bissection.userInput()
            elif user_selection == "2":
                newton.userInput()
            elif user_selection == "3":
                secant.userInput()
            elif user_selection == "4":
                break
            else:
                print("Selecione uma opção válida! (1,2,3 ou 4)")
    except KeyboardInterrupt:
        print("\nInterrompido pelo utilizador.")
