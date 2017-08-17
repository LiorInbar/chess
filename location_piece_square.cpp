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




bool operator==(const Location& location1, const Location& location2){
    return location1.getRow() == location2.getRow() &&
           location1.getColumn() == location2.getColumn();
}

bool operator==(const Square &square1, const Square &square2)
{
    if(square1.is_empty())
        return square2.is_empty();
    return ((!square2.is_empty())&&(square1.getPiece()==square2.getPiece()));
}

bool operator==(const Piece &piece1, const Piece &piece2)
{
    return ((piece1.getType()==piece2.getType())
            &&(piece1.getColor()==piece2.getColor()));
}

int Location::getColumn() const
{
    return column;
}

void Location::setColumn(int value)
{
    column = value;
}

int Location::getRow() const
{
    return row;
}

void Location::setRow(int value)
{
    row = value;
}

piece_type Piece::getType() const
{
    return type;
}

void Piece::setType(const piece_type &value)
{
    type = value;
}

Color Piece::getColor() const
{
    return color;
}

void Piece::setColor(const Color &value)
{
    color = value;
}

Location Piece::getLocation() const
{
    return location;
}

void Piece::setLocation(const Location &value)
{
    location = value;
}

int Piece::getMoves_counter() const
{
    return moves_counter;
}

void Piece::setMoves_counter(int value)
{
    moves_counter = value;
}

int Piece::getId() const
{
    return id;
}

void Piece::setId(int value)
{
    id = value;
}

Piece Square::getPiece() const
{
    return piece;
}

void Square::setPiece(const Piece &value)
{
    piece = value;
}

void Square::setEmpty(bool value)
{
    empty=value;
}

Location Square::getLocation() const
{
    return location;
}

void Square::setLocation(const Location &value)
{
    location = value;
}
