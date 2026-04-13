DROP TABLE pessoas, paide, maede CASCADE;

CREATE TABLE Pessoas(
    id  INTEGER,
    nome    VARCHAR(60),
    sexo    CHAR,
    dataNasc    DATE,
    PRIMARY KEY(id)
);

CREATE TABLE MaeDe(
    mae    INTEGER,
    filho   INTEGER,
    PRIMARY KEY(filho),
    FOREIGN KEY(filho) REFERENCES Pessoas(id)
);

CREATE TABLE PaiDe(
    pai    INTEGER,
    filho   INTEGER,
    PRIMARY KEY(filho),
    FOREIGN KEY(filho) REFERENCES Pessoas(id)
);

CREATE OR REPLACE FUNCTION maeDeGender() RETURNS TRIGGER AS $$
    BEGIN 
        IF 'f' != (SELECT sexo FROM Pessoas
                WHERE id = NEW.mae)
        THEN
            RAISE EXCEPTION 'O sexo da Mae tem de ser Feminino.';
        END IF;
        RETURN NEW;
    END;
    $$ LANGUAGE plpgsql;

CREATE TRIGGER genderMae
    BEFORE INSERT 
    ON MaeDe
    FOR EACH ROW
        EXECUTE PROCEDURE maeDeGender();

CREATE OR REPLACE FUNCTION paiDeGender() RETURNS TRIGGER AS $$
    BEGIN
        IF 'm' != (SELECT sexo FROM Pessoas
                WHERE id = NEW.pai)
        THEN
            RAISE EXCEPTION 'O sexo do Pai tem de ser masculino.';
        END IF;
        RETURN NEW;
    END;
    $$ LANGUAGE plpgsql;

CREATE TRIGGER genderPai
    BEFORE INSERT
    ON PaiDe
    FOR EACH ROW
        EXECUTE PROCEDURE paiDeGender();

CREATE OR REPLACE FUNCTION paiAgeDiff() RETURNS TRIGGER AS $$
    BEGIN
        IF (SELECT dataNasc FROM Pessoas WHERE id = NEW.filho) <=
        (SELECT dataNasc FROM Pessoas WHERE id = NEW.pai)
        THEN 
            RAISE EXCEPTION 'O filho nao pode ser mais velho que o pai.';
        END IF;
        RETURN NEW;
    END;
    $$ LANGUAGE plpgsql;

CREATE TRIGGER paiAge
    BEFORE INSERT 
    ON PaiDe
    FOR EACH ROW
        EXECUTE PROCEDURE paiAgeDiff();

CREATE OR REPLACE FUNCTION maeAgeDiff() RETURNS TRIGGER AS $$
    BEGIN 
        IF(SELECT dataNasc FROM Pessoas WHERE id = NEW.filho) <=
        (SELECT dataNasc FROM Pessoas WHERE id = NEW.mae)
        THEN
            RAISE EXCEPTION 'O filho nao pode ser mais velho que a mae.';
        END IF;
        RETURN NEW;
    END;
    $$ LANGUAGE plpgsql;

CREATE TRIGGER maeAge
    BEFORE INSERT
    ON MaeDe
    FOR EACH ROW
        EXECUTE PROCEDURE maeAgeDiff();

CREATE VIEW AvouDe (x, y) AS 
(
    SELECT A1.pai, A2.filho
    FROM PaiDe AS A1, PaiDe As A2
    WHERE A1.filho = A2.pai
)
UNION
(
    SELECT A1.pai, A2.filho
    FROM PaiDe AS A1, MaeDe AS A2
    WHERE A1.filho = A2.mae
);


CREATE VIEW AvohDe (x, y) AS
(
    SELECT A1.mae, A2.filho
    FROM MaeDe AS A1, MaeDe AS A2
    WHERE A1.filho = A2.mae
)
UNION
(
    SELECT A1.mae, A2.filho
    FROM MaeDe AS A1, PaiDe AS A2
    WHERE A1.filho = A2.pai
);
    
                                                                                                          

