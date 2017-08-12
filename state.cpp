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

    //board initialization
    for (int row = 0; row < 8; row++){
        board.emplace_back(vector<Square>());
        for (int column = 0; column < 8; column++){
            board[row].push_back(Square(row,column));
        }
    }

    //put white pieces in the board
    for (Piece piece:white_pieces) {
        Square &square = board[piece.location.row][piece.location.column];
        square.piece = piece;
        square.empty = false;

    }

    //put black pieces in the board
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



Location State::king_location(Color color) const{ //nothing to explain
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
    if(direction == LEFT) { // l1 is right to l2 => go left
        for (int column = l1.column-1; column > l2.column; column--)
            row_vector.emplace_back(Location(l1.row,column));
    }
    else{ // l1 is left to l2 => go right
        for (int column = l1.column+1; column < l2.column; column++)
            row_vector.emplace_back(Location(l1.row,column));
    }
    return row_vector;
}

/*Return vector of all the locations in the column between
 * l1 and l2, not including*/
vector<Location> get_column(const Location& l1, const Location& l2){

    vector<Location> column_vector;
    Direction direction = l1.row > l2.row ? DOWN : UP;
    if(direction==DOWN) { // l1 is higher then l2 => go down
        for (int row = l1.row-1; row > l2.row; row--)
            column_vector.emplace_back(Location(row,l1.column));
    }
    else{  // l1 is lower then l2 => go up
        for (int row = l1.row+1; row < l2.row; row++)
            column_vector.emplace_back(Location(row,l1.column));
    }
    return column_vector;
}

/*return vector of all the locations in the diagonal between
 * l1 and l2, not including*/
vector<Location> get_diagonal(const Location& l1, const Location& l2){

    vector<Location> diagonal_vector;
    Direction horizontal_direction = l1.column > l2.column ? LEFT : RIGHT;
    Direction vertical_direction = l1.row > l2.row ? DOWN : UP;

    // l2 is down and left to l1 => go left and down
    if(horizontal_direction == LEFT && vertical_direction == DOWN) {
        for (int row_index = l1.row-1, column_index = l1.column-1;
             row_index > l2.row; row_index--, column_index--){
            diagonal_vector.emplace_back(Location(row_index,column_index));
        }
    }

    // l2 is aove and left to l1 => go left and up
    else if(horizontal_direction == LEFT && vertical_direction == UP) {
        for (int row_index = l1.row+1, column_index = l1.column-1;
             row_index < l2.row; row_index++, column_index--){
            diagonal_vector.emplace_back(Location(row_index,column_index));
        }
    }

    // l2 is down and right to l1 => go right and down
    else if(horizontal_direction == RIGHT && vertical_direction == DOWN) {
        for (int row_index = l1.row-1, column_index = l1.column+1;
             row_index > l2.row; row_index--, column_index++){
            diagonal_vector.emplace_back(Location(row_index,column_index));
        }
    }

    // l2 is above and right to l1 => go right and up
    else if(horizontal_direction == RIGHT && vertical_direction == UP) {
        for (int row_index = l1.row+1, column_index = l1.column+1;
             row_index < l2.row; row_index++, column_index++){
            diagonal_vector.emplace_back(Location(row_index,column_index));
        }
    }
    return diagonal_vector;
}

bool State::threatened_square(const Location& location, const Color threatening_player) const{

    /*for every piece of the threatening_player, check if the piece has a direct course
     * to the location*/
    vector<Piece> pieces = threatening_player == WHITE ? white_pieces : black_pieces;
    for(Piece piece: pieces){
        for(Location loc: direct_course(piece)){
            if(loc==location)
                return true;          
        }
    }
    return false;
}
bool State::is_in_check(Color color) const{
    /*check if the king square is under threat*/
    Color opposition = color == WHITE ? BLACK : WHITE;
    return threatened_square(king_location(color), opposition);

}

