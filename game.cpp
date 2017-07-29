//
// Created by Lior on 28/07/2017.
//
#include "game.h"



State initial_state(){
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
    return State(WHITE, white_pieces, black_pieces);

}


Color Game::Turn() {
    return white_moves.size()>black_moves.size() ? BLACK : WHITE;
}

Game::Game(const State &current_state) : current_state(current_state) {}

Game::Game(){
    State state=initial_state();
    current_state=state;
    current_state.update_squares();
}

