//
// Created by Lior on 19/03/2017.
//

#include "state.h"
#include <iostream>
#include <math.h>
#include <algorithm>


State::State(Color new_turn, vector<Piece> new_white_pieces,
             vector<Piece> new_black_pieces):
        turn(new_turn),white_pieces(new_white_pieces),
        black_pieces(new_black_pieces),
        en_passant_location(Location(0,0)){
    for (int row = 0; row < 8; row++){
        board.emplace_back(vector<Square>());
        for (int column = 0; column < 8; column++){
            board[row].push_back(Square(row,column));
        }
    }


    for (Piece piece:white_pieces) {
        Square &square = board[piece.location.row][piece.location.column];
        square.piece = piece;
        square.empty = false;

    }
    for (Piece piece:black_pieces) {
        Square &square = board[piece.location.row][piece.location.column];
        square.piece = piece;
        square.empty = false;
    }

}

State::State(const State& original_state):
        State(original_state.turn, original_state.white_pieces, original_state.black_pieces) {
    en_passant_flag=original_state.en_passant_flag;
    en_passant_location=original_state.en_passant_location;
}

State& State::operator=(const State& state) {
    turn =state.turn;
    white_pieces = state.white_pieces;
    black_pieces = state.black_pieces;
    en_passant_location = state.en_passant_location;
    en_passant_flag = state.en_passant_flag;
    for (int row = 0; row < 8; row++){
        for (int column = 0; column < 8; column++){
            board[row][column] = state.board[row][column];
        }
    }
    return *this;
}

Location State::king_location(const Color color) const{ //nothing to explain
    vector<Piece> pieces = color == WHITE ? white_pieces : black_pieces;
    for(Piece piece:pieces){
        if(piece.type == KING)
            return piece.location;
    }
}

/*return vector of all the locations in the row between
 * l1 and l2, not including*/
vector<Location> get_row(const Location& l1, const Location& l2){
    vector<Location> row_vector;
    Direction direction = l1.column > l2.column ? LEFT : RIGHT;
    if(direction == LEFT) {
        for (int column = l1.column-1; column > l2.column; column--){
            row_vector.emplace_back(Location(l1.row,column));
        }
    }
    else{
        for (int column = l1.column+1; column < l2.column; column++){
            row_vector.emplace_back(Location(l1.row,column));
        }
    }
    return row_vector;
}

/*Return vector of all the locations in the column between
 * l1 and l2, not including*/
vector<Location> get_column(const Location& l1, const Location& l2){
    vector<Location> column_vector;
    Direction direction = l1.row > l2.row ? DOWN : UP;
    if(direction==DOWN) {
        for (int index = l1.row-1; index > l2.row; index--){
            column_vector.emplace_back(Location(index,l1.column));
        }
    }
    else{
        for (int index = l1.row+1; index < l2.row; index++){
            column_vector.emplace_back(Location(index,l1.column));
        }
    }
    return column_vector;
}

/*return vector of all the locations in the diagonal between
 * l1 and l2, not including*/
vector<Location> get_diagonal(const Location& l1, const Location& l2){
    vector<Location> diagonal_vector;
    Direction horizontal_direction = l1.column > l2.column ? LEFT : RIGHT;
    Direction vertical_direction = l1.row > l2.row ? DOWN : UP;

    if(horizontal_direction == LEFT && vertical_direction == DOWN) {
        for (int row_index = l1.row-1, column_index = l1.column-1;
             row_index > l2.row; row_index--, column_index--){
            diagonal_vector.emplace_back(Location(row_index,column_index));
        }
    }

    else if(horizontal_direction == LEFT && vertical_direction == UP) {
        for (int row_index = l1.row+1, column_index = l1.column-1;
             row_index < l2.row; row_index++, column_index--){
            diagonal_vector.emplace_back(Location(row_index,column_index));
        }
    }
    else if(horizontal_direction == RIGHT && vertical_direction == DOWN) {
        for (int row_index = l1.row-1, column_index = l1.column+1;
             row_index > l2.row; row_index--, column_index++){
            diagonal_vector.emplace_back(Location(row_index,column_index));
        }
    }
    else if(horizontal_direction == RIGHT && vertical_direction == UP) {

        for (int row_index = l1.row+1, column_index = l1.column+1;
             row_index < l2.row; row_index++, column_index++){
            diagonal_vector.emplace_back(Location(row_index,column_index));
        }
    }
    return diagonal_vector;
}