bool State::possible_queenside_castling() const{

    //check if queenside castling is legal for the current player

    if(turn == WHITE){

        return !(board[0][E].is_empty()) && //the king and rook are on their squares
               !(board[0][A].is_empty()) &&
               board[0][E].piece.color == WHITE &&
               board[0][A].piece.color == WHITE &&
               board[0][E].piece.type == KING &&
               board[0][A].piece.type == ROOK &&
               board[0][E].piece.moves_counter == 0 && //the king and rook didnt move yet
               board[0][A].piece.moves_counter == 0 &&
               board[0][B].is_empty() && //squares between king and rook squares are empty
               board[0][C].is_empty() &&
               board[0][D].is_empty() &&
               !threatened_square(Location(0,E),BLACK) && //squares between king and rook squares are not threatened
               !threatened_square(Location(0,D),BLACK) &&
               !threatened_square(Location(0,C),BLACK);
    }
    if(turn == BLACK){

        return !board[7][E].is_empty() && //the king and rook are on their squares
               !board[7][A].is_empty() &&
                board[7][E].piece.color == BLACK &&
                board[7][A].piece.color == BLACK &&
                board[7][E].piece.type == KING &&
                board[7][A].piece.type == ROOK &&
                board[7][E].piece.moves_counter == 0 &&//the king and rook didnt move yet
                board[7][A].piece.moves_counter == 0 &&
                board[7][B].is_empty() && //squares between king and rook squares are empty
                board[7][C].is_empty() &&
                board[7][D].is_empty() &&
                !threatened_square(Location(7,E),WHITE) && //squares between king and rook squares are not threatened
                !threatened_square(Location(7,D),WHITE) &&
                !threatened_square(Location(7,C),WHITE);
    }

}

bool State::possible_kingside_castling() const{

//check if kingside castling is legal for the current player

    if(turn == WHITE){
        return !board[0][E].is_empty() && //the king and rook are on their squares
               !board[0][H].is_empty() &&
                board[0][E].piece.color == WHITE &&
                board[0][H].piece.color == WHITE &&
                board[0][E].piece.type == KING &&
                board[0][H].piece.type == ROOK &&
                board[0][E].piece.moves_counter == 0 && //the king and rook didnt move yet
                board[0][H].piece.moves_counter == 0 &&
                board[0][G].is_empty() &&  //squares between king and rook squares are empty
                board[0][F].is_empty() &&
               !threatened_square(Location(0,E),BLACK) &&  //squares between king and rook squares are not threatened
               !threatened_square(Location(0,F),BLACK) &&
               !threatened_square(Location(0,G),BLACK);
    }
    if(turn == BLACK){
        return !board[7][E].is_empty() && //the king and rook are on their squares
               !board[7][H].is_empty() &&
                board[7][E].piece.color == BLACK &&
                board[7][H].piece.color == BLACK &&
                board[7][E].piece.type == KING &&
                board[7][H].piece.type == ROOK &&
                board[7][E].piece.moves_counter == 0 && //the king and rook didnt move yet
                board[7][H].piece.moves_counter == 0 &&
                board[7][G].is_empty() &&  //squares between king and rook squares are empty
                board[7][F].is_empty() &&
               !threatened_square(Location(7,E),WHITE) &&  //squares between king and rook squares are not threatened
               !threatened_square(Location(7,F),WHITE) &&
               !threatened_square(Location(7,G),WHITE);
    }

}

void State::remove_piece_in_location(const Location &location){

    Piece &piece = board[location.row][location.column].piece;
    vector<Piece>& pieces = piece.color == WHITE ? white_pieces : black_pieces;
    int id = piece.id;
    //remove the piece from the pieces vector and updating the id's
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
    pieces[id].location = to; //updating the pieces vector
    board[to.row][to.column].piece = pieces[id]; //updating the board
    board[from.row][from.column].empty=true;
    board[to.row][to.column].empty= false;

}

void State::capture_piece(const Location& from, const Location& to){
    remove_piece_in_location(to);
    move_piece(from,to);
}


void State::capture_piece_en_passant(const Location& from, const Location& to){
    Location captured_pawn_location = //calculate the location of the captured piece
            turn == WHITE ? Location(to.row-1, to.column): Location(to.row+1, to.column);
    remove_piece_in_location(captured_pawn_location);
    move_piece(from,to);
}

//get all the locations in the same diagonal as 'location'
vector<Location> get_diagonals(const Location& location) {
    vector<Location> locations;

    //top right diagonal
    for (int row_index = location.row + 1, column_index = location.column + 1;
         row_index <= 7 && column_index <= H; row_index++, column_index++) {
        locations.emplace_back(Location(row_index, column_index));
    }
    //top left diagonal
    for (int row_index = location.row + 1, column_index = location.column - 1;
         row_index <= 7 && column_index >= A; row_index++, column_index--) {
        locations.emplace_back(Location(row_index, column_index));
    }
    //bottom left diagonal
    for (int row_index = location.row - 1, column_index = location.column - 1;
         row_index >= 0 && column_index >= A; row_index--, column_index--) {
        locations.emplace_back(Location(row_index, column_index));
    }

    //bottom right diagonal
    for (int row_index = location.row - 1, column_index = location.column + 1;
         row_index >= 0 && column_index <= H; row_index--, column_index++) {
        locations.emplace_back(Location(row_index, column_index));
    }
    return locations;
}

