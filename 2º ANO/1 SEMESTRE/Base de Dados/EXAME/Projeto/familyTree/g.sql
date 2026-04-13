SELECT nome
FROM (
    SELECT Pessoas.nome, COUNT(*) AS num_netos
    FROM (
        SELECT x AS id FROM AvohDe
        UNION ALL
        SELECT y AS id FROM AvohDe
        UNION ALL
        SELECT x AS id FROM AvouDe
        UNION ALL
        SELECT y AS id FROM AvouDe
    ) AS netos
    JOIN Pessoas ON Pessoas.id = netos.id
    GROUP BY Pessoas.nome
    ORDER BY num_netos DESC
    LIMIT 1
) AS maior_num_netos;
