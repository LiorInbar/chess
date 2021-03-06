//
// Created by Lior on 25/07/2017.
//

#ifndef CHESS_VERSION2_LOCATION_PIECE_SQUARE_H
#define CHESS_VERSION2_LOCATION_PIECE_SQUARE_H


#include <string>
#include <vector>

using namespace std;
typedef enum{PAWN,BISHOP,KNIGHT,ROOK,QUEEN,KING} piece_type;
typedef enum {EMPTY, WHITE, BLACK} Color;
typedef enum column {A, B, C, D, E, F, G, H} Column;


// coordinates of a chessboard square
class Location{

    int row;
    int column;
public:
    Location()= default;
   ~Location()= default;
    Location(int new_row ,int new_column): row(new_row),column(new_column){}
    Location(const Location& location) = default;
    Location(Location&& location) noexcept = default;
    Location& operator=(const Location& location)= default;
    Location& operator= (Location&& location) noexcept = default;
    int getRow() const;
    void setRow(int value);
    int getColumn() const;
    void setColumn(int value);
};

bool operator==(const Location& location1, const Location& location2);

/* chess piece*/
class Piece {

    int id;
    piece_type type;
    Color color;
    Location location;
    int moves_counter = 0;
public:
    Piece()= default;
    Piece(piece_type type, Color color, const Location location)
            : type(type), color(color), location(location) {}
    Piece& operator=(const Piece& piece) = default;
   int getId() const;
    void setId(int value);
    piece_type getType() const;
    void setType(const piece_type &value);
    Color getColor() const;
    void setColor(const Color &value);
    Location getLocation() const;
    void setLocation(const Location &value);
    int getMoves_counter() const;
    void setMoves_counter(int value);
};

bool operator==(const Piece& square1, const Piece& square2);

// chess board square

class Square {

    Location location;
    Piece piece;
    bool empty = true;

public:

    Square()=default;
    Square(int row, int column):location(row,column){}
    bool is_empty() const{
        return empty;
    }
    Location getLocation() const;
    void setLocation(const Location &value);
    Piece getPiece() const;
    void setPiece(const Piece &value);
    void setEmpty(bool value);

};

bool operator==(const Square& square1, const Square& square2);


//create string representation for new data types
string color_to_string(Color color);
string piece_type_to_string(piece_type type);


#endif //CHESS_VERSION2_LOCATION_PIECE_SQUARE_H
