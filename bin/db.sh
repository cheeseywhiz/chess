#!/bin/bash
set -Eeuxo pipefail

function assert_exists() {
    if [[ ! -a chess.sqlite3 ]]; then
        echo error: database does not exist
        return 1
    fi
}

function assert_not_exists() {
    if [[ -a chess.sqlite3 ]]; then
        echo error: database exists
        return 1
    fi
}

case $1 in
    create)
        assert_not_exists
        sqlite3 chess.sqlite3 < sql/schema.sql
        sqlite3 chess.sqlite3 < sql/data.sql
        ;;
    destroy)
        rm -f chess.sqlite3
        ;;
    reset)
        $0 destroy
        $0 create
        ;;
    dump)
        assert_exists
        sqlite3 -batch -line chess.sqlite3 '.mode column' '.header on' 'SELECT * FROM users'
        ;;
    cmd)
        sqlite3 chess.sqlite3
        ;;
    *)
        echo "Usage: $0 (create|destroy|reset|dump|cmd)"
        exit 1
esac
