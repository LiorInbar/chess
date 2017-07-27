#include <iostream>
//#include "move.h"
#include "state.h"


void print_board(const State& state) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if(!state.getSquare(Location(i,j)).is_empty())
                cout<<"("+color_to_string(state.getSquare(Location(i,j)).piece->color)+","
                      +piece_type_to_string(state.getSquare(Location(i,j)).piece->type)+")"+" ";
            else
                cout<<"empty ";
        }
        cout<<endl;
    }
}


void initial_state() {
    vector<Piece> white_pieces;
    Piece white_king(KING, WHITE, Location(0, E)),
            white_queen(QUEEN, WHITE, Location(0, D)),
            white_rook1(ROOK, WHITE, Location(0, A)),
            white_rook2(ROOK, WHITE, Location(0, H)),
            white_bishop1(BISHOP, WHITE, Location(0, C)),
            white_bishop2(BISHOP, WHITE, Location(0, F)),
            white_knight1(KNIGHT, WHITE, Location(0, B)),
            white_knight2(KNIGHT, WHITE, Location(0, G));

    white_pieces.push_back(white_king);
    white_pieces.push_back(white_queen);
    white_pieces.push_back(white_bishop1);
    white_pieces.push_back(white_bishop2);
    white_pieces.push_back(white_rook1);
    white_pieces.push_back(white_rook2);
    white_pieces.push_back(white_knight1);
    white_pieces.push_back(white_knight2);

    for (int c = 0; c <= 7; c++) {
        white_pieces.emplace_back(Piece(PAWN, WHITE, Location(1, c)));
    }

    vector<Piece> black_pieces;
    Piece black_king(KING, BLACK, Location(7, E)),
            black_queen(QUEEN, BLACK, Location(7, D)),
            black_rook1(ROOK, BLACK, Location(7, A)),
            black_rook2(ROOK, BLACK, Location(7, H)),
            black_bishop1(BISHOP, BLACK, Location(7, C)),
            black_bishop2(BISHOP, BLACK, Location(7, F)),
            black_knight1(KNIGHT, BLACK, Location(7, B)),
            black_knight2(KNIGHT, BLACK, Location(7, G));

    black_pieces.push_back(black_king);
    black_pieces.push_back(black_queen);
    black_pieces.push_back(black_bishop1);
    black_pieces.push_back(black_bishop2);
    black_pieces.push_back(black_rook1);
    black_pieces.push_back(black_rook2);
    black_pieces.push_back(black_knight1);
    black_pieces.push_back(black_knight2);

    for (int c = A; c <= H; c++) {
        black_pieces.emplace_back(Piece(PAWN, BLACK, Location(6, c)));
    }

    vector<vector <Square>> board;
    for (int i = 0; i < 8; i++) {
        board.emplace_back(vector <Square>());
        for (int j = A; j <= H; j++) {
            Square square(i,j);
            board[i].push_back(square);
        }
    }

    for (int i = 0; i < white_pieces.size(); i++) {
        board[white_pieces[i].location.row][white_pieces[i].location.column].piece = &white_pieces[i];
    }
    for (int i = 0; i < black_pieces.size(); i++) {
        board[black_pieces[i].location.row][black_pieces[i].location.column].piece = &black_pieces[i];
    }
    State state(board, WHITE, white_pieces, black_pieces);
    print_board(state);
    cout<<"-----------------"<<endl;
    Piece* pawny = state.getSquare(Location(1,A)).piece;
    state.make_move(*pawny,Location(3,A));
    print_board(state);
    cout<<"-----------------"<<endl;

    Piece* knighty = state.getSquare(Location(7,B)).piece;
    state.make_move(*knighty,Location(5,A));
    print_board(state);
    cout<<"-----------------"<<endl;
    for(Location loc:state.available_locations(*pawny)){
       print_location(loc);
    }
    for(Location loc:state.available_locations(*knighty)){
        print_location(loc);
    }
    Piece* rooky = state.getSquare(Location(0,A)).piece;
    for(Location loc:state.available_locations(*rooky)){
        print_location(loc);
    }

}







int main() {
    initial_state();


    return 0;
}