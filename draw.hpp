/* draw the chess game state */
#pragma once
#include <vector>
#include "chess.hpp"
using std::vector;

void init_ncurses(void);
void draw_board(const BoardT&);
void clear_possible_moves(void);
void draw_possible_moves(const BoardT&, EndgameState, size_t, size_t);
void draw_captures(const vector<Piece>&, const vector<Piece>&);
void draw_n_moves(unsigned);
void draw_player(Player, EndgameState);
void draw_promotion_prompt(Player);
void draw_selection(size_t column1=BOARD_WIDTH, size_t row1=BOARD_HEIGHT,
                    size_t column2=BOARD_WIDTH);
