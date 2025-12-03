/*Trabalho Realizado Por:
 * Diogo Carvalho - a90247
 * Diogo Freitas - a90147
 * 
 * PL1- G10
 */

package aed.tables;

import java.util.ArrayList;
import java.util.List;
import java.util.function.Function;

class UAlshBucket<Key,Value> implements IUAlshBucket<Key,Value> {
    Key key;
    Value value;
    int hashCode1;
    int hashCode2;
    int maxST;

    UAlshBucket() {
        this.key = null;
        this.value = null;
        this.maxST = 0;
    }

    @Override
    public Key getKey() {return key;}

    @Override
    public Value getValue() {return value;}

    @Override
    public boolean isEmpty() {return key == null;}

    @Override
    public boolean isDeleted() {return key != null && value == null;}
}

public class UAlshTable<Key,Value> {

    private static final int[] primes = {
            5, 7, 11, 17, 37, 79, 163, 331,
            673, 1361, 2729, 5471, 10949,
            21911, 43853, 87719, 175447, 350899,
            701819, 1403641, 2807303, 5614657,
            11229331, 22458671, 44917381, 89834777, 179669557
    };

    private static final int NUM_TABLES = 5;

    private UAlshBucket<Key,Value>[][] tables;
    private int[] tableSizes;
    private int[] primeIndices;
    private Function<Key,Integer> hashCode2Func;
    private int size;
    private int deletedCount;

    @SuppressWarnings("unchecked")
    public UAlshTable(Function<Key,Integer> hc2) {
        this.hashCode2Func = hc2;
        this.size = 0;
        this.deletedCount = 0;
        // Índices iniciais: T1=37, T2=17, T3=11, T4=7, T5=5
        this.primeIndices = new int[]{4, 3, 2, 1, 0};
        this.tableSizes = new int[NUM_TABLES];
        this.tables = new UAlshBucket[NUM_TABLES][];
        initTables();
    }

    @SuppressWarnings("unchecked")
    private void initTables() {
        for (int i = 0; i < NUM_TABLES; i++) {
            tableSizes[i] = primes[primeIndices[i]];
            tables[i] = new UAlshBucket[tableSizes[i]];
            for (int j = 0; j < tableSizes[i]; j++) {
                tables[i][j] = new UAlshBucket<>();
            }
        }
    }

    public int size() {return size;}
    
    public int getMainCapacity() {return tableSizes[0];}
    
    public int getTotalCapacity() {
        int total = 0;
        for (int i = 0; i < NUM_TABLES; i++) total += tableSizes[i];
        return total;
    }
    
    // LoadFactor inclui elementos apagados (ocupam espaço físico)
    public float getLoadFactor() {return (float) (size + deletedCount) / getTotalCapacity();}

    public int getDeletedNotRemoved() {return deletedCount;}

    public IUAlshBucket<Key,Value>[] getSubTable(int i) {
        if (i < 1 || i > NUM_TABLES) return null;
        return tables[i - 1];
    }

    // Função hash: h_i(x) = (hc1 + i*hc2) % m_i, onde i vai de 1 a L
    private int hash(int hc1, int hc2, int tableIndex) {
        int m = tableSizes[tableIndex];
        int i = tableIndex + 1; // i vai de 1 a 5
        // Garantir valor positivo
        int h = ((hc1 + i * hc2) & 0x7fffffff) % m;
        return h;
    }

    // Obter o balde na posição hash para uma dada chave numa dada subtabela
    private UAlshBucket<Key,Value> getBucket(int hc1, int hc2, int tableIndex) {
        int pos = hash(hc1, hc2, tableIndex);
        return tables[tableIndex][pos];
    }

    // Calcular z = min{B_i.maxST} para todos os baldes ligados pela chave
    // Se algum maxST for 0, retorna 0 imediatamente (chave não existe)
    private int calculateZ(int hc1, int hc2) {
        int z = Integer.MAX_VALUE;
        for (int i = 0; i < NUM_TABLES; i++) {
            UAlshBucket<Key,Value> b = getBucket(hc1, hc2, i);
            if (b.maxST == 0) {
                return 0;
            }
            z = Math.min(z, b.maxST);
        }
        return z;
    }

