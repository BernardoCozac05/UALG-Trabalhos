SELECT nome
FROM Pessoas
WHERE id IN (
    SELECT mae
    FROM MaeDe
    GROUP BY mae
    HAVING COUNT(filho) > 3
) OR id IN (
    SELECT pai
    FROM PaiDe
    GROUP BY pai
    HAVING COUNT(filho) > 3
);
