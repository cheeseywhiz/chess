PRAGMA foreign_keys = ON;

CREATE TABLE users (
    username VARCHAR NOT NULL,
    created DATETIME DEFAULT CURRENT_TIMESTAMP NOT NULL,
    PRIMARY KEY (username)
);

CREATE TABLE states (
    state_id INTEGER PRIMARY KEY,
    state VARCHAR NOT NULL,
    player VARCHAR NOT NULL,
    endgame_state VARCHAR NOT NULL,
    n_moves INTEGER NOT NULL,
    white_captures VARCHAR NOT NULL,
    black_captures VARCHAR NOT NULL,
    board VARCHAR NOT NULL,
    created DATETIME DEFAULT CURRENT_TIMESTAMP NOT NULL
);

CREATE TABLE games (
    game_id INTEGER PRIMARY KEY,
    state_id INTEGER NOT NULL,
    white VARCHAR NOT NULL,
    black VARCHAR NOT NULL,
    created DATETIME DEFAULT CURRENT_TIMESTAMP NOT NULL,
    FOREIGN KEY (state_id) REFERENCES states (state_id)
        ON DELETE CASCADE
        ON UPDATE CASCADE,
    FOREIGN KEY (white) REFERENCES users (username)
        ON DELETE CASCADE
        ON UPDATE CASCADE,
    FOREIGN KEY (black) REFERENCES users (username)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);

CREATE TABLE history (
    game_id INTEGER NOT NULL,
    state_id INTEGER NOT NULL,
    created DATETIME DEFAULT CURRENT_TIMESTAMP NOT NULL,
    FOREIGN KEY (game_id) REFERENCES games (game_id)
        ON DELETE CASCADE
        ON UPDATE CASCADE,
    FOREIGN KEY (state_id) REFERENCES states (state_id)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);