    // Procurar chave (apenas não-apagadas) - usado para get, containsKey, delete
    private UAlshBucket<Key,Value> findBucket(Key k) {
        int hc1 = k.hashCode();
        int hc2 = hashCode2Func.apply(k);
        int z = calculateZ(hc1, hc2);
        
        if (z == 0) return null;
        
        // Procurar da tabela z-1 até 0 (mais provável estar em tabelas de índice maior)
        for (int i = z - 1; i >= 0; i--) {
            UAlshBucket<Key,Value> bucket = getBucket(hc1, hc2, i);
            if (!bucket.isEmpty() && !bucket.isDeleted()) {
                // Comparar hashcodes primeiro (otimização)
                if (bucket.hashCode1 == hc1 && bucket.hashCode2 == hc2) {
                    if (bucket.key.equals(k)) {
                        return bucket;
                    }
                }
            }
        }
        return null;
    }
    
    // Procurar chave incluindo apagadas - usado no put para verificar se é reinserção
    private UAlshBucket<Key,Value> findBucketIncludingDeleted(Key k, int hc1, int hc2) {
        // Procurar em TODAS as subtabelas (chave apagada pode estar além de z atual)
        // porque z pode ter diminuído desde que a chave foi inserida
        for (int i = 0; i < NUM_TABLES; i++) {
            UAlshBucket<Key,Value> bucket = getBucket(hc1, hc2, i);
            if (!bucket.isEmpty()) {
                // Comparar hashcodes primeiro (otimização)
                if (bucket.hashCode1 == hc1 && bucket.hashCode2 == hc2) {
                    if (bucket.key.equals(k)) {
                        return bucket;
                    }
                }
            }
        }
        return null;
    }

    public boolean containsKey(Key k) {return findBucket(k) != null;}

    public Value get(Key k) {
        UAlshBucket<Key,Value> bucket = findBucket(k);
        return bucket == null ? null : bucket.value;
    }

    public void put(Key k, Value v) {
        if (v == null) {
            delete(k);
            return;
        }
        
        int hc1 = k.hashCode();
        int hc2 = hashCode2Func.apply(k);
        
        // 1. Verificar se a chave já existe (incluindo apagadas)
        UAlshBucket<Key,Value> existing = findBucketIncludingDeleted(k, hc1, hc2);
        if (existing != null) {
            if (existing.isDeleted()) {
                // Reinserção de chave apagada - reaproveitar o MESMO balde
                existing.value = v;
                deletedCount--;
                size++;
            } else {
                // Atualização de valor existente
                existing.value = v;
            }
            return;
        }
        
        // 2. Nova inserção - verificar resize (quando size == tamanho de T1)
        if (size >= tableSizes[0]) {
            resize(true);
            hc1 = k.hashCode();
            hc2 = hashCode2Func.apply(k);
        }
        
        // 3. Procurar primeiro balde disponível
        // Primeiro passa: procurar VAZIO
        // Segunda passa: se não há vazio, procurar APAGADO
        int insertTable = -1;
        int firstDeletedTable = -1;
        
        for (int i = 0; i < NUM_TABLES; i++) {
            UAlshBucket<Key,Value> bucket = getBucket(hc1, hc2, i);
            if (bucket.isEmpty()) {
                insertTable = i;
                break;
            } else if (bucket.isDeleted() && firstDeletedTable == -1) {
                firstDeletedTable = i;
            }
        }
        
        // Se não encontrou vazio, usar apagado
        if (insertTable == -1 && firstDeletedTable != -1) {
            insertTable = firstDeletedTable;
        }
        
        // 4. Se não encontrou nenhum disponível, fazer resize
        if (insertTable == -1) {
            resize(true);
            put(k, v);
            return;
        }
        
        // 5. Inserir no balde selecionado
        int pos = hash(hc1, hc2, insertTable);
        UAlshBucket<Key,Value> bucket = tables[insertTable][pos];
        
        // Se estamos a usar um balde apagado de outra chave, decrementar contador
        if (bucket.isDeleted()) {
            deletedCount--;
        }
        
        bucket.key = k;
        bucket.value = v;
        bucket.hashCode1 = hc1;
        bucket.hashCode2 = hc2;
        size++;
        
        // 6. Atualizar maxST de todos os baldes ligados
        int maxSTValue = insertTable + 1;
        for (int i = 0; i < NUM_TABLES; i++) {
            int p = hash(hc1, hc2, i);
            tables[i][p].maxST = Math.max(tables[i][p].maxST, maxSTValue);
        }
    }

