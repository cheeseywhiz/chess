// draw the chess game state
#pragma once
#include <vector>
#include "chess.hpp"
using std::vector;

void init_ncurses(void);
void draw_board(const BoardT&);
void draw_captures(const vector<Piece>&, const vector<Piece>&);
void draw_player(Player);
void draw_selection(State, size_t, size_t, size_t);
