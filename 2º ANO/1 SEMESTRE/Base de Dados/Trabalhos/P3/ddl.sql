--Bases de Dados 2025/2026 - Grupo 26

CREATE TABLE cliente (
    id_cliente      INT PRIMARY KEY,
    nome            VARCHAR(100) NOT NULL,
    email           VARCHAR(255)
);

CREATE TABLE cliente_telefone (
    id_cliente      INT NOT NULL,
    telefone        VARCHAR(20) NOT NULL,
    PRIMARY KEY (id_cliente, telefone),
    FOREIGN KEY (id_cliente) REFERENCES cliente(id_cliente)
);

CREATE TABLE socio (
    id_cliente          INT  PRIMARY KEY,
    num_socio           INT  NOT NULL,
    data_inicio_socio   DATE NOT NULL,
    FOREIGN KEY (id_cliente) REFERENCES cliente(id_cliente),
    CONSTRAINT uq_socio_num_socio UNIQUE (num_socio)
);

CREATE TABLE funcionario (
    id_func INT PRIMARY KEY,
    nome    VARCHAR(100) NOT NULL
);

CREATE TABLE treinador (
    id_treinador    INT PRIMARY KEY,
    id_func         INT NOT NULL,
    FOREIGN KEY (id_func) REFERENCES funcionario(id_func),
    CONSTRAINT uq_treinador_id_func UNIQUE (id_func)
);

CREATE TABLE fornecedor (
    id_forn     INT PRIMARY KEY,
    nome        VARCHAR(100) NOT NULL,
    contacto    VARCHAR(100)
);

CREATE TABLE produto (
    id_produto      INT PRIMARY KEY,
    nome            VARCHAR(100) NOT NULL,
    categoria       VARCHAR(50) NOT NULL,
    marca           VARCHAR(50) NOT NULL,
    modelo          VARCHAR(50) NOT NULL,
    preco_venda     NUMERIC(10,2) NOT NULL CHECK (preco_venda > 0),
    stock_atual     INT NOT NULL CHECK (stock_atual >= 0),
    stock_minimo    INT NOT NULL CHECK (stock_minimo >= 0),
    CONSTRAINT uq_produto_marca_modelo UNIQUE (marca, modelo)
);

CREATE TABLE produto_loja (
    id_produto  INT PRIMARY KEY,
    FOREIGN KEY (id_produto) REFERENCES produto(id_produto)
);

CREATE TABLE produto_bar (
    id_produto  INT PRIMARY KEY,
    FOREIGN KEY (id_produto) REFERENCES produto(id_produto)
);

CREATE TABLE venda (
    id_venda            INT PRIMARY KEY,
    data_hora           TIMESTAMP NOT NULL,
    meio_pagamento      VARCHAR(30) NOT NULL,
    desconto_aplicado   NUMERIC(5,2) NOT NULL CHECK (desconto_aplicado >= 0 AND desconto_aplicado <= 100),
    id_cliente          INT NOT NULL,
    id_func             INT NOT NULL,
    FOREIGN KEY (id_cliente) REFERENCES cliente(id_cliente),
    FOREIGN KEY (id_func) REFERENCES funcionario(id_func)
);

CREATE TABLE venda_item (
    id_venda        INT NOT NULL,
    id_produto      INT NOT NULL,
    quantidade      INT NOT NULL CHECK (quantidade > 0),
    preco_unitario  NUMERIC(10,2) NOT NULL CHECK (preco_unitario > 0),
    PRIMARY KEY (id_venda, id_produto),
    FOREIGN KEY (id_venda) REFERENCES venda(id_venda),
    FOREIGN KEY (id_produto) REFERENCES produto(id_produto)
);

CREATE TABLE encomenda (
    id_encomenda    INT PRIMARY KEY,
    data_emissao    TIMESTAMP NOT NULL,
    id_func         INT NOT NULL,
    id_forn         INT NOT NULL,
    FOREIGN KEY (id_func) REFERENCES funcionario(id_func),
    FOREIGN KEY (id_forn) REFERENCES fornecedor(id_forn)
);