    public void fastPut(Key k, Value v) {
        int hc1 = k.hashCode();
        int hc2 = hashCode2Func.apply(k);
        
        // Verificar resize
        if (size >= tableSizes[0]) {
            resize(true);
            hc1 = k.hashCode();
            hc2 = hashCode2Func.apply(k);
        }
        
        // fastPut assume que a chave não existe ATIVA, mas pode existir APAGADA
        UAlshBucket<Key,Value> existing = findBucketIncludingDeleted(k, hc1, hc2);
        if (existing != null && existing.isDeleted()) {
            existing.value = v;
            deletedCount--;
            size++;
            return;
        }
        
        // Procurar vazio primeiro, depois apagado
        int insertTable = -1;
        int firstDeletedTable = -1;
        
        for (int i = 0; i < NUM_TABLES; i++) {
            UAlshBucket<Key,Value> bucket = getBucket(hc1, hc2, i);
            if (bucket.isEmpty()) {
                insertTable = i;
                break;
            } else if (bucket.isDeleted() && firstDeletedTable == -1) {
                firstDeletedTable = i;
            }
        }
        
        if (insertTable == -1 && firstDeletedTable != -1) {
            insertTable = firstDeletedTable;
        }
        
        if (insertTable == -1) {
            resize(true);
            fastPut(k, v);
            return;
        }
        
        int pos = hash(hc1, hc2, insertTable);
        UAlshBucket<Key,Value> bucket = tables[insertTable][pos];
        
        if (bucket.isDeleted()) {
            deletedCount--;
        }
        
        bucket.key = k;
        bucket.value = v;
        bucket.hashCode1 = hc1;
        bucket.hashCode2 = hc2;
        size++;
        
        // Atualizar maxST
        int maxSTValue = insertTable + 1;
        for (int i = 0; i < NUM_TABLES; i++) {
            int p = hash(hc1, hc2, i);
            tables[i][p].maxST = Math.max(tables[i][p].maxST, maxSTValue);
        }
    }

    public void delete(Key k) {
        UAlshBucket<Key,Value> bucket = findBucket(k);
        if (bucket == null) return;
        
        // Remoção preguiçosa - marcar como apagado (value = null)
        bucket.value = null;
        size--;
        deletedCount++;
        
        // Verificar se precisa de downsize
        if (size < tableSizes[0] / 4 && primeIndices[0] > 4) {
            resize(false);
        }
    }

    private void resize(boolean grow) {
        // Guardar todas as chaves/valores válidos (não apagados)
        List<Key> keys = new ArrayList<>();
        List<Value> values = new ArrayList<>();
        List<Integer> hc1s = new ArrayList<>();
        List<Integer> hc2s = new ArrayList<>();
        
        for (int i = 0; i < NUM_TABLES; i++) {
            for (int j = 0; j < tableSizes[i]; j++) {
                UAlshBucket<Key,Value> b = tables[i][j];
                if (!b.isEmpty() && !b.isDeleted()) {
                    keys.add(b.key);
                    values.add(b.value);
                    hc1s.add(b.hashCode1);
                    hc2s.add(b.hashCode2);
                }
            }
        }
        
        // Ajustar índices dos primos
        if (grow) {
            for (int i = 0; i < NUM_TABLES; i++) {
                if (primeIndices[i] < primes.length - 1) {
                    primeIndices[i]++;
                }
            }
        } else {
            for (int i = 0; i < NUM_TABLES; i++) {
                // T1 nunca pode ser menor que índice 4 (valor 37)
                int minIdx = (i == 0) ? 4 : 0;
                if (primeIndices[i] > minIdx) {
                    primeIndices[i]--;
                }
            }
        }
        
        // Reinicializar tabelas
        initTables();
        size = 0;
        deletedCount = 0;
        
        // Reinserir todas as chaves
        for (int idx = 0; idx < keys.size(); idx++) {
            reinsert(keys.get(idx), values.get(idx), hc1s.get(idx), hc2s.get(idx));
        }
    }

