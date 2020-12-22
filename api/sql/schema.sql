PRAGMA foreign_keys = ON;

CREATE TABLE users(
    username VARCHAR NOT NULL,
    created DATETIME DEFAULT CURRENT_TIMESTAMP NOT NULL,
    PRIMARY KEY (username)
);
