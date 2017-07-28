//
// Created by Lior on 19/03/2017.
//

#include "state.h"
#include <iostream>


State::State(Color new_turn, vector<Piece> new_white_pieces,
             vector<Piece> new_black_pieces):
        turn(new_turn),white_pieces(new_white_pieces),
        black_pieces(new_black_pieces),
        en_passant_location(Location(0,0)){
    for (int index = 0; index < 8; index++){
        board.emplace_back(vector<Square>());
        for (int index2 = 0; index2 < 8; index2++){
            board[index].push_back(Square(index,index2));
        }
    }
    update_squares();
}

State::State(const State& original_state):   State(original_state.turn, original_state.white_pieces,
original_state.black_pieces) {
    en_passant_flag=original_state.en_passant_flag;
    en_passant_location=original_state.en_passant_location;
}


Location State::king_location(Color player) { //nothing to explain
    vector<Piece> pieces = player == WHITE ?
                           white_pieces :
                           black_pieces;
    for(Piece piece:pieces){
        if(piece.type == KING)
            return piece.location;
    }
}

/*return vector of all the locations in the row between
 * l1 and l2*/
vector<Location> get_row(Location l1, Location l2){
    vector<Location> row_vector;
    Direction d = l1.column > l2.column ? LEFT : RIGHT;
    if(d == LEFT) {
        for (int column = l1.column-1; column >= l2.column; column--){
            row_vector.emplace_back(Location(l1.row,column));
        }
    }
    else{
        for (int column = l1.column+1; column <= l2.column; column++){
            row_vector.emplace_back(Location(l1.row,column));
        }
    }
    return row_vector;
}

/*Return vector of all the locations in the column between
 * l1 and l2*/
vector<Location> get_column(Location l1, Location l2){
    vector<Location> column_vector;
    Direction d = l1.row > l2.row ? DOWN : UP;
    if(d==DOWN) {
        for (int index = l1.row-1; index >= l2.row; index--){
            column_vector.emplace_back(Location(index,l1.column));
        }
    }
    else{
        for (int index = l1.row+1; index <= l2.row; index++){
            column_vector.emplace_back(Location(index,l1.column));
        }
    }
    return column_vector;
}

/*return vector of all the locations in the diagonal between
 * l1 and l2*/
vector<Location> get_diagonal(Location l1, Location l2){
    vector<Location> diagonal_vector;
    Direction d1 = l1.column > l2.column ? LEFT : RIGHT;
    Direction d2 = l1.row > l2.row ? DOWN : UP;

    if(d1 == LEFT && d2 == DOWN) {
        for (int row_index = l1.row-1, column_index = l1.column-1;
             row_index > l2.row; row_index--, column_index--){
            diagonal_vector.emplace_back(Location(row_index,column_index));
        }
    }

    else if(d1 == LEFT && d2 == UP) {
        for (int row_index = l1.row+1, column_index = l1.column-1;
             row_index < l2.row; row_index++, column_index--){
            diagonal_vector.emplace_back(Location(row_index,column_index));
        }
    }
    else if(d1 == RIGHT && d2 == DOWN) {
        for (int row_index = l1.row-1, column_index = l1.column+1;
             row_index > l2.row; row_index--, column_index++){
            diagonal_vector.emplace_back(Location(row_index,column_index));
        }
    }
    else if(d1 == RIGHT && d2 == UP) {

        for (int row_index = l1.row+1, column_index = l1.column+1;
             row_index < l2.row; row_index++, column_index++){
            diagonal_vector.emplace_back(Location(row_index,column_index));
        }
    }
    return diagonal_vector;
}



bool State::threatened_square(Location location, Color threatening_player) {
    vector<Piece> pieces = threatening_player == WHITE ?
                           white_pieces :
                           black_pieces;
    for(Piece piece: pieces){
        for(Location loc: direct_course(piece)){
            if(loc==location){
                return true;
            }
        }
    }
    return false;
}
bool State::is_in_check(Color player){
    /*check if the king square is under threat*/
    Color opposition = player == WHITE ?
                       BLACK :
                       WHITE;

    return threatened_square(king_location(player), opposition);

}

