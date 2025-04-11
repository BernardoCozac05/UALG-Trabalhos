#!/bin/bash

# Compilar o programa
gcc -o DT1 DT1.c

# Lista de categorias de testes
test_categories=("TDec" "TIncr" "TPeak" "TRand" "TSame")

echo "Iniciando testes..."

# Para cada categoria e tamanho, procurar dentro da pasta Testes
for category in "${test_categories[@]}"; do
    for size in 10 100 1000 10000 100000; do
        
        # Monta o caminho para a pasta de testes
        dir="Testes/${category}${size}"
        
        if [ -d "$dir" ]; then
            # Para cada arquivo .in dentro daquela pasta
            for f in "$dir"/test*.in; do
                # Arquivo esperado .out
                output_file="${f}.out"
                
                # Exibir qual teste está rodando
                echo "=== Rodando com $f ==="
                
                # Redireciona o arquivo .in para o executável
                result=$(./DT1 < "$f")
                
                # Lê o conteúdo esperado
                expected=$(cat "$output_file")
                
                # Compara
                if [ "$result" == "$expected" ]; then
                    echo "✅ Teste passou!"
                else
                    echo "❌ Teste falhou!"
                    echo "Esperado: $expected"
                    echo "Obtido:   $result"
                fi
                echo "======================"
            done
        fi
    done
done

echo "Testes concluídos."
