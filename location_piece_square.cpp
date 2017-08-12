//
// Created by Lior on 25/07/2017.
//


#include <iostream>
#include "location_piece_square.h"

string color_to_string(Color color){
    return color==WHITE ? "white" : color == BLACK ? "black" : color == EMPTY ? "empty" : "error";
}

string piece_type_to_string(piece_type type){
    switch(type){
        case KING:
            return "king";
        case QUEEN:
            return "queen";
        case ROOK:
            return "rook";
        case BISHOP:
            return "bishop";
        case KNIGHT:
            return "knight";
        case PAWN:
            return "pawn";
        default:
            return "error";
    }
}

string location_to_string(const Location& location){
    return string("<<location.row<<","<<location.column<<");
}

bool operator==(const Location& location1, const Location& location2){
    return location1.row == location2.row &&
           location1.column == location2.column;
}
