SELECT nome, dataNasc
FROM alunos
WHERE curso = 'LEI'
AND dataNasc BETWEEN '2000-01-01' AND '2004-12-31';