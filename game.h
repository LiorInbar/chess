//
// Created by Lior on 28/07/2017.
//

#ifndef CHESS_VERSION2_GAME_H
#define CHESS_VERSION2_GAME_H

#include "state.h"
typedef enum {IN_PROGRESS,WHITE_WIN,BLACK_WIN,DRAW} Result;

class Game{

    vector<State> states; //the game's states
    Result result = IN_PROGRESS; //game result
    bool piece_chosen_check = false; // flag for indicating wehther the current player chose a piece to play
    Piece chosen_Piece; //the piece which been chosen (if been chosen).
    State current_state;
    int moves_without_capture_or_piece_move = 0;

    void update_result(); //update the result property after a move.

public:

    Game();
    Game(const State &current_state);

    Game(const Game& game)= default;

    /*geters and seters*/

    bool isPiece_chosen() const;

    void setPiece_chosen_check(bool piece_chosen);

    const Piece &getChosen_Piece() const;

    void setChosen_Piece(const Piece &chosen_Piece);

    const State &getCurrent_state() const;

    void setCurrent_state(const State &current_state);

    Result getResult() const;

    void setResult(Result result);
    //--------------------------------

    /*get the availale locations of 'piece' in the current state with the
     * function State::available_locations*/
    vector<Location> current_state_available_locations(const Piece& piece) const;

    /*promote pawn in 'location' to piece of type 'type'*/
    void promotion(piece_type type);

    /*return the color of the current player*/
    Color Turn() const;

    /* make the move defined by moving 'piece' to 'location' in the current
     * state and create the new state which caused by the move*/
    void move(const Location& location);
};

/*string representation of the result type for testing purposes*/
string result_to_string(Result result);

#endif //CHESS_VERSION2_GAME_H
