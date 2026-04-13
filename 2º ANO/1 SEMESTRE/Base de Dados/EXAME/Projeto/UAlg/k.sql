SELECT MAX(realiza.nota) AS nota_mais_alta
FROM Realiza, disciplinas
WHERE realiza.disciplina = disciplinas.codigo AND disciplinas.nome = 'Programacao Imperativa';
