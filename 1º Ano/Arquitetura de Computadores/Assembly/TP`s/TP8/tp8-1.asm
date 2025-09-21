# ---- Macros para opera��es de pilha ----
.macro push %rg
    addi $sp, $sp, -4      # Move o stack pointer para baixo (reserva espa�o na pilha)
    sw %rg, 0($sp)         # Salva o conte�do do registrador %rg no topo da pilha
.end_macro

.macro pop %rg
    lw %rg, 0($sp)         # Recupera o valor do topo da pilha para o registrador %rg
    addi $sp, $sp, 4       # Move o stack pointer para cima (libera espa�o da pilha)
.end_macro

# ---- In�cio do Programa ----

    li $v0, 5              # syscall: c�digo 5 ? ler um inteiro do utilizador
    syscall                # executa a leitura ? valor armazenado em $v0

    move $a0, $v0          # move o n�mero lido para $a0, argumento da fun��o factorial

    jal factorial          # chama a fun��o recursiva factorial
                           # o resultado (n!) vir� em $v0

    move $a0, $v0          # move o resultado para $a0 (para imprimir)
    li   $v0, 1            # syscall: c�digo 1 ? imprimir um inteiro
    syscall                # imprime o resultado

    li $v0, 10             # syscall: c�digo 10 ? sair do programa
    syscall                # termina o programa

# ---- Fun��o Recursiva para Fatorial ----
# Entrada : $a0 = n
# Sa�da   : $v0 = n!

factorial:
    li   $t0, 1            # $t0 ? 1 (usado para verificar o caso base)
    ble  $a0, $t0, base_case # se n <= 1, salta para o caso base

    push $ra               # guarda o endere�o de retorno na pilha
    push $a0               # guarda o argumento atual (n) na pilha

    addi $a0, $a0, -1      # $a0 ? $a0 - 1 (prepara chamada recursiva com n-1)
    jal factorial          # chamada recursiva: factorial(n - 1)

    pop $a0                # recupera o valor original de n
    mul $v0, $a0, $v0      # $v0 ? n * factorial(n - 1)

    pop $ra                # recupera o endere�o de retorno
    jr  $ra                # retorna da fun��o

# ---- Caso Base: fatorial(0) = 1 e fatorial(1) = 1 ----

base_case:
    li $v0, 1              # retorna 1 no caso base
    jr $ra                 # volta para a fun��o chamadora
