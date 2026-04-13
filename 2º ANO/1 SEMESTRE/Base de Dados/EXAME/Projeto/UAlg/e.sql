SELECT disciplinas.nome
FROM disciplinas, Professores
WHERE disciplinas.regente = professores.codigo AND professores.dept = 'DEEI';
    