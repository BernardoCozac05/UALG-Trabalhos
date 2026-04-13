SELECT disciplinas.nome, professores.nome
FROM disciplinas, professores
WHERE disciplinas.regente = professores.codigo; 