PRAGMA foreign_keys = ON;

CREATE TABLE users (
    username VARCHAR NOT NULL,
    created DATETIME DEFAULT CURRENT_TIMESTAMP NOT NULL,
    PRIMARY KEY (username)
);

CREATE TABLE states (
    stateId INTEGER PRIMARY KEY,
    state VARCHAR NOT NULL,
    player VARCHAR NOT NULL,
    endgameState VARCHAR NOT NULL,
    nMoves INTEGER NOT NULL,
    whiteCaptures VARCHAR NOT NULL,
    blackCaptures VARCHAR NOT NULL,
    board VARCHAR NOT NULL,
    created DATETIME DEFAULT CURRENT_TIMESTAMP NOT NULL
);

CREATE TABLE games (
    gameId INTEGER PRIMARY KEY,
    stateId INTEGER NOT NULL,
    newStateId INTEGER,
    white VARCHAR NOT NULL,
    black VARCHAR NOT NULL,
    created DATETIME DEFAULT CURRENT_TIMESTAMP NOT NULL,
    FOREIGN KEY (stateId) REFERENCES states (stateId)
        ON DELETE CASCADE
        ON UPDATE CASCADE,
    FOREIGN KEY (newStateId) REFERENCES states (stateId)
        ON DELETE SET NULL
        ON UPDATE CASCADE,
    FOREIGN KEY (white) REFERENCES users (username)
        ON DELETE CASCADE
        ON UPDATE CASCADE,
    FOREIGN KEY (black) REFERENCES users (username)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);

CREATE TABLE history (
    gameId INTEGER NOT NULL,
    stateId INTEGER NOT NULL,
    prev INTEGER,
    next INTEGER,
    created DATETIME DEFAULT CURRENT_TIMESTAMP NOT NULL,
    PRIMARY KEY (gameId, stateId),
    FOREIGN KEY (gameId) REFERENCES games (gameId)
        ON DELETE CASCADE
        ON UPDATE CASCADE,
    FOREIGN KEY (stateId) REFERENCES states (stateId)
        ON DELETE CASCADE
        ON UPDATE CASCADE,
    FOREIGN KEY (prev) REFERENCES states (stateId)
        ON DELETE SET NULL
        ON UPDATE CASCADE,
    FOREIGN KEY (next) REFERENCES states (stateId)
        ON DELETE SET NULL
        ON UPDATE CASCADE
);
