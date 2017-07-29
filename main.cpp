#include <iostream>
//#include "move.h"
#include "state.h"





void test() {
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
    for (int index = 0; index < white_pieces.size(); index++) {
        white_pieces[index].id=index;
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

    for (int index = 0; index < black_pieces.size(); index++) {
        black_pieces[index].id=index;

    }


    State state(WHITE, white_pieces, black_pieces);

    print_board(state);
    cout<<"-----------------"<<endl;
    Piece pawny = state.getWhite_pieces()[state.getSquare(Location(1,A)).piece_id];
    state.make_move(pawny,Location(3,A));
    print_board(state);
    cout<<"-----------------"<<endl;
    Piece knighty = state.getBlack_pieces()[state.getSquare(Location(7,B)).piece_id];
    state.make_move(knighty,Location(5,A));
    print_board(state);
    cout<<"-----------------"<<endl;


    print_location(state.getWhite_pieces()[8].location);

    for(Location loc:state.available_locations(state.getWhite_pieces()[state.getSquare(Location(3,A)).piece_id])){
       print_location(loc);
    }
    cout<<state.getSquare(Location(3,A)).piece_id<<endl;

    Piece rooky = state.getWhite_pieces()[state.getSquare(Location(0,A)).piece_id];
    for(Location loc:state.available_locations(rooky)){
        print_location(loc);
    }
    state.make_move(state.getWhite_pieces()[state.getSquare(Location(3,A)).piece_id],Location(4,A));
    for(Location loc:state.available_locations(state.getBlack_pieces()[state.getSquare(Location(7,A)).piece_id])){
          print_location(loc);
      }
    state.make_move(state.getBlack_pieces()[state.getSquare(Location(6,E)).piece_id],Location(4,E));
    print_board(state);
    for(Location loc:state.available_locations(state.getWhite_pieces()[state.getSquare(Location(1,E)).piece_id])){
        print_location(loc);
    }
    cout<<"-----------------"<<endl;
    state.make_move(state.getWhite_pieces()[state.getSquare(Location(0,G)).piece_id],Location(2,F));
    print_board(state);
    for(Location loc:state.available_locations(state.getBlack_pieces()[state.getSquare(Location(7,F)).piece_id])){
        print_location(loc);
    }
    cout<<"-----------------"<<endl;
}





int main() {

    test();
    vector<int> v;
    vector<int> vv;
    v.push_back(9);
    vv=v;
    int y;
    int j;
    vv.push_back(8);
    cout<<v.size()<<endl;





    return 0;
}