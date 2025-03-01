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
li $t1, 1   # Limite do contador so vai at� 1
li $t2, 1   # Acumulador do fatorial come�a em 1

fact_calc:
ble $t0, $t1, print_r  # Se contador(negativo) < 1, termina

mul $t2, $t2, $t0  # Multiplica acumulador pelo contador
addi $t0, $t0, -1  # Decrementa o numero/// Pode tambem se usar sub $t0, $t0, 1 
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
