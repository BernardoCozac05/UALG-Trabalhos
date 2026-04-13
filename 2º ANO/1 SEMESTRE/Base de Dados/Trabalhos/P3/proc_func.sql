--Bases de Dados 2025/2026 - Grupo 26

-- =====================================================================================
-- REQUISITO: Stored Procedure para dar resposta a um requisito funcional
-- =====================================================================================
-- Descrição: O sistema de gestão de stock necessita de uma forma de dar entrada 
-- dos produtos quando uma encomenda chega do fornecedor. 
-- Este procedimento percorre os itens de uma encomenda específica e soma as 
-- quantidades recebidas ao stock atual dos respetivos produtos.

CREATE OR REPLACE PROCEDURE sp_processar_chegada_encomenda(p_id_encomenda INT)
LANGUAGE plpgsql
AS $$
DECLARE
    registo RECORD;
    encomenda_existe BOOLEAN;
BEGIN
    -- 1. Verificar se a encomenda existe
    SELECT EXISTS(SELECT 1 FROM encomenda WHERE id_encomenda = p_id_encomenda) 
    INTO encomenda_existe;

    IF NOT encomenda_existe THEN
        RAISE EXCEPTION 'A encomenda % não existe.', p_id_encomenda;
    END IF;

    -- 2. Percorrer todos os itens da encomenda
    FOR registo IN 
        SELECT id_produto, quantidade 
        FROM encomenda_item 
        WHERE id_encomenda = p_id_encomenda
    LOOP
        -- 3. Atualizar o stock do produto (Stock Atual + Quantidade da Encomenda)
        UPDATE produto
        SET stock_atual = stock_atual + registo.quantidade
        WHERE id_produto = registo.id_produto;
        
        -- Log opcional (para debug)
        RAISE NOTICE 'Produto ID %: Stock aumentado em % unidades.', registo.id_produto, registo.quantidade;
    END LOOP;

    RAISE NOTICE 'Encomenda % processada com sucesso. Stocks atualizados.', p_id_encomenda;
END;
$$;

-- EXEMPLO DE UTILIZAÇÃO DO PROCEDURE:
-- Vamos simular a chegada da Encomenda nº 10 (que tem 120 unidades do produto 10).
-- O stock atual do produto 10 é 90 (ver dml_insert.sql).
-- Após correr este comando, o stock deverá passar para 210.

/*
CALL sp_processar_chegada_encomenda(10);
*/


-- =====================================================================================
-- REQUISITO EXTRA (Função): Calcular total gasto por um cliente
-- =====================================================================================
-- Descrição: Função auxiliar para calcular o valor total (líquido) gasto por um cliente,
-- útil para determinar clientes VIP.

CREATE OR REPLACE FUNCTION fn_calcular_total_gasto_cliente(p_id_cliente INT)
RETURNS NUMERIC
LANGUAGE plpgsql
AS $$
DECLARE
    total_gasto NUMERIC(10,2);
BEGIN
    SELECT COALESCE(SUM(total_liquido), 0) INTO total_gasto
    FROM (
        SELECT 
            SUM(vi.quantidade * vi.preco_unitario) * (1 - (v.desconto_aplicado / 100.0)) AS total_liquido
        FROM venda v
        JOIN venda_item vi ON v.id_venda = vi.id_venda
        WHERE v.id_cliente = p_id_cliente
        GROUP BY v.id_venda
    ) subquery;

    RETURN total_gasto;
END;
$$;
