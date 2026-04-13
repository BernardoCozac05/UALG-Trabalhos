--Bases de Dados 2025/2026 - Grupo 26


-- Q1: Obter as encomendas realizadas pela funcionária Rita Almeida
SELECT e.id_encomenda, e.data_emissao, f.nome
FROM encomenda e, funcionario f
WHERE e.id_func = f.id_func
  AND f.nome = 'Rita Almeida';


-- Q2: Obter o total de vendas por cliente que é sócio
SELECT c.nome, COUNT(v.id_venda) AS total_vendas
FROM cliente c, venda v, socio s
WHERE c.id_cliente = v.id_cliente
  AND c.id_cliente = s.id_cliente
GROUP BY c.nome;


-- Q3: Obter os produtos com mais de 2 unidades vendidas no total
SELECT p.nome, SUM(vi.quantidade) AS total_vendido
FROM produto p, venda_item vi
WHERE p.id_produto = vi.id_produto
GROUP BY p.nome
HAVING SUM(vi.quantidade) > 2;


-- Q4: Obter todos os funcionários e indicar se são treinadores
SELECT f.nome, t.id_treinador
FROM funcionario f
LEFT JOIN treinador t ON f.id_func = t.id_func;


-- Q5: Obter os clientes que não são sócios
SELECT c.nome, c.email
FROM cliente c
WHERE c.id_cliente NOT IN (SELECT id_cliente FROM socio);

-- Q6: Alerta Stock Mínimo
WITH ProdutosCriticos AS (
    SELECT id_produto, nome, stock_atual, stock_minimo
    FROM produto
    WHERE stock_atual <= stock_minimo
)
SELECT nome, stock_atual, (stock_minimo - stock_atual) AS quantidade_em_falta
FROM ProdutosCriticos;

-- Q7: Obter o nome dos fornecedores que nunca receberam nenhuma encomenda do clube.
SELECT f.nome
FROM fornecedor f
WHERE NOT EXISTS (
    SELECT 1 
    FROM encomenda e 
    WHERE e.id_forn = f.id_forn
);

-- Q.8: Encontrar pares de produtos diferentes que tenham exatamente o mesmo preço de venda.

SELECT p1.nome AS produto_A, p2.nome AS produto_B, p1.preco_venda
FROM produto p1, produto p2
WHERE p1.preco_venda = p2.preco_venda
  AND p1.id_produto < p2.id_produto;

-- Q9: Obter o nome dos clientes que compraram TODOS os produtos da marca 'Wilson'.
-- (Lógica: Selecionar clientes para os quais NÃO EXISTE um produto 'Wilson' que eles NÃO tenham comprado).
SELECT c.nome
FROM cliente c
WHERE NOT EXISTS (
    SELECT p.id_produto
    FROM produto p
    WHERE p.marca = 'Wilson'
      AND NOT EXISTS (
          SELECT v.id_venda
          FROM venda v, venda_item vi
          WHERE v.id_cliente = c.id_cliente
            AND v.id_venda = vi.id_venda
            AND vi.id_produto = p.id_produto
      )
);

-- Q.10: Obter o nome e categoria dos produtos que começam pela palavra "Raquete".
SELECT nome, categoria
FROM produto
WHERE nome LIKE 'Raquete%';

-- Q.11: Obter os produtos que são mais caros do que todos os produtos da categoria 'Acessório'.
SELECT nome, preco_venda
FROM produto
WHERE preco_venda > ALL (
    SELECT preco_venda 
    FROM produto 
    WHERE categoria = 'Acessório'
);

-- Q.12: Calcular o valor total faturado por venda, listando a data, o nome do cliente e o valor final a pagar (considerando a soma dos itens e aplicando o desconto percentual da venda).
SELECT v.id_venda, 
       v.data_hora, 
       c.nome AS cliente,
       SUM(vi.quantidade * vi.preco_unitario) AS subtotal_bruto,
       v.desconto_aplicado,
       SUM(vi.quantidade * vi.preco_unitario) * (1 - (v.desconto_aplicado / 100.0)) AS valor_final_liquido
FROM venda v
JOIN cliente c ON v.id_cliente = c.id_cliente
JOIN venda_item vi ON v.id_venda = vi.id_venda
GROUP BY v.id_venda, v.data_hora, c.nome, v.desconto_aplicado
ORDER BY valor_final_liquido DESC;