CREATE TABLE encomenda_item (
    id_encomenda    INT NOT NULL,
    id_produto      INT NOT NULL,
    quantidade      INT NOT NULL CHECK (quantidade > 0),
    PRIMARY KEY (id_encomenda, id_produto),
    FOREIGN KEY (id_encomenda) REFERENCES encomenda(id_encomenda),
    FOREIGN KEY (id_produto) REFERENCES produto(id_produto)
);

CREATE TABLE consumo_interno (
    id_consumo  INT PRIMARY KEY,
    mes_ref     DATE NOT NULL,
    id_treinador INT NOT NULL,
    FOREIGN KEY (id_treinador) REFERENCES treinador(id_treinador),
    CONSTRAINT uq_consumo_mes_treinador UNIQUE (mes_ref, id_treinador)
);

CREATE TABLE consumo_item (
    id_consumo  INT NOT NULL,
    id_produto  INT NOT NULL,
    quantidade  INT NOT NULL CHECK (quantidade > 0),
    PRIMARY KEY (id_consumo, id_produto),
    FOREIGN KEY (id_consumo) REFERENCES consumo_interno(id_consumo),
    FOREIGN KEY (id_produto) REFERENCES produto_loja(id_produto)
);


-- Função que atualiza o stock
CREATE OR REPLACE FUNCTION atualiza_stock()
RETURNS TRIGGER AS $$
BEGIN
    UPDATE produto 
    SET stock_atual = stock_atual - NEW.quantidade
    WHERE id_produto = NEW.id_produto;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

-- Trigger para venda_item
CREATE TRIGGER trg_atualiza_stock_venda
AFTER INSERT ON venda_item
FOR EACH ROW EXECUTE FUNCTION atualiza_stock();

-- Trigger para consumo_item
CREATE TRIGGER trg_atualiza_stock_consumo
AFTER INSERT ON consumo_item
FOR EACH ROW EXECUTE FUNCTION atualiza_stock();



-- Função que confirma stock insuficiente
CREATE OR REPLACE FUNCTION verifica_stock()
RETURNS TRIGGER AS $$
DECLARE
    stock_disponivel INT;
BEGIN
    SELECT stock_atual INTO stock_disponivel
    FROM produto WHERE id_produto = NEW.id_produto;

    IF stock_disponivel < NEW.quantidade THEN
        RAISE EXCEPTION 'Stock insuficiente. Disponível: %, Pedido: %',
            stock_disponivel, NEW.quantidade;
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

-- Trigger para venda_item
CREATE TRIGGER trg_verifica_stock_venda
BEFORE INSERT ON venda_item
FOR EACH ROW EXECUTE FUNCTION verifica_stock();

-- Trigger para consumo_item
CREATE TRIGGER trg_verifica_stock_consumo
BEFORE INSERT ON consumo_item
FOR EACH ROW EXECUTE FUNCTION verifica_stock();



-- Função que alerta quando stock atinge mínimo
CREATE OR REPLACE FUNCTION alerta_stock_minimo()
RETURNS TRIGGER AS $$
BEGIN
    IF NEW.stock_atual <= NEW.stock_minimo THEN
        RAISE NOTICE 'ALERTA: Produto % atingiu stock mínimo! Atual: %, Mínimo: %',
            NEW.nome, NEW.stock_atual, NEW.stock_minimo;
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;


-- Trigger que dispara após UPDATE stock_atual ON produto
CREATE TRIGGER trg_alerta_stock
AFTER UPDATE OF stock_atual ON produto
FOR EACH ROW EXECUTE FUNCTION alerta_stock_minimo();



-- Função que garante desconto apenas para sócios
CREATE OR REPLACE FUNCTION verifica_desconto_socio()
RETURNS TRIGGER AS $$
BEGIN
    IF NEW.desconto_aplicado > 0 THEN
        IF NOT EXISTS (SELECT 1 FROM socio WHERE id_cliente = NEW.id_cliente) THEN
            RAISE EXCEPTION 'Desconto só pode ser aplicado a sócios. Cliente % não é sócio.', 
                NEW.id_cliente;
        END IF;
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

-- Trigger para venda
CREATE TRIGGER trg_verifica_desconto
BEFORE INSERT ON venda
FOR EACH ROW EXECUTE FUNCTION verifica_desconto_socio();



