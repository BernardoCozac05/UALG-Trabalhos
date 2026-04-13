(
    SELECT A1.nome AS nome1, A2.nome AS nome2
    FROM Pessoas AS A1, Pessoas AS A2, AvouDe
    WHERE A1.id = AvouDe.x AND A2.id = AvouDe.y
)
UNION
(
    SELECT A1.nome AS nome1, A2.nome AS nome2
    FROM Pessoas AS A1, Pessoas AS A2, AvohDe
    WHERE A1.id = AvohDe.x AND A2.id = AvohDe.y
)
ORDER BY nome1, nome2