bool State::threatened_square(const Location& location, const Color threatening_player) const{
    vector<Piece> pieces = threatening_player == WHITE ? white_pieces : black_pieces;
    for(Piece piece: pieces){
        for(Location loc: direct_course(piece)){
            if(loc==location){
                return true;
            }
        }
    }
    return false;
}
bool State::is_in_check(const Color color) const{
    /*check if the king square is under threat*/
    Color opposition = color == WHITE ? BLACK : WHITE;
    return threatened_square(king_location(color), opposition);

}

bool State::possible_queenside_castling() const{

    if(turn == WHITE){
        return !(board[0][E].is_empty()) &&
               !(board[0][A].is_empty()) &&
               board[0][B].is_empty() &&
               board[0][C].is_empty() &&
               board[0][D].is_empty() &&
               board[0][E].piece.color == WHITE &&
               board[0][A].piece.color == WHITE &&
               board[0][E].piece.type == KING &&
               board[0][A].piece.type == ROOK &&
               board[0][E].piece.moves_counter == 0 &&
               board[0][A].piece.moves_counter == 0 &&
               !threatened_square(Location(0,E),BLACK) &&
               !threatened_square(Location(0,D),BLACK) &&
               !threatened_square(Location(0,C),BLACK);
    }
    if(turn == BLACK){
        return !board[7][E].is_empty() &&
               !board[7][A].is_empty() &&
               board[7][B].is_empty() &&
               board[7][C].is_empty() &&
               board[7][D].is_empty() &&
               board[7][E].piece.color == BLACK &&
               board[7][A].piece.color == BLACK &&
               board[7][E].piece.type == KING &&
               board[7][A].piece.type == ROOK &&
               board[7][E].piece.moves_counter == 0 &&
               board[7][A].piece.moves_counter == 0 &&
               !threatened_square(Location(7,E),WHITE) &&
               !threatened_square(Location(7,D),WHITE) &&
               !threatened_square(Location(7,C),WHITE);
    }

}

bool State::possible_kingside_castling() const{

    if(turn == WHITE){
        return !board[0][E].is_empty() &&
               !board[0][H].is_empty() &&
               board[0][G].is_empty() &&
               board[0][F].is_empty() &&
               board[0][E].piece.color == WHITE &&
               board[0][H].piece.color == WHITE &&
               board[0][E].piece.type == KING &&
               board[0][H].piece.type == ROOK &&
               board[0][E].piece.moves_counter == 0 &&
               board[0][H].piece.moves_counter == 0 &&
               !threatened_square(Location(0,E),BLACK) &&
               !threatened_square(Location(0,F),BLACK) &&
               !threatened_square(Location(0,G),BLACK);
    }
    if(turn == BLACK){
        return !board[7][E].is_empty() &&
               !board[7][H].is_empty() &&
               board[7][G].is_empty() &&
               board[7][F].is_empty() &&
               board[7][E].piece.color == BLACK &&
               board[7][H].piece.color == BLACK &&
               board[7][E].piece.type == KING &&
               board[7][H].piece.type == ROOK &&
               board[7][E].piece.moves_counter == 0 &&
               board[7][H].piece.moves_counter == 0 &&
               !threatened_square(Location(7,E),WHITE) &&
               !threatened_square(Location(7,F),WHITE) &&
               !threatened_square(Location(7,G),WHITE);
    }

}

