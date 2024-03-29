CREATE TABLE t_persons
(id INTEGER PRIMARY KEY AUTOINCREMENT,
first_name TEXT NOT NULL,
last_name TEXT NOT NULL,
representative TEXT,
birthday TEXT,
photo TEXT,
address TEXT,
phone TEXT,
email TEXT);

CREATE TABLE t_races
(id INTEGER PRIMARY KEY AUTOINCREMENT,
name TEXT NOT NULL,
place TEXT,
date TEXT);

CREATE TABLE t_categories
(id INTEGER PRIMARY KEY AUTOINCREMENT,
race_id INTEGER REFERENCES t_races(id) ON DELETE SET NULL,
name TEXT NOT NULL,
description TEXT);

CREATE TABLE t_registrations
(id INTEGER PRIMARY KEY AUTOINCREMENT,
person_id INTEGER REFERENCES t_persons(id) ON DELETE CASCADE,
category_id INTEGER REFERENCES t_categories(id) ON DELETE SET NULL,
number INTEGER);

CREATE TABLE t_rounds
(id INTEGER PRIMARY KEY AUTOINCREMENT,
race_id INTEGER REFERENCES t_races(id) ON DELETE CASCADE,
name TEXT NOT NULL,
description TEXT);

CREATE TABLE t_results
(id INTEGER PRIMARY KEY AUTOINCREMENT,
registration_id INTEGER REFERENCES t_registrations(id) ON DELETE CASCADE,
round_id INTEGER REFERENCES t_rounds(id) ON DELETE SET NULL,
time TEXT NOT NULL);
