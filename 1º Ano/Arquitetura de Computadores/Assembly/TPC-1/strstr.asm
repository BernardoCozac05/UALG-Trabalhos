.data
haystack:    .asciiz "Exemplo123deTeste456"
needle:      .asciiz "123"
foundMsg:    .asciiz "FOUND"
notFoundMsg: .asciiz "NOT FOUND"

.text

main:
    # carrega ponteiros e chama strstr
    la   $a0, haystack       # endere�o da string principal
    la   $a1, needle         # endere�o da substring
    jal  strstr

    # verifica resultado
    beqz $v0, print_not_found

    # imprime mensagem de sucesso
    li   $v0, 4
    la   $a0, foundMsg
    syscall

    j    exit

print_not_found:
    li   $v0, 4
    la   $a0, notFoundMsg
    syscall

exit:
    li   $v0, 10
    syscall

#---------------------------------------------------
# strstr: busca a primeira ocorr�ncia de needle em haystack
# Entrada: $a0 = haystack, $a1 = needle
# Sa�da:   $v0 = ponteiro para ocorr�ncia ou 0
# Registros usados: $t0�$t6
#---------------------------------------------------
strstr:
    move $t6, $a0            # salva base de haystack
    lb   $t0, 0($a1)         # primeiro char de needle
    beqz $t0, ret_base       # needle vazia -> retorna base

search:
    lb   $t1, 0($a0)         # char atual de haystack
    beqz $t1, not_found      # fim de haystack
    bne  $t1, $t0, advance   # se diferente, avan�a

    # poss�vel in�cio de match
    move $t2, $a0            # ptr1 = haystack
    move $t3, $a1            # ptr2 = needle

check:
    lb   $t4, 0($t3)         # char de needle
    beqz $t4, match          # fim de needle -> encontrado
    lb   $t5, 0($t2)         # char de haystack
    bne  $t5, $t4, advance   # diverge -> falha
    addiu $t2, $t2, 1        # avan�a ambos
    addiu $t3, $t3, 1
    j     check

match:
    move $v0, $a0            # retorna ponteiro de in�cio
    jr   $ra

advance:
    addiu $a0, $a0, 1        # avan�a in�cio de busca
    j     search

ret_base:
    move $v0, $t6            # retorna base original
    jr   $ra

not_found:
    move $v0, $zero          # retorna NULL
    jr   $ra
