SELECT 
(SELECT nome FROM Pessoas WHERE id = maeDe.mae) AS nomeMae, 
(SELECT nome FROM Pessoas WHERE id = maeDe.filho) AS nomefilho
FROM MaeDe
ORDER BY nomeMae, nomeFilho;
