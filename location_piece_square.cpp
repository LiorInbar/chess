//
// Created by Lior on 25/07/2017.
//


#include <iostream>
#include "location_piece_square.h"

string color_to_string(Color color){
    return color==WHITE ? "white" : color == BLACK ? "black" : "error";
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

void print_location(Location location){
    cout<<"("<<location.row<<","<<location.column<<")"<<endl;
}