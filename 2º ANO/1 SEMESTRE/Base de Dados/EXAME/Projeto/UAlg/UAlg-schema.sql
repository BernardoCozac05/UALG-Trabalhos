CREATE TABLE Cursos(
    sigla   VARCHAR(10),
    nome    VARCHAR(70),
    PRIMARY KEY(sigla)
);

CREATE TABLE Departamentos(
    sigla   VARCHAR(10),
    nome    VARCHAR(70),
    PRIMARY KEY(sigla)
);

CREATE TABLE Alunos(
    numero  INTEGER,
    nome    VARCHAR(70),
    morada  VARCHAR(70),
    dataNasc    DATE,
    curso   VARCHAR(70),
    PRIMARY KEY(numero),
    FOREIGN KEY (curso) REFERENCES Cursos(sigla)
);

CREATE TABLE Professores(
    codigo  INTEGER,
    nome    VARCHAR(70),
    dept    VARCHAR(70),
    PRIMARY KEY(codigo),
    FOREIGN KEY (dept) REFERENCES Departamentos(sigla)
);  

CREATE TABLE Disciplinas(
    codigo  INTEGER,
    nome    VARCHAR(100),
    regente INTEGER,
    PRIMARY KEY(codigo),
    FOREIGN KEY (regente) REFERENCES Professores(codigo)
);

CREATE TABLE Realiza(
    aluno   INTEGER,
    disciplina INTEGER,
    nota    INTEGER,
    PRIMARY KEY (aluno, disciplina),    
    FOREIGN KEY (aluno) REFERENCES Alunos(numero),
    FOREIGN KEY (disciplina) REFERENCES Disciplinas(codigo)
)