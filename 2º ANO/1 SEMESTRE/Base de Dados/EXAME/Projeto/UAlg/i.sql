SELECT Alunos.numero, Alunos.nome, COUNT(Realiza.disciplina), AVG(Realiza.nota)
FROM Alunos, realiza
WHERE alunos.numero = realiza.aluno AND alunos.curso = 'LEI' 
GROUP BY Alunos.numero, Alunos.nome;
