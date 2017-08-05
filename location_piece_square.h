//
// Created by Lior on 25/07/2017.
//

#ifndef UNTITLED1_LOCATION_PIECE_SQUARE_H
#define UNTITLED1_LOCATION_PIECE_SQUARE_H
//
// Created by Lior on 18/03/2017.
//

#ifndef CHESS_SQUARE_H
#define CHESS_SQUARE_H

#include <string>
#include <vector>
using namespace std;
typedef enum{PAWN,BISHOP,KNIGHT,ROOK,QUEEN,KING} piece_type;
typedef enum {EMPTY, WHITE, BLACK} Color;
typedef enum column {A, B, C, D, E, F, G, H} Column;

struct Location{
    int row;
    int column;
    Location()= default;
    ~Location()= default;
    Location(int new_row ,int new_column): row(new_row),column(new_column){}
    Location(const Location& location) = default;
    Location(Location&& location) noexcept = default;
    Location& operator=(const Location& location)= default;
    Location& operator= (Location&& location) noexcept = default;

};

bool operator==(const Location location1, const Location location2);

struct Piece {
    int id;
    piece_type type;
    Color color;
    Location location;
    int moves_counter = 0;
    Piece()= default;
    Piece(piece_type type, Color color, const Location location)
            : type(type), color(color), location(location) {}
    Piece& operator=(const Piece& piece) = default;


};



struct Square {
    Location location;
    Piece piece;
    bool empty = true;
    Square():location(Location()){}
    Square(int row, int column):location(row,column){}
    bool is_empty() const{
        return empty;
    }
};

string color_to_string(Color color);
string piece_type_to_string(piece_type type);
void print_location(Location location);

#endif //CHESS_SQUARE_H

#endif //UNTITLED1_LOCATION_PIECE_SQUARE_H
