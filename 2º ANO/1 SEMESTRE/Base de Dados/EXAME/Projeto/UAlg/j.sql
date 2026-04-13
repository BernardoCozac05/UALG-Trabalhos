SELECT Alunos.numero, Alunos.nome, Disciplinas.nome, Realiza.nota 
FROM Alunos, Realiza, Disciplinas
WHERE realiza.aluno = alunos.numero AND realiza.disciplina = disciplinas.codigo AND realiza.nota = (SELECt MAX(nota) FROM realiza);
