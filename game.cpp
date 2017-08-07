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

Game::Game():current_state(initial_state()){
    //  State state=initial_state();

}

void Game::update_result() {
    if(current_state.is_mate()){
        result=Turn()==WHITE ? BLACK_WIN : WHITE_WIN;
    }
    if(current_state.is_stale_mate()){
        result=DRAW;
    }
}

void Game::move(Piece piece, Location location) {
    State new_state(current_state);
    new_state.make_move(piece, location);
    Move_Type type = current_state.move_type(piece,location);
    vector<Move>& moves = Turn() == WHITE ? white_moves : black_moves;
    moves.push_back(Move(current_state,new_state,piece.location,location,piece.type,
                         current_state.move_type(piece,location)));

    current_state = new_state;
    if((type!=PROMOTION)&&(type!=PROMOTION_AND_CAPTURE))
        update_result();
}

const vector<Move> &Game::getWhite_moves() const {
    return white_moves;
}

void Game::setWhite_moves(const vector<Move> &white_moves) {
    Game::white_moves = white_moves;
}

const vector<Move> &Game::getBlack_moves() const {
    return black_moves;
}

void Game::setBlack_moves(const vector<Move> &black_moves) {
    Game::black_moves = black_moves;
}

const State &Game::getCurrent_state() const {
    return current_state;
}

void Game::setCurrent_state(const State &current_state) {
    Game::current_state = current_state;
}

Result Game::getResult() const {
    return result;
}

void Game::setResult(Result result) {
    Game::result = result;
}

bool Game::isPiece_chosen() const {
    return piece_chosen_check;
}

vector<Location> Game::current_state_available_locations(Piece piece)
{
    return current_state.available_locations(piece);
}

void Game::setPiece_chosen_check(bool piece_chosen_check) {
    Game::piece_chosen_check = piece_chosen_check;
}

const Piece &Game::getChosen_Piece() const {
    return chosen_Piece;
}

void Game::setChosen_Piece(const Piece &chosen_Piece) {
    Game::chosen_Piece = chosen_Piece;
}

void Game::promotion(Location location, piece_type type) {
    current_state.promotion(location,type);
    update_result();
}

string result_to_string(Result result){
    switch(result){
        case IN_PROGRESS:
            return "in progress";
        case BLACK_WIN:
            return "Black Win!";
        case WHITE_WIN:
            return "White Win!";
        case DRAW:
            return "Draw!";
    }
}

