--Bases de Dados 2025/2026 - Grupo 26

INSERT INTO cliente (id_cliente, nome, email) VALUES
(1, 'João Silva', 'joao.silva@exemplo.pt'),
(2, 'Maria Oliveira', 'maria.oliveira@exemplo.pt'),
(3, 'Ricardo Costa', 'ricardo.costa@exemplo.pt'),
(4, 'Ana Rodrigues', 'ana.rodrigues@exemplo.pt'),
(5, 'Tiago Fernandes', 'tiago.fernandes@exemplo.pt'),
(6, 'Sofia Santos', 'sofia.santos@exemplo.pt'),
(7, 'Pedro Carvalho', 'pedro.carvalho@exemplo.pt'),
(8, 'Inês Marques', 'ines.marques@exemplo.pt'),
(9, 'Luís Gonçalves', 'luis.goncalves@exemplo.pt'),
(10,'Carla Nogueira','carla.nogueira@exemplo.pt');


INSERT INTO cliente_telefone (id_cliente, telefone) VALUES
(1, '912345678'),
(1, '965432100'),
(2, '913001001'),
(3, '914112233'),
(4, '915223344'),
(5, '916334455'),
(6, '917445566'),
(7, '918556677'),
(8, '919667788'),
(9, '911778899'),
(10, '919045896');


INSERT INTO socio (id_cliente, num_socio, data_inicio_socio) VALUES
(1, 1001, '2023-01-10'),
(2, 1002, '2023-02-15'),
(3, 1003, '2023-03-20'),
(4, 1004, '2023-04-05'),
(5, 1005, '2023-05-12');



INSERT INTO funcionario (id_func, nome) VALUES
(1, 'Miguel Pereira'),
(2, 'Rita Almeida'),
(3, 'Nuno Figueiredo'),
(4, 'Helena Matos'),
(5, 'Bruno Lopes'),
(6, 'Carolina Freitas'),
(7, 'André Barbosa'),
(8, 'Filipa Sousa'),
(9, 'Hugo Machado'),
(10,'Patrícia Reis');


INSERT INTO treinador (id_treinador, id_func) VALUES
(1, 3),
(2, 5),
(3, 9),
(4, 4),
(5, 1);



INSERT INTO fornecedor (id_forn, nome, contacto) VALUES
(1, 'Fornecedora Atlântico, Lda', 'atlantic@fornecedores.pt'),
(2, 'NutriDesporto SA', 'nutri@fornecedores.pt'),
(3, 'Bebidas do Sul Lda', 'comercial@bebidassul.pt'),
(4, 'Equipamentos FitLife', 'vendas@fitlife.pt'),
(5, 'Saúde & Energia Lda', 'geral@saudeenergia.pt'),
(6, 'CoolDrinks Portugal', 'info@cooldrinks.pt'),
(7, 'Snacks Saudáveis SA', 'snacks@saudaveis.pt'),
(8, 'TechGym Iberia', 'contacto@techgym.pt'),
(9, 'Águas da Serra', 'comercial@aguaserrapt.pt'),
(10,'SportsWear Algarve', 'vendas@sportswearalgarve.pt');


INSERT INTO produto (id_produto, nome, categoria, marca, modelo, preco_venda, stock_atual, stock_minimo) VALUES
(1, 'Garrafa de água 0.5L', 'bebida', 'Águas da Serra', 'Serra 0.5', 0.80, 200, 50),
(2, 'Barra proteica chocolate', 'snack', 'NutriBar', 'ChocoPro', 1.80, 150, 30),
(3, 'Barra proteica amendoim', 'snack', 'NutriBar', 'PeanutPro', 1.80, 120, 25),
(4, 'Bebida isotónica limão', 'bebida', 'IsoMax', 'Limao500', 1.50, 180, 40),
(5, 'Bebida isotónica frutos vermelhos', 'bebida', 'IsoMax', 'FrutosV500', 1.60, 160, 40),
(6, 'Toalha ginásio pequena', 'acessório', 'FitLife', 'TowelS', 5.50, 60, 10),
(7, 'Toalha ginásio grande', 'acessório', 'FitLife', 'TowelL', 8.90, 40, 10),
(8, 'T-shirt club homem', 'têxtil', 'SportsWear', 'TSH-H-2025', 12.90, 80, 20),
(9, 'T-shirt club mulher', 'têxtil', 'SportsWear', 'TSH-M-2025', 12.90, 70, 20),
(10,'Shaker proteína 600ml', 'acessório', 'NutriShaker', 'NS600', 6.50, 90, 15),
(11, 'Bolas de ténis (pack 4)', 'equipamento', 'Wilson', 'Championship', 8.50, 500, 100);


