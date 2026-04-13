SELECT disciplinas.nome
FROM disciplinas, professores
WHERE disciplinas.regente = professores.codigo AND professores.nome = 'Fernando Lobo';