// draw the chess game state
#pragma once
#include <vector>
#include "chess.hpp"
using std::vector;

void init_ncurses(void);
void draw_board(const BoardT&);
void clear_possible_moves(void);
void draw_possible_moves(const BoardT&, EndgameState, size_t, size_t);
void draw_captures(const vector<Piece>&, const vector<Piece>&);
void draw_player(Player, EndgameState);
void draw_selection(State, size_t, size_t, size_t);
