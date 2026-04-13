SELECT disciplinas.nome, realiza.nota
FROM disciplinas, realiza, alunos
WHERE alunos.nome = 'Jose Antunes' AND alunos.numero = realiza.aluno AND disciplinas.codigo = realiza.disciplina;