void State::remove_piece_in_location(Location location){
    Piece &piece = board[location.row][location.column].piece;
    vector<Piece>& pieces = piece.color == WHITE ? white_pieces : black_pieces;
    int id = piece.id;
    pieces.erase(pieces.begin() + id);
    for(int index = id; index<pieces.size(); index++){
        Piece &p = pieces[index];
        p.id--;
        board[p.location.row][p.location.column].piece.id--;
    }
    board[location.row][location.column].empty=true;
}

void State::move_piece(const Location& from, const Location& to){

    vector<Piece>& pieces = board[from.row][from.column].piece.color == WHITE ? white_pieces : black_pieces;
    int id = board[from.row][from.column].piece.id;
    pieces[id].location = to;
    board[to.row][to.column].piece = pieces[id];
    board[from.row][from.column].empty=true;
    board[to.row][to.column].empty= false;

}

void State::capture_piece(const Location& from, const Location& to){
    remove_piece_in_location(to);
    move_piece(from,to);
}


void State::capture_piece_en_passant(const Location& from, const Location& to){
    Location captured_pawn_location =
            turn == WHITE ? Location(to.row-1, to.column): Location(to.row+1, to.column);
    remove_piece_in_location(captured_pawn_location);
    move_piece(from,to);
}

vector<Location> get_diagonals(const Location& location) {
    vector<Location> locations;
    for (int row_index = location.row + 1, column_index = location.column + 1;
         row_index <= 7 && column_index <= H; row_index++, column_index++) {
        locations.emplace_back(Location(row_index, column_index));
    }
    for (int row_index = location.row + 1, column_index = location.column - 1;
         row_index <= 7 && column_index >= A; row_index++, column_index--) {
        locations.emplace_back(Location(row_index, column_index));
    }
    for (int row_index = location.row - 1, column_index = location.column - 1;
         row_index >= 0 && column_index >= A; row_index--, column_index--) {
        locations.emplace_back(Location(row_index, column_index));
    }
    for (int row_index = location.row - 1, column_index = location.column + 1;
         row_index >= 0 && column_index <= H; row_index--, column_index++) {
        locations.emplace_back(Location(row_index, column_index));
    }
    return locations;
}

bool is_knight_move(const Location& from, const Location& to){//check if the target location can be reached from the  piece location with a knight move.

    return ((abs(from.row - to.row) == 2
             && abs(from.column - to.column) == 1)
            || (abs(from.row - to.row) == 1
                && abs(from.column - to.column) == 2));
}

vector<Location> two_squares_distance(const Location& location){
    vector<Location> locations;
    for (int row_index = max(location.row-2, 0); row_index <= min(7, location.row+2); row_index++){
        for (int column_index = max(location.column-2, 0); column_index <= min(7, location.column+2); column_index++) {
            if (row_index != location.row || column_index != location.column)
                locations.emplace_back(Location(row_index, column_index));
        }
    }
    return locations;
}

vector<Location> one_squares_distance(const Location& location){
    vector<Location> locations;
    for (int row_index = max(location.row-1, 0); row_index <= min(7, location.row+1); row_index++){
        for (int column_index = max(location.column-1, 0); column_index <= min(7, location.column+1); column_index++) {
            if (row_index != location.row || column_index != location.column)
                locations.emplace_back(Location(row_index, column_index));
        }
    }
    return locations;
}

vector<Location> State::direct_course_knight(const Piece& piece) const{
    vector<Location> locations;
    vector<Location> potential_locations = two_squares_distance(piece.location);
    for(Location loc:potential_locations) {
        if (is_knight_move(piece.location,loc) &&
            (!player_piece_on_location(loc, piece.color))) {
            locations.push_back(loc);
        }
    }
    return locations;
}


bool State::player_piece_on_location(const Location& location, const Color color) const{
    if(board[location.row][location.column].is_empty())
        return false;
    return board[location.row][location.column].piece.color == color;
}

