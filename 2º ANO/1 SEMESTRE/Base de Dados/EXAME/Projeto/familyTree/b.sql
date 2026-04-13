CREATE VIEW AvouDe AS 
(
    SELECT A1.pai, A2.filho
    FROM PaiDe AS A1, PaiDe As A2
    WHERE A1.filho = A2.pai
)
UNION
(
    SELECT A1.mae, A2.filho
    FROM MaeDe AS A1, PaiDe AS A2
    WHERE A1.filho = A2.pai
);


CREATE VIEW AvohDe (x, y) AS
(
    SELECT A1.mae, A2.filho
    FROM MaeDe AS A1, MaeDe AS A2
    WHERE A1.filho = A2.mae
)
UNION
(
    SELECT A1.pai, A2.filho
    FROM PaiDe AS A1, MaeDe AS A2
    WHERE A1.filho = A2.mae
);
    