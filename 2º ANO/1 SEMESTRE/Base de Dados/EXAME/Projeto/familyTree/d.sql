SELECT DISTINCT
    CASE WHEN P1.nome < P2.nome THEN P1.nome ELSE P2.nome END AS nome1,
    CASE WHEN P1.nome < P2.nome THEN P2.nome ELSE P1.nome END AS nome2
FROM
    Pessoas P1
    JOIN PaiDe pai1 ON P1.id = pai1.filho
    JOIN MaeDe mae1 ON P1.id = mae1.filho
    JOIN PaiDe pai2 ON pai1.pai = pai2.pai AND pai1.filho <> pai2.filho
    JOIN MaeDe mae2 ON mae1.mae = mae2.mae AND mae1.filho <> mae2.filho
    JOIN Pessoas P2 ON P2.id = pai2.filho AND P2.id = mae2.filho
ORDER BY
    nome1;