vector<Location> State::direct_course_rook(const Piece& piece) const{
    vector<Location> locations;
    for(int column = 0; column < 8; column++){
        if(column == piece.location.column)
            continue;
        if(player_piece_on_location(Location(piece.location.row, column), piece.color))
            continue;
        vector<Location> row = get_row(piece.location, Location(piece.location.row, column));
        if (row.empty()) {
            locations.push_back(Location(piece.location.row,column));
            continue;
        }
        bool check=true;
        for (Location loc:row) { //check if there other pieces on the
            if (!board[loc.row][loc.column].is_empty()) {//row between the locations
                check=false;
                break;
            }
        }
        if(check)
            locations.push_back(Location(piece.location.row,column));
    }
    for(int row = 0; row < 8; row++){
        if(row == piece.location.row)
            continue;
        if(player_piece_on_location(Location(row,piece.location.column), piece.color))
            continue;

        vector<Location> column = get_column(piece.location, Location(row,piece.location.column));
        if (column.empty()) {
            locations.push_back(Location(row,piece.location.column));
            continue;
        }
        bool check=true;
        for (Location loc:column) { //check if there other pieces on the
            if (!board[loc.row][loc.column].is_empty()) { //column between the locations
                check = false;
                break;
            }
        }
        if(check)
            locations.push_back(Location(row,piece.location.column));
    }

    return locations;
}

vector<Location> State::direct_course_queen(const Piece& piece)const{
    vector<Location> diagonal_locations, raws_and_columns_locations, locations;
    diagonal_locations = direct_course_bishop(piece);
    raws_and_columns_locations = direct_course_rook(piece);
    for(Location location:diagonal_locations){
        locations.push_back(location);
    }
    for(Location location:raws_and_columns_locations){
        locations.push_back(location);
    }
    return locations;
}

vector<Location> State::direct_course_king(const Piece& piece) const{
    vector<Location> one_squares_distance_locations = one_squares_distance(piece.location);
    vector<Location> locations;
    for(Location location:one_squares_distance_locations){
        if(!player_piece_on_location(location, piece.color))
            locations.push_back(location);
    }
    return locations;
}

vector<Location> State::direct_course_pawn(const Piece& piece) const{
    vector<Location> locations;
    int row = piece.location.row;
    int column = piece.location.column;
    if (piece.color == WHITE) {
        if (board[row + 1][column].is_empty()) { //regular pawn move
            locations.emplace_back(Location(row + 1, column));
            if (row == 1 && board[3][column].is_empty()) //double square pawn move
                locations.emplace_back(Location(3, column));
        }
        //left capture
        if (column > A &&
            !(board[row + 1][column - 1].is_empty()) &&
            board[row + 1][column - 1].piece.color == BLACK)
            locations.emplace_back(Location(row + 1, column - 1));

        //right capture
        if (column < H &&
            !(board[row + 1][column + 1].is_empty()) &&
            board[row + 1][column + 1].piece.color == BLACK)
            locations.emplace_back(Location(row + 1, column + 1));

    }
    else if (piece.color == BLACK) {
        if (board[row - 1][column].is_empty()) {
            locations.emplace_back(Location(row - 1, column)); //regular pawn move
            if (row == 6 && board[4][column].is_empty())
                locations.emplace_back(Location(4, column)); //double square pawn move

        }
        //left capture
        if (column > A &&
            !(board[row - 1][column - 1].is_empty()) &&
            board[row - 1][column - 1].piece.color == WHITE)
            locations.emplace_back(Location(row - 1, column - 1));

        //right capture
        if (column < H &&
            !(board[row - 1][column + 1].is_empty()) &&
            board[row - 1][column + 1].piece.color == WHITE)
            locations.emplace_back(Location(row - 1, column + 1));

    }
    return locations;
}

vector<Location> State::direct_course_bishop(const Piece& piece) const{

    vector<Location> locations;
    vector<Location> diagonals = get_diagonals(piece.location);
    for(Location location:diagonals) {
        if(player_piece_on_location(location,piece.color))
            continue;

        vector<Location> current_diagonal = get_diagonal(location, piece.location);
        if (current_diagonal.empty()) {
            locations.push_back(location);
            continue;
        }

        bool check=true;
        for (Location loc:current_diagonal) { //check if there other pieces on the
            if (!(board[loc.row][loc.column].is_empty())) { //diagonal between the locations
                check = false;
                break;
            }
        }
        if(check)
            locations.push_back(location);
    }

    return locations;
}