INSERT INTO produto_loja (id_produto) VALUES
(6),
(7),
(8),
(9),
(11);


INSERT INTO produto_bar (id_produto) VALUES
(1),
(2),
(3),
(4),
(5),
(10);


INSERT INTO venda (id_venda, data_hora, meio_pagamento, desconto_aplicado, id_cliente, id_func) VALUES
(1,  '2025-01-10 09:15:00', 'dinheiro',   5.00,  1,  10),  
(2,  '2025-01-10 10:30:00', 'multibanco', 5.00,  2,  2),   
(3,  '2025-01-11 11:05:00', 'mbway',      10.00, 3,  10),  
(4,  '2025-01-11 17:20:00', 'multibanco', 10.00, 4,  7),   
(5,  '2025-01-12 08:50:00', 'dinheiro',   5.00,  5,  2),   
(6,  '2025-01-12 19:10:00', 'mbway',      0.00,  6,  6),   
(7,  '2025-01-13 09:40:00', 'multibanco', 0.00,  7,  6),  
(8,  '2025-01-13 18:25:00', 'dinheiro',   0.00,  8,  8), 
(9,  '2025-01-14 12:15:00', 'multibanco', 0.00,  9,  2),   
(10, '2025-01-14 20:05:00', 'mbway',      0.00,  10, 7);   


INSERT INTO venda_item (id_venda, id_produto, quantidade, preco_unitario) VALUES
(1, 1, 2, 0.80),
(1, 2, 1, 1.80),
(2, 4, 2, 1.50),
(3, 2, 1, 1.80),
(4, 6, 1, 5.50),
(5, 1, 1, 0.80),
(6, 3, 2, 1.80),
(7, 8, 1, 12.90),
(8, 9, 1, 12.90),
(9, 10,1, 6.50),
(10, 3,1, 1.80);

INSERT INTO encomenda (id_encomenda, data_emissao, id_func, id_forn) VALUES
(1, '2024-12-01 10:00:00', 7, 9),
(2, '2024-12-02 11:30:00', 8, 2),
(3, '2024-12-03 09:45:00', 7, 7),
(4, '2024-12-04 14:10:00', 2, 3),
(5, '2024-12-05 15:20:00', 2, 4),
(6, '2024-12-06 16:05:00', 6, 1),
(7, '2024-12-07 10:50:00', 6, 5),
(8, '2024-12-08 13:15:00', 7, 6),
(9, '2024-12-09 09:05:00', 8, 8),
(10,'2024-12-10 17:40:00', 10,10);


INSERT INTO encomenda_item (id_encomenda, id_produto, quantidade) VALUES
(1, 1, 300),
(2, 2, 200),
(3, 3, 150),
(4, 4, 180),
(5, 5, 160),
(6, 6, 80),
(7, 7, 60),
(8, 8, 100),
(9, 9, 90),
(10,10,120);


INSERT INTO consumo_interno (id_consumo, mes_ref, id_treinador) VALUES
(1, '2025-01-01', 1),
(2, '2025-02-01', 2),
(3, '2025-03-01', 3),
(4, '2025-04-01', 4),
(5, '2025-05-01', 5),
(6, '2025-06-01', 1),
(7, '2025-07-01', 2),
(8, '2025-08-01', 3),
(9, '2025-09-01', 4),
(10,'2025-10-01',5);


INSERT INTO consumo_item (id_consumo, id_produto, quantidade) VALUES
(1, 11, 10),  
(2, 11, 8),
(3, 11, 6),
(4, 11, 5),
(5, 11, 7),
(6, 11, 6),
(7, 11, 4),
(8, 11, 5),
(9, 11, 3),
(10, 11, 4);
