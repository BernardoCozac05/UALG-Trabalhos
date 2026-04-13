SELECT DISTINCT
    CASE WHEN P1.nome < P2.nome THEN P1.nome ELSE P2.nome END AS nome1,
    CASE WHEN P1.nome < P2.nome THEN P2.nome ELSE P1.nome END AS nome2
FROM
    Pessoas P1
    JOIN (PaiDe pai1 JOIN MaeDe mae1 ON pai1.filho = mae1.filho) ON P1.id = pai1.filho
    JOIN Pessoas P2
        ON (pai1.pai = P2.id AND mae1.mae != P2.id)
        OR (mae1.mae = P2.id AND pai1.pai != P2.id)
ORDER BY
    nome1;
