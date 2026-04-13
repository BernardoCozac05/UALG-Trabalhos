INSERT INTO Pessoas VALUES
(101, 'Marty Rogers', 'm', '2004-09-23'),
(202, 'John Doe', 'm', '1990-05-15'),
(303, 'Emily Smith', 'f', '1988-12-03'),
(404, 'Sarah Brown', 'f', '1985-02-10'),
(505, 'Michael Davis', 'm', '1993-09-28'),
(606, 'Paul Wester', 'm', '1987-03-15'),
(707, 'Jessica Paulson', 'f', '2001-01-05'),
(808, 'Troy Guterrez', 'm', '1998-02-14'),
(909, 'Jedediah Forth', 'm', '1968-07-03'),
(1001, 'Connor OConnell', 'm', '1989-05-02'),
(1101, 'Maria Agua', 'f', '1900-09-30'),
(1202, 'Isabelle Light', 'f', '2002-12-13'),
(1303, 'Travis Poll', 'm', '1988-12-30'),
(1404, 'Inaki Carter', 'm', '2005-05-01'),
(1505, 'Shonda Rihanna', 'f', '1949-09-20'),
(1606, 'Olivia Ortogonal', 'f', '1999-03-02'),
(1707, 'Shaniqua Holmes', 'f', '2000-10-30'),
(1808, 'Walter Potter', 'm', '1991-01-10'),
(1909, 'Maggie Rogers', 'f', '1994-06-18'),
(2001, 'Plato Plateau', 'm', '2006-03-15'),
(2101, 'Artie Abrams', 'm', '2000-08-13'),
(2202, 'Laurie Addie', 'f', '1997-02-17'),
(2303, 'Naldo Ron', 'm', '1955-04-02'),
(2404, 'Jonathan Ron', 'm', '1966-02-14'),
(2505, 'Daryl Ron', 'm', '1930-01-12'),
(2606, 'Umbridge Dorothy', 'f', '1900-05-22');


INSERT INTO MaeDe VALUES
(303, 101),
(303, 1909),
(404, 303),
(1505, 404),
(1505, 1001),
(1101, 1505),
(1707, 1404),
(1606, 1707),
(2606, 2505);

INSERT INTO PaiDe VALUES
(606, 101),
(606, 1909),
(606, 303),
(606, 2001),
(1303, 1404),
(909, 1303),
(2505, 2404),
(2505, 2303);



