#!/bin/bash
set -Eeuxo pipefail

DB=build/chess.sqlite3

function assert_exists() {
    if [[ ! -a $DB ]]; then
        echo error: database does not exist
        return 1
    fi
}

function assert_not_exists() {
    if [[ -a $DB ]]; then
        echo error: database exists
        return 1
    fi
}

function dump-table () {
    sqlite3 -batch -line $DB '.mode column' '.header on' "SELECT * FROM $1"
}

case $1 in
    create)
        assert_not_exists
        sqlite3 $DB < sql/schema.sql
        sqlite3 $DB < sql/data.sql
        ;;
    destroy)
        rm -f $DB
        ;;
    reset)
        $0 destroy
        $0 create
        ;;
    dump)
        assert_exists
        for table in users games states history; do
            dump-table $table
        done
        ;;
    dump-table)
        dump-table $2
        ;;
    cmd)
        sqlite3 $DB
        ;;
    *)
        echo "Usage: $0 (create|destroy|reset|dump|cmd)"
        exit 1
esac
