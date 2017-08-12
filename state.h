//
// Created by Lior on 19/03/2017.
//

#ifndef CHESS_STATE_H
#define CHESS_STATE_H

#include "location_piece_square.h"

typedef enum {UP, DOWN, LEFT, RIGHT} Direction;
typedef enum {REGULAR, CAPTURE, EN_PASSANT, KINGSIDE_CASTLING,
              QUEENSIDE_CASTLING, PROMOTION, PROMOTION_AND_CAPTURE} Move_Type;




class State {
private:
    vector<vector<Square>> board;
    Color turn;
    vector<Piece> white_pieces;
    vector<Piece> black_pieces;
    bool en_passant_flag = false; // current en passant possibility and location
    Location en_passant_location;
    Location promotion_location;

    //implementations of the function direct course for specific piece

    vector<Location> direct_course_pawn(const Piece& piece) const;
    vector<Location> direct_course_knight(const Piece& piece) const;
    vector<Location> direct_course_bishop(const Piece& piece) const;
    vector<Location> direct_course_rook(const Piece& piece) const;
    vector<Location> direct_course_queen(const Piece& piece) const;
    vector<Location> direct_course_king(const Piece& piece) const;

    //remove from the board the piece which locate on 'location'.
    void remove_piece_in_location(const Location& location);

    //perform a capture - the piece on location 'from' capture the piece on location 'to'.
    void capture_piece(const Location& from, const Location& to);

    /*perform en passant capture - the piece on location 'from' capture the piece on
     * the row below location 'to' (if the capture piece is white) or the row above
     * location 'to' (if the capture piece is white). see: https://en.wikipedia.org/wiki/En_passant.*/
    void capture_piece_en_passant(const Location& from, const Location& to);

    //move a piece from location 'from' to location 'to'.
    void move_piece(const Location& from, const Location& to);

    /*add a piece of type 'type' and color 'color' on 'location'.
     used when performing promotion*/
    void add_piece(piece_type type, const Location& location, Color color);

    /*get the location of the king from color 'color'*/
    Location king_location(Color color) const;

    //perform king side castling.
    void king_side_castling();

    //perform queen side castling.
    void queen_side_castling();

public:

    State()= default;
    State(Color new_turn, vector<Piece> new_white_pieces,
          vector<Piece> new_black_pieces);
    ~State()= default;
    State(const State& original_state);
    State(State&& state) noexcept = default;
    State& operator=(State&& state)= default;
    State& operator=(const State& state)=default;

    /*geters and seters*/
    Color getTurn() const;
    void setTurn(Color turn);
    const vector<Piece> &getWhite_pieces() const;
    void setWhite_pieces(const vector<Piece> &white_pieces);
    const vector<Piece> &getBlack_pieces() const;
    void setBlack_pieces(const vector<Piece> &black_pieces);
    bool isEn_passant_flag() const;
    void setEn_passant_flag(bool en_passant_flag);
    const Location &getEn_passant_location() const;
    void setEn_passant_location(const Location &en_passant_location);
//------------------------------------------
    //get the square in the location 'location'
    Square getSquare (const Location& location) const;

    //check if there is a piece from color 'color' on 'location'.
    bool player_piece_on_location(const Location& location,Color color) const;

    /*get all the locations which 'piece' has a "direct course" into them.
     * we say that a piece has a direct course to a location if it can
     * move to this location in the current state, assuming that the resulting
     * move will not put the player which the piece is belong to in check
     * position. Therefore, a piece can move to a location if it has a direct
     * course to this location and the resulting move will not put the piece's
     * player in a check position.*/
    vector<Location> direct_course(const Piece& piece) const;

    /*get the total number of available moves of the current player -
     * used to check if the current state is mate or stalemate*/
    int total_available_moves_for_current_player() const;

    /*check if the player with color 'color' is in check position (against him)*/
    bool is_in_check(Color color) const;

    /*check if the state is mate state*/
    bool is_mate() const;

    /*check if the state is stalemate state*/
    bool is_stale_mate() const;

    /*  check if 'location' is in a direct threat from a piece of threatening_player
     * location is threatened by a player if one of the player's pieces threatening it.
     * a piece is threatening a square if it has a direct course to it (see
     * function "direct_course")*/
    bool threatened_square(const Location& location, Color threatening_player) const;

    /* Check if the current player can perform queenside castling.*/
    bool possible_queenside_castling() const;

    /* Check if the current player can perform kingside castling.*/
    bool possible_kingside_castling() const;

    /*return the move type of moving 'piece' to 'location'. See type
     * "Move_type" definition. */
    Move_Type move_type(const Piece& piece, const Location& to) const;

    /*return all the locations which 'piece' can move into in the state.
     * A piece can move to a location if it has a direct course to this
     * location and the resulting move will not put the piece's
     * player in a check position. See function 'direct course'*/
    vector<Location> available_locations(const Piece& piece) const;

    /* make the move defined by moving 'piece' to 'location', possibly
     * involving capture, castling, etc.*/
    void make_move(Piece& piece, const Location& to);

    /* Promote the pawn on 'location' to a piece of type ,type.*/
    void promotion(piece_type type);

    Location getPromotion_location() const;
    void setPromotion_location(const Location &value);
};

/* print the board of 'state' (for testing purposes).*/
void print_board(const State& state);

#endif //CHESS_STATE_H
