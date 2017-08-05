//
// Created by Lior on 28/07/2017.
//

#ifndef CHESS_VERSION2_MOVE_H
#define CHESS_VERSION2_MOVE_H
//

#include "state.h"


class Move{
    State before_move;
    State after_move;
    Location from;
    Location to;
    piece_type moved_piece_type;
    Move_Type moveType;
public:
    Move(const State bm, const State am, const Location f, const Location t,
         piece_type mpt, Move_Type mt) : before_move(bm),
                                         after_move(am),
                                         from(f), to(t),
                                         moved_piece_type(mpt),
                                         moveType(mt){}
    Move(const Move& original_move) = default;

    Location getTo() const;
    void setTo(const Location &value);
};






#endif //CHESS_VERSION2_MOVE_H