/* a player's piece can move to a location if there is a direct course (or a possible castling or en passant
 * capture) from the location of the piece to the target location and the player's king isn't under threat as
 * a result of the move. */
const vector<Location> State::available_locations(const Piece& piece) const{

    vector<Location> locations = direct_course(piece);
    //check possile en passant
    int column = piece.location.column;
    int row = piece.location.row;
    if(piece.type == PAWN) {

        if (piece.color == WHITE) {
            //left en_passant_capture
            if (column > A &&
                row == 4 &&
                en_passant_flag &&
                en_passant_location == Location(5, column - 1))
                locations.emplace_back(Location(5, column - 1));

            //right en_passant_capture
            if (column < H &&
                row == 4 &&
                en_passant_flag &&
                en_passant_location == Location(5, column + 1))
                locations.emplace_back(Location(5, column + 1));
        }
        else {
            //left en_passant_capture
            if (column > A &&
                row == 3 &&
                en_passant_flag &&
                en_passant_location == Location(2, column - 1))
                locations.emplace_back(Location(row - 1, column - 1));

            //right en_passant_capture
            if (column < H &&
                row == 3 &&
                en_passant_flag &&
                en_passant_location == Location(2, column + 1))
                locations.emplace_back(Location(row - 1, column + 1));
        }
    }

    else if(piece.type==KING){

        if(piece.color == WHITE){
            if(possible_kingside_castling())
                locations.emplace_back(Location(0,G));
            if(possible_queenside_castling())
                locations.emplace_back(Location(0,C));
        }
        else if(piece.color == BLACK){
            if(possible_kingside_castling())
                locations.emplace_back(Location(7,G));
            if(possible_queenside_castling())
                locations.emplace_back(Location(7,C));
        }
    }

    vector<Location> final_locations;
    for(auto loc:locations){
        State new_state(*this);
        if(piece.type == PAWN && //en_passant_capture
           loc.column != column &&
           board[loc.row][loc.column].is_empty())
            new_state.capture_piece_en_passant(piece.location,loc);
        else if(!(new_state.board[loc.row][loc.column].is_empty()))
            new_state.capture_piece(piece.location,loc);
        else
            new_state.move_piece(piece.location,loc);

        new_state.turn = turn == WHITE ?
                         BLACK : WHITE;
        if(!(new_state.is_in_check(turn)))
            final_locations.push_back(loc);
    }

    return final_locations;
}


vector<Location> State::direct_course(const Piece& piece) const{
    vector<Location> locations;
    // cout<<piece_type_to_string(piece.type)<<endl;
    switch(piece.type) {
        case PAWN:
            locations = direct_course_pawn(piece);
            break;
        case KNIGHT:
            locations = direct_course_knight(piece);
            break;
        case BISHOP:
            locations = direct_course_bishop(piece);
            break;
        case ROOK:
            locations = direct_course_rook(piece);
            break;
        case QUEEN:
            locations = direct_course_queen(piece);
            break;
        case KING:
            locations = direct_course_king(piece);
            break;
    }
    return locations;
}


Move_Type State::move_type(const Piece& piece, const Location& to) const{

    int current_column = piece.location.column;
    if (piece.type == PAWN) {
        if (to.column != current_column) {
            if (board[to.row][to.column].is_empty())
                return EN_PASSANT;
            if(to.row==0||to.row==7)
                return PROMOTION_AND_CAPTURE;
            return CAPTURE;

        }
        if(to.row==0||to.row==7){
            return PROMOTION;
        }
        return REGULAR;
    }

    if(piece.type==KING){
        if(current_column == E &&
           to.column==G)
            return KINGSIDE_CASTLING;
        if(current_column == E &&
           to.column==C)
            return QUEENSIDE_CASTLING;
    }
    if(!(board[to.row][to.column].is_empty()))
        return CAPTURE;
    return REGULAR;

}