bool State::possible_queenside_castling(){

    if(turn == WHITE){
        return !board[0][E].is_empty() &&
               !board[0][A].is_empty() &&
               board[0][B].is_empty() &&
               board[0][C].is_empty() &&
               board[0][D].is_empty() &&
               board[0][E].piece->type == KING &&
               board[0][A].piece->type == ROOK &&
               board[0][E].piece->moves_counter == 0 &&
               board[0][A].piece->moves_counter == 0 &&
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
               board[7][E].piece->type == KING &&
               board[7][A].piece->type == ROOK &&
               board[7][E].piece->moves_counter == 0 &&
               board[7][A].piece->moves_counter == 0 &&
               !threatened_square(Location(7,E),WHITE) &&
               !threatened_square(Location(7,D),WHITE) &&
               !threatened_square(Location(7,C),WHITE);
    }

}

bool State::possible_kingside_castling(){

    if(turn == WHITE){
        return !board[0][E].is_empty() &&
               !board[0][H].is_empty() &&
               board[0][G].is_empty() &&
               board[0][F].is_empty() &&
               board[0][E].piece->type == KING &&
               board[0][H].piece->type == ROOK &&
               board[0][E].piece->moves_counter == 0 &&
               board[0][H].piece->moves_counter == 0 &&
               !threatened_square(Location(0,E),BLACK) &&
               !threatened_square(Location(0,F),BLACK) &&
               !threatened_square(Location(0,G),BLACK);
    }
    if(turn == BLACK){
        return !board[7][E].is_empty() &&
               !board[7][H].is_empty() &&
               board[7][G].is_empty() &&
               board[7][F].is_empty() &&
               board[7][E].piece->type == KING &&
               board[7][H].piece->type == ROOK &&
               board[7][E].piece->moves_counter == 0 &&
               board[7][H].piece->moves_counter == 0 &&
               !threatened_square(Location(7,E),WHITE) &&
               !threatened_square(Location(7,F),WHITE) &&
               !threatened_square(Location(7,G),WHITE);
    }

}

void State::remove_piece_in_location(Location location){
    int index=0;
    for(Piece piece: white_pieces){
        if (piece.location == location) {
            white_pieces.erase(white_pieces.begin() + index);
            return;
        }
        index++;
    }
    index=0;
    for(Piece piece: black_pieces){
        if (piece.location == location) {
            black_pieces.erase(black_pieces.begin() + index);
            return;
        }
        index++;
    }
}

void State::move_piece(Location from, Location to){

    board[from.row][from.column].piece->location = to;
    board[to.row][to.column].piece = board[from.row][from.column].piece;
    board[from.row][from.column].piece = nullptr;
}

void State::capture_piece(Location from, Location to){
    remove_piece_in_location(to);
    move_piece(from,to);
}


void State::capture_piece_en_passant(Location from, Location to){
    Location captured_pawn_location = turn == WHITE ?
                                      Location(to.row-1, to.column):
                                      Location(to.row+1, to.column);
    remove_piece_in_location(captured_pawn_location);
    move_piece(from,to);
}