//check if the 'to' location can be reached from the 'from' location with a knight move.
bool is_knight_move(const Location& from, const Location& to){

    return ((abs(from.row - to.row) == 2 //distance of two rows and one column
             && abs(from.column - to.column) == 1)
            || (abs(from.row - to.row) == 1 //distance of one row and two columns
                && abs(from.column - to.column) == 2));
}

//get all the squares that can be reached with a knight move to location.
vector<Location> get_knight_moves(const Location& location){
    vector<Location> locations;
    //check all the squares with distance of two squares or less
    int first_row = max(location.row-2, 0),
            last_row = min(7, location.row+2),
            first_column = max(location.column-2, 0),
            last_column = min(7, location.column+2);

    for (int row_index = first_row; row_index <= last_row; row_index++){
        for (int column_index = first_column; column_index <= last_column; column_index++) {
             Location loc(row_index, column_index);
            if (is_knight_move(loc,location))
                locations.emplace_back(loc);
        }
    }
    return locations;
}

//get all the squares with distance of one square from 'location'.
vector<Location> one_squares_distance(const Location& location){
    vector<Location> locations;
    int first_row = max(location.row-1, 0),
            last_row = min(7, location.row+1),
            first_column = max(location.column-1, 0),
            last_column = min(7, location.column+1);

    for (int row_index = first_row; row_index <= last_row; row_index++){
        for (int column_index = first_column; column_index <= last_column; column_index++) {
            if (row_index != location.row || column_index != location.column)
                locations.emplace_back(Location(row_index, column_index));
        }
    }
    return locations;
}

vector<Location> State::direct_course_knight(const Piece& piece) const{
    vector<Location> locations;
    //get the locations in knight move distance from 'piece' location.
    vector<Location> potential_locations = get_knight_moves(piece.location);
    for(Location loc:potential_locations) {
        if (!player_piece_on_location(loc, piece.color))
            //check if there is a piece from color 'color' on 'location'
            locations.push_back(loc);
    }
    return locations;
}


bool State::player_piece_on_location(const Location& location, Color color) const{
    if(board[location.row][location.column].is_empty())
        return false;
    return board[location.row][location.column].piece.color == color;
}

vector<Location> State::direct_course_rook(const Piece& piece) const{
    vector<Location> locations;
    //check locations in the piece's row.
    for(int column = 0; column < 8; column++){
        //ignore piece's square
        if(column == piece.location.column)
            continue;
        //ignore square with pieces in the same color as piece
        if(player_piece_on_location(Location(piece.location.row, column), piece.color))
            continue;
        //check that the row between the location and the piece's location is empty
        vector<Location> row = get_row(piece.location, Location(piece.location.row, column));
        bool check=true;
        for (Location loc:row) {
            if (!board[loc.row][loc.column].is_empty()) {
                check=false;
                break;
            }
        }
        if(check)
            locations.push_back(Location(piece.location.row,column));
    }

    //check locations in the piece's column.
    for(int row = 0; row < 8; row++){
        //ignore piece's square
        if(row == piece.location.row)
            continue;
        //ignore square with pieces in the same color as piece
        if(player_piece_on_location(Location(row,piece.location.column), piece.color))
            continue;
        //check that the column between the location and the piece's location is empty
        vector<Location> column = get_column(piece.location, Location(row,piece.location.column));
        bool check=true;
        for (Location loc:column) {
            if (!board[loc.row][loc.column].is_empty()) {
                check = false;
                break;
            }
        }
        if(check)
            locations.push_back(Location(row,piece.location.column));
    }

    return locations;
}

vector<Location> State::direct_course_bishop(const Piece& piece) const{

    vector<Location> locations;
    //check locations in the piece's diagonals.
    vector<Location> diagonals = get_diagonals(piece.location);
    for(Location location:diagonals) {
        //ignore square with pieces in the same color as piece
        if(player_piece_on_location(location,piece.color))
            continue;

        //check that the diagonal between the location and the piece's location is empty
        vector<Location> diagonal = get_diagonal(location, piece.location);
        bool check=true;
        for (Location loc:diagonal) {
            if (!(board[loc.row][loc.column].is_empty())) {
                check = false;
                break;
            }
        }
        if(check)
            locations.push_back(location);
    }
    return locations;
}



