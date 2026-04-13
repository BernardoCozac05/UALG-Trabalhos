SELECT disciplinas.nome, realiza.nota
FROM disciplinas, realiza
WHERE realiza.aluno = 77892 AND disciplinas.codigo = realiza.disciplina;