vector<Location> get_diagonals(Location location) {
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

bool is_knight_move(Location from, Location to){//check if the target location can be reached from the  piece location with a knight move.

    return ((abs(from.row - to.row) == 2
             && abs(from.column - to.column) == 1)
            || (abs(from.row - to.row) == 1
                && abs(from.column - to.column) == 2));
}

vector<Location> two_squares_distance(Location location){
    vector<Location> locations;
    for (int row_index = max(location.row-2, 0); row_index <= min(7, location.row+2); row_index++){
        for (int column_index = max(location.column-2, 0); column_index <= min(7, location.column+2); column_index++) {
            if (row_index != location.row || column_index != location.column)
                locations.emplace_back(Location(row_index, column_index));
        }
    }
    return locations;
}

vector<Location> one_squares_distance(Location location){
    vector<Location> locations;
    for (int row_index = max(location.row-1, 0); row_index <= min(7, location.row+1); row_index++){
        for (int column_index = max(location.column-1, 0); column_index <= min(7, location.column+1); column_index++) {
            if (row_index != location.row || column_index != location.column)
                locations.emplace_back(Location(row_index, column_index));
        }
    }
    return locations;
}

vector<Location> State::direct_course_knight(Piece piece){
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


bool State::player_piece_on_location(Location location, Color player){
    if(board[location.row][location.column].is_empty())
        return false;
    return board[location.row][location.column].piece->color == player;
}

vector<Location> State::direct_course_rook(Piece piece) {
    vector<Location> locations;
    for(int column = 0; column < 8; column++){
        if(column == piece.location.column)
            continue;
        if(player_piece_on_location(Location(piece.location.row, column), piece.color))
            continue;
        vector<Location> row = get_row(piece.location, Location(piece.location.row, column));
        if (row.size() == 1) {
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
        if (column.size() == 1) {
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

vector<Location> State::direct_course_queen(Piece piece){
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

vector<Location> State::direct_course_king(Piece piece) {
    vector<Location> one_squares_distance_locations = one_squares_distance(piece.location);
    vector<Location> locations;
    for(Location location:one_squares_distance_locations){
        if(!player_piece_on_location(location, piece.color))
            locations.push_back(location);
    }
    return locations;
}

vector<Location> State::direct_course_pawn(Piece piece) {
    vector<Location> locations;
    if (piece.color == WHITE) {
        if (board[piece.location.row + 1][piece.location.column].is_empty()) { //regular pawn move
            locations.emplace_back(Location(piece.location.row + 1, piece.location.column));
            if (piece.location.row == 1 && board[3][piece.location.column].is_empty()) //double square pawn move
                locations.emplace_back(Location(3, piece.location.column));
        }
        //left capture
        if (piece.location.column > A &&
            !(board[piece.location.row + 1][piece.location.column - 1].is_empty()) &&
            board[piece.location.row + 1][piece.location.column - 1].piece->color == BLACK)
            locations.emplace_back(Location(piece.location.row + 1, piece.location.column - 1));

        //right capture
        if (piece.location.column < H &&
            !(board[piece.location.row + 1][piece.location.column + 1].is_empty()) &&
            board[piece.location.row + 1][piece.location.column + 1].piece->color == BLACK)
            locations.emplace_back(Location(piece.location.row + 1, piece.location.column + 1));

    }
    if (piece.color == BLACK) {
        if (board[piece.location.row - 1][piece.location.column].is_empty()) {
            locations.emplace_back(Location(piece.location.row - 1, piece.location.column)); //regular pawn move
            if (piece.location.row == 6 && board[4][piece.location.column].is_empty())
                locations.emplace_back(Location(4, piece.location.column)); //double square pawn move

        }
        //left capture
        if (piece.location.column > A &&
            !(board[piece.location.row - 1][piece.location.column - 1].is_empty()) &&
            board[piece.location.row - 1][piece.location.column - 1].piece->color == WHITE)
            locations.emplace_back(Location(piece.location.row - 1, piece.location.column - 1));

        //right capture
        if (piece.location.column < H &&
            !(board[piece.location.row - 1][piece.location.column + 1].is_empty()) &&
            board[piece.location.row - 1][piece.location.column + 1].piece->color == WHITE)
            locations.emplace_back(Location(piece.location.row - 1, piece.location.column + 1));

    }
    return locations;
}

vector<Location> State::direct_course_bishop(Piece piece) {

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
vector<Location> State::available_locations(Piece piece) {

    vector<Location> locations = direct_course(piece);
    //check possile en passant
    if(piece.type==PAWN) {
        if (piece.color == WHITE) {
            //left en_passant_capture
            if (piece.location.column > A &&
                piece.location.row == 4 &&
                en_passant_flag &&
                en_passant_location == Location(5, piece.location.column - 1))
                locations.emplace_back(Location(5, piece.location.column - 1));

            //right en_passant_capture
            if (piece.location.column < H &&
                piece.location.row == 4 &&
                en_passant_flag &&
                en_passant_location == Location(5, piece.location.column + 1))
                locations.emplace_back(Location(5, piece.location.column + 1));
        }
        else {
            //left en_passant_capture
            if (piece.location.column > A &&
                piece.location.row == 3 &&
                en_passant_flag &&
                en_passant_location == Location(2, piece.location.column - 1))
                locations.emplace_back(Location(piece.location.row - 1, piece.location.column - 1));

            //right en_passant_capture
            if (piece.location.column < H &&
                piece.location.row == 3 &&
                en_passant_flag &&
                en_passant_location == Location(2, piece.location.column + 1))
                locations.emplace_back(Location(piece.location.row - 1, piece.location.column + 1));
        }
    }

    else if(piece.type==KING){

        if(piece.color == WHITE){
            if(possible_kingside_castling())
                locations.emplace_back(Location(0,G));
            if(possible_queenside_castling())
                locations.emplace_back(Location(0,C));
        }
        if(piece.color == BLACK){
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
           loc.column != piece.location.column&&
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


vector<Location> State::direct_course(Piece piece) {
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


Move_Type State::move_type(Piece piece, Location to){
    if (piece.type == PAWN) {
        if (to.column != piece.location.column) {
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
        if(piece.location.column==E &&
           to.column==G)
            return KINGSIDE_CASTLING;
        if(piece.location.column==E &&
           to.column==C)
            return QUEENSIDE_CASTLING;
    }
    if(!(board[to.row][to.column].is_empty()))
        return CAPTURE;
    return REGULAR;

}

void State::add_piece(piece_type type, Location location){

    vector<Piece> pieces = turn == WHITE ?
                           white_pieces :
                           black_pieces;
    Piece piece(type,turn,location);
    pieces.push_back(piece);
    board[location.row][location.column].piece = &(pieces[pieces.size()-1]);

}

void State::promotion(Location location){
    string s;
    cout<<"choose piece"<<endl;
    cin>>s;
    piece_type type;
    if(s=="queen")
        type = QUEEN;
    else if(s=="bishop")
        type = BISHOP;
    else if(s=="knight")
        type = KNIGHT;
    else if(s=="rook")
        type = ROOK;
    else
        promotion(location);
    add_piece(type,location);
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


void State::make_move(Piece* piece, Location to){

    piece->moves_counter++;
    en_passant_flag = false;
    switch(move_type(*piece,to)){
        case EN_PASSANT:
            capture_piece_en_passant(piece->location,to);
            break;
        case CAPTURE:
            capture_piece(piece->location,to);
            break;
        case PROMOTION:
            move_piece(piece->location,to);
            promotion(to);
            break;
        case PROMOTION_AND_CAPTURE:
            capture_piece(piece->location,to);
            promotion(to);
            break;
        case KINGSIDE_CASTLING:
             king_side_castling();
            break;
        case QUEENSIDE_CASTLING:
            queen_side_castling();
            break;
        case REGULAR:
            if(piece->type==PAWN && abs(piece->location.row-to.row)==2){
                en_passant_flag = true;
                en_passant_location = Location((piece->location.row+to.row)/2,to.column);
            }

            move_piece(piece->location,to);
    }
    turn = turn == WHITE ? BLACK : WHITE;
}

Color State::getTurn() const {
    return turn;
}

void State::setTurn(Color turn) {
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

Square State::getSquare (Location location) const{
    return board[location.row][location.column];
}

State& State::operator=(State state) {
    turn =state.turn;
    white_pieces = state.white_pieces;
    black_pieces = state.black_pieces;
    en_passant_location = state.en_passant_location;
    en_passant_flag = state.en_passant_flag;
    for (int index = 0; index < 8; index++){
        for (int index2 = 0; index2 < 8; index2++){
            board[index][index2] = state.board[index][index2];
        }
    }
    return *this;
}
int State::available_moves_for_current_player(){
    vector<Piece> pieces = turn == WHITE ? white_pieces : black_pieces;
    int total_moves = 0;
    for(Piece piece:pieces){
        total_moves += available_locations(piece).size();
    }
    return total_moves;
}
bool State::is_mate() {
    return is_in_check(turn) && available_moves_for_current_player() == 0;
}

bool State::is_stale_mate() {
    return !(is_in_check(turn)) && available_moves_for_current_player() == 0;
}

void State::update_squares() {
    for (int index = 0; index < white_pieces.size(); index++) {
        board[white_pieces[index].location.row][white_pieces[index].location.column].piece = &(white_pieces[index]);
    }
    for (int index = 0; index < black_pieces.size(); index++) {
        board[black_pieces[index].location.row][black_pieces[index].location.column].piece = &(black_pieces[index]);
    }
}

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


