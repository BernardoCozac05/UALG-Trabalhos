--Bases de Dados 2025/2026 - Grupo 26

-- =====================================================================================
-- VISTA 1: Catálogo Público de Produtos
-- =====================================================================================
-- Requisito/Justificação:
-- Os clientes e visitantes devem poder consultar os produtos e preços, mas não devem 
-- ter acesso a informações sensíveis de gestão, como a quantidade exata de stock, 
-- o stock mínimo ou o fornecedor.
-- Esta vista oculta o 'stock_atual' e substitui por um indicador de disponibilidade.

CREATE OR REPLACE VIEW v_catalogo_publico AS
SELECT 
    nome AS produto,
    categoria,
    marca,
    modelo,
    preco_venda AS preco,
    CASE 
        WHEN stock_atual > 0 THEN 'Disponível'
        ELSE 'Esgotado'
    END AS estado_stock
FROM produto;

-- EXEMPLO DE UTILIZAÇÃO:
-- Consultar o catálogo de raquetes e bebidas disponíveis para venda.
/*
SELECT * FROM v_catalogo_publico 
WHERE categoria IN ('bebida', 'equipamento');
*/


-- =====================================================================================
-- VISTA 2: Lista de Contactos de Sócios Ativos
-- =====================================================================================
-- Requisito/Justificação:
-- A receção necessita de uma lista rápida para contactar sócios, sem necessitar de fazer
-- joins complexos manualmente entre as tabelas 'cliente', 'socio' e 'cliente_telefone'.
-- Esta vista filtra apenas quem é sócio e apresenta os dados de contacto consolidados.

CREATE OR REPLACE VIEW v_contactos_socios AS
SELECT 
    s.num_socio,
    c.nome,
    c.email,
    ct.telefone,
    s.data_inicio_socio
FROM socio s
JOIN cliente c ON s.id_cliente = c.id_cliente
LEFT JOIN cliente_telefone ct ON c.id_cliente = ct.id_cliente;