void State::add_piece(const piece_type type, const Location& location, const Color color){

    vector<Piece>& pieces = color == WHITE ? white_pieces : black_pieces;
    Piece piece(type,color,location);
    piece.id= static_cast<int>(pieces.size());
    pieces.push_back(piece);
    board[location.row][location.column].piece=piece;
    board[location.row][location.column].empty=false;


}

void State::promotion(const Location& location, const piece_type type){
    Color color = board[location.row][location.column].piece.color;
    remove_piece_in_location(location);
    add_piece(type,location,color);
}

void State::king_side_castling(){
    int row = turn == WHITE ? 0 : 7;
    move_piece(Location(row,H),Location(row,F));
    move_piece(Location(row,E),Location(row,G));
}

void State::queen_side_castling(){
    int row = turn == WHITE ? 0 : 7;
    move_piece(Location(row,A),Location(row,D));
    move_piece(Location(row,E),Location(row,C));
}


void State::make_move(Piece& piece, const Location& to){
    vector<Piece>& pieces = piece.color == WHITE ? white_pieces : black_pieces;
    pieces[piece.id].moves_counter++;
    en_passant_flag = false;
    switch(move_type(piece,to)){
        case EN_PASSANT:
            capture_piece_en_passant(piece.location,to);
            break;
        case CAPTURE:
            capture_piece(piece.location,to);
            break;
        case PROMOTION:
            move_piece(piece.location,to);
            break;
        case PROMOTION_AND_CAPTURE:
            capture_piece(piece.location,to);
            break;
        case KINGSIDE_CASTLING:
            king_side_castling();
            break;
        case QUEENSIDE_CASTLING:
            queen_side_castling();
            break;
        case REGULAR:
            if(piece.type==PAWN && abs(piece.location.row-to.row)==2){
                en_passant_flag = true;
                en_passant_location = Location((piece.location.row+to.row)/2,to.column);
            }

            move_piece(piece.location,to);
    }
    turn = turn == WHITE ? BLACK : WHITE;
}

Color State::getTurn() const {
    return turn;
}

void State::setTurn(const Color turn) {
    State::turn = turn;
}

const vector<Piece> &State::getWhite_pieces() const {
    return white_pieces;
}

void State::setWhite_pieces(const vector<Piece> &white_pieces) {
    State::white_pieces = white_pieces;
}

const vector<Piece> &State::getBlack_pieces() const {
    return black_pieces;
}

void State::setBlack_pieces(const vector<Piece> &black_pieces) {
    State::black_pieces = black_pieces;
}

bool State::isEn_passant_flag() const {
    return en_passant_flag;
}

void State::setEn_passant_flag(bool en_passant_flag) {
    State::en_passant_flag = en_passant_flag;
}

const Location &State::getEn_passant_location() const {
    return en_passant_location;
}

void State::setEn_passant_location(const Location &en_passant_location) {
    State::en_passant_location = en_passant_location;
}

Square State::getSquare (const Location& location) const{
    return board[location.row][location.column];
}

int State::total_available_moves_for_current_player() const{
    vector<Piece> pieces = turn == WHITE ? white_pieces : black_pieces;
    int total_moves = 0;
    for(Piece piece:pieces){
        total_moves += available_locations(piece).size();
    }
    return total_moves;
}

bool State::is_mate() const{
    return is_in_check(turn) && total_available_moves_for_current_player() == 0;
}

bool State::is_stale_mate() const{
    return !(is_in_check(turn)) && total_available_moves_for_current_player() == 0;
}

void print_board(const State& state){
    for (int row = 0; row < 8; row++) {
        for (int column = 0; column < 8; column++) {
            Square square = state.getSquare(Location(row,column));
            if(!square.is_empty()) {
                cout << "(" + color_to_string(square.piece.color) + ","
                        + piece_type_to_string(square.piece.type) + ")" + " ";
            }
            else
                cout<<"empty ";
        }
        cout<<endl;
    }
}