    // Reinserção otimizada durante resize (sabe-se que chave não existe)
    private void reinsert(Key k, Value v, int hc1, int hc2) {
        int insertTable = -1;
        for (int i = 0; i < NUM_TABLES; i++) {
            int pos = hash(hc1, hc2, i);
            if (tables[i][pos].isEmpty()) {
                insertTable = i;
                break;
            }
        }
        
        // Se não houver espaço, há um problema (deveria haver após resize)
        if (insertTable == -1) {
            // Fazer resize adicional se necessário
            resize(true);
            reinsert(k, v, hc1, hc2);
            return;
        }
        
        int pos = hash(hc1, hc2, insertTable);
        UAlshBucket<Key,Value> bucket = tables[insertTable][pos];
        bucket.key = k;
        bucket.value = v;
        bucket.hashCode1 = hc1;
        bucket.hashCode2 = hc2;
        size++;
        
        int maxSTValue = insertTable + 1;
        for (int i = 0; i < NUM_TABLES; i++) {
            int p = hash(hc1, hc2, i);
            tables[i][p].maxST = Math.max(tables[i][p].maxST, maxSTValue);
        }
    }

    public Iterable<Key> keys() {
        List<Key> lista = new ArrayList<>();
        for (int i = 0; i < NUM_TABLES; i++) {
            for (int j = 0; j < tableSizes[i]; j++) {
                UAlshBucket<Key,Value> b = tables[i][j];
                if (!b.isEmpty() && !b.isDeleted()) {
                    lista.add(b.key);
                }
            }
        }
        return lista;
    }

    public Iterable<Value> values() {
        List<Value> result = new ArrayList<>();
        for (int i = 0; i < NUM_TABLES; i++) {
            for (int j = 0; j < tableSizes[i]; j++) {
                UAlshBucket<Key,Value> b = tables[i][j];
                if (!b.isEmpty() && !b.isDeleted()) {
                    result.add(b.value);
                }
            }
        }
        return result;
    }

    public static void main(String[] args) {
        System.out.println("=== Testes UAlshTable ===\n");
        
        Function<Integer, Integer> hc2 = k -> k * 31 + 7;
        UAlshTable<Integer, String> table = new UAlshTable<>(hc2);
        
        System.out.println("Tabela vazia:");
        System.out.println("  size=" + table.size());
        System.out.println("  mainCapacity=" + table.getMainCapacity());
        System.out.println("  totalCapacity=" + table.getTotalCapacity());
        
        table.put(1, "um");
        table.put(2, "dois");
        table.put(3, "tres");
        table.put(10, "dez");
        table.put(20, "vinte");
        
        System.out.println("\nApós 5 inserções:");
        System.out.println("  size=" + table.size());
        System.out.println("  containsKey(3)=" + table.containsKey(3));
        System.out.println("  get(10)=" + table.get(10));
        
        table.put(1, "UM_ATUALIZADO");
        System.out.println("\nApós update: size=" + table.size());
        
        table.delete(2);
        System.out.println("\nApós delete: size=" + table.size());
        System.out.println("  deletedNotRemoved=" + table.getDeletedNotRemoved());
        
        table.put(2, "dois_novo");
        System.out.println("\nApós reinserir: size=" + table.size());
        System.out.println("  deletedNotRemoved=" + table.getDeletedNotRemoved());
        
        System.out.println("\n=== Testes Concluídos ===");
    }
}