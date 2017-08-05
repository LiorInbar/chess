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
    Result result = IN_PROGRESS;
    bool piece_chosen_check = false;
    Piece chosen_Piece;
    State current_state;
public:

    bool isPiece_chosen() const;
    vector<Location> current_state_available_locations(Piece piece);
    void setPiece_chosen_check(bool piece_chosen);
    void add_piece(piece_type type, Color color, Location location);
    const Piece &getChosen_Piece() const;

    void setChosen_Piece(const Piece &chosen_Piece);

    Game();
    Game(const vector<Move> &white_moves, const vector<Move> &black_moves, const State &current_state) : white_moves(
            white_moves), black_moves(black_moves), current_state(current_state) {}

    Game(const State &current_state);

    Game(const Game& game)= default;
    Color Turn();
    void move(Piece piece, Location location);

    const vector<Move> &getWhite_moves() const;

    void setWhite_moves(const vector<Move> &white_moves);

    const vector<Move> &getBlack_moves() const;

    void setBlack_moves(const vector<Move> &black_moves);

    const State &getCurrent_state() const;

    void setCurrent_state(const State &current_state);

    Result getResult() const;

    void setResult(Result result);

};

string result_to_string(Result result);

#endif //CHESS_VERSION2_GAME_H
