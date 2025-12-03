    CREATE TABLE Estudios(
        nome           VARCHAR(30),
        morada         VARCHAR(70),
        PRIMARY KEY (nome)
    );
    
    CREATE TABLE Realizadores(
        nome           VARCHAR(50),
        categoria      CHAR,          
        PRIMARY KEY (nome)
    );
    
    CREATE TABLE Filmes(
        nome           VARCHAR(50),
        ano            INTEGER,
        duracao        INTEGER,
        aCores         BOOLEAN,
        nomeEstudio    VARCHAR(30),
        nomeRealizador VARCHAR(50),
        PRIMARY KEY (nome,ano),
        FOREIGN KEY (nomeEstudio) REFERENCES Estudios(nome),
        FOREIGN KEY (nomeRealizador) REFERENCES Realizadores(nome)
    );
    
    CREATE TABLE Actores(
        nome           VARCHAR(50),
        morada         VARCHAR(70),
        sexo           CHAR,
        dataNascimento DATE,
        PRIMARY KEY (nome)
    );
    
    CREATE TABLE Participa(
        nomeFilme      VARCHAR(50),
        anoFilme       INTEGER,
        nomeActor      VARCHAR(50),
        PRIMARY KEY (nomeFilme,anoFilme,nomeActor),
        FOREIGN KEY (nomeActor) REFERENCES Actores(nome),
        FOREIGN KEY (nomeFilme,anoFilme) REFERENCES Filmes(nome,ano)
    );

