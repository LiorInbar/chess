//
// Created by Lior on 28/07/2017.
//

#ifndef CHESS_VERSION2_GAME_H
#define CHESS_VERSION2_GAME_H

#include "move.h"
typedef enum {IN_PROGRESS,WHITE_WIN,BLACK_WIN,DRAW} Result;

class Game{

   vector<Move> white_moves;
    vector<Move> black_moves;
    State current_state;
    Result result = IN_PROGRESS;

public:
    Game();
    Game(const vector<Move> &white_moves, const vector<Move> &black_moves, const State &current_state) : white_moves(
            white_moves), black_moves(black_moves), current_state(current_state) {}

    Game(const State &current_state);

    Game(const Game& game)= default;
    Color Turn();


};

#endif //CHESS_VERSION2_GAME_H