vector<Location> State::direct_course_queen(const Piece& piece)const{

    /*check locations in the piece's column, row and diagonals
     * using the direct course functions of rook and bishop*/
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
    /*get all the locations with distance of one square of the piece
    without a piece in the same color*/
    vector<Location> one_squares_distance_locations = one_squares_distance(piece.location);
    vector<Location> locations;
    for(Location location:one_squares_distance_locations){
        if(!player_piece_on_location(location, piece.color))
            locations.push_back(location);
    }
    return locations;
}

Location State::getPromotion_location() const
{
    return promotion_location;
}

void State::setPromotion_location(const Location &value)
{
    promotion_location = value;
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


/* a player's piece can move to a location if there is a direct course (or a possible castling or en passant
 * capture) from the location of the piece to the target location and the player's king isn't under threat as
 * a result of the move. */
vector<Location> State::available_locations(const Piece& piece) const{

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

    else if(piece.type==KING){ //check possible castling

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
    /*for every possible locations, make the move to the location
     * in a temp state and check if the player's king under threat*/
    vector<Location> final_locations;
    for(auto loc:locations){
        //make the move in a temp state
        State new_state(*this);
        if(piece.type == PAWN && //en_passant_capture
           loc.column != column &&
           board[loc.row][loc.column].is_empty())
                new_state.capture_piece_en_passant(piece.location,loc);
        else if(!(new_state.board[loc.row][loc.column].is_empty()))
            new_state.capture_piece(piece.location,loc); //capture
        else
            new_state.move_piece(piece.location,loc);
       // check if the player's king under threat
        new_state.turn = turn == WHITE ?
                         BLACK : WHITE;
        if(!(new_state.is_in_check(turn)))
            final_locations.push_back(loc);
    }

    return final_locations;
}


vector<Location> State::direct_course(const Piece& piece) const{
    vector<Location> locations;
    //call the relevant direct_course function
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

    int column = piece.location.column;
    if (piece.type == PAWN) {
        if (to.column != column) {
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
        if(column == E &&
           to.column==G)
            return KINGSIDE_CASTLING;
        if(column == E &&
           to.column==C)
            return QUEENSIDE_CASTLING;
    }
    if(!(board[to.row][to.column].is_empty()))
        return CAPTURE;
    return REGULAR;

}

void State::add_piece(piece_type type, const Location& location, Color color){

    vector<Piece>& pieces = color == WHITE ? white_pieces : black_pieces;
    Piece piece(type,color,location); //add the piece to the pieces vector
    piece.id= static_cast<int>(pieces.size());
    pieces.push_back(piece);
    board[location.row][location.column].piece=piece; //update the board
    board[location.row][location.column].empty=false;
}

void State::promotion(const piece_type type){
    Location location = getPromotion_location();
    Color color = board[location.row][location.column].piece.color;
    remove_piece_in_location(location);
    add_piece(type,location,color);
    turn = turn == WHITE ? BLACK : WHITE; //change player
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
    Move_Type type = move_type(piece,to);
    switch(type){
        case EN_PASSANT:
            capture_piece_en_passant(piece.location,to);
            break;
        case CAPTURE:
            capture_piece(piece.location,to);
            break;
        case PROMOTION:
            move_piece(piece.location,to);
            promotion_location = to;
            break;
        case PROMOTION_AND_CAPTURE:
            capture_piece(piece.location,to);
            promotion_location = to;
            break;
        case KINGSIDE_CASTLING:
            king_side_castling();
            break;
        case QUEENSIDE_CASTLING:
            queen_side_castling();
            break;
        case REGULAR:
            //if double square pawn move, set en passant flag and update location
            if(piece.type==PAWN && abs(piece.location.row-to.row)==2){
                en_passant_flag = true;
                en_passant_location = Location((piece.location.row+to.row)/2,to.column);
            }
            move_piece(piece.location,to);
    }
    if((type!=PROMOTION)&&(type!=PROMOTION_AND_CAPTURE))
        turn = turn == WHITE ? BLACK : WHITE; //change player
}

int State::total_available_moves_for_current_player() const{
    vector<Piece> pieces = turn == WHITE ? white_pieces : black_pieces;
    int total_moves = 0;
    for(Piece piece:pieces){ //sum all available locations of the current player's pieces
        total_moves += available_locations(piece).size();
    }
    return total_moves;
}

bool State::is_mate() const{
    //if the current player has no available moves and is in check - checkmate
    return is_in_check(turn) && total_available_moves_for_current_player() == 0;
}

//if the current player has no available moves and is not in check - stalemate
bool State::is_stale_mate() const{
    return !(is_in_check(turn)) && total_available_moves_for_current_player() == 0;
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


