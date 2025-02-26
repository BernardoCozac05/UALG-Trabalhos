.data 
gnum: .asciiz "Give a number: "  # Mensagem para pedir um n�mero
result: .asciiz "Factorial: "  # Mensagem para o resultado

.text

main:
# Exibir mensagem "Give a number: "
li $v0, 4
la $a0, gnum
syscall

# Ler n�mero inteiro do usu�rio
li $v0, 5
syscall
move $t0, $v0  # Armazena n�mero em $t0

# Inicializar vari�veis
li $t1, 1   # Contador come�a em 1
li $t2, 1   # Acumulador do fatorial come�a em 1

fact_calc:
bgt $t1, $t0, print_r  # Se contador > n�mero, termina

mul $t2, $t2, $t1  # Multiplica acumulador pelo contador
addi $t1, $t1, 1   # Incrementa contador
j fact_calc        # Repete o loop

print_r:
# Exibir mensagem "Factorial: "
li $v0, 4
la $a0, result
syscall

# Exibir resultado do fatorial
li $v0, 1
move $a0, $t2
syscall

end:
li $v0, 10
syscall  # Encerrar programa
