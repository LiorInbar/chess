//
// Created by Lior on 19/03/2017.
//

#ifndef CHESS_STATE_H
#define CHESS_STATE_H

#include "location_piece_square.h"

typedef enum {UP, DOWN, LEFT, RIGHT} Direction;
typedef enum {REGULAR, CAPTURE, EN_PASSANT, KINGSIDE_CASTLING, QUEENSIDE_CASTLING, PROMOTION, PROMOTION_AND_CAPTURE} Move_Type;




class State {
    vector<vector<Square>> board;
    Color turn;
    vector<Piece> white_pieces;
    vector<Piece> black_pieces;
    bool en_passant_flag = false; // current en passant possibility and location
    Location en_passant_location;

    void remove_piece_in_location(Location location);
    void capture_piece(Location from, Location to);
    void capture_piece_en_passant(Location from, Location to);
    void move_piece(Location from, Location to);

public:
    State(Color new_turn, vector<Piece> new_white_pieces,
          vector<Piece> new_black_pieces);
    ~State()= default;
    State(const State& original_state);
    State(State&& state) noexcept = default;
    State& operator=(State&& state)= default;
    Square getSquare (Location location) const;

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

    State& operator=(State state);

    vector<Location> direct_course_pawn(Piece piece);

    vector<Location> direct_course_knight(Piece piece);

    vector<Location> direct_course_bishop(Piece piece);

    vector<Location> direct_course_rook(Piece piece);

    vector<Location> direct_course_queen(Piece piece);

    vector<Location> direct_course_king(Piece piece);

    bool player_piece_on_location(Location location, Color player);

    vector<Location> direct_course(Piece piece);

    void promotion(Location location);
    int available_moves_for_current_player();

public:


    /*location of player's king*/
    Location king_location(Color player);

    /*check if player is in check position (against him)*/
    bool is_in_check(Color player);

    bool is_mate();
    bool is_stale_mate();

    /*  check if location is in a direct threat from a piece of threatening_player
     * location is threatened by a player if one of the player's pieces threatening it.
     * except for pawns, a piece is threatening a square if it has a direct course to it (see
     * function "direct_course")*/
    bool threatened_square(Location location, Color threatening_player);

    bool possible_queenside_castling();

    bool possible_kingside_castling();

    Move_Type move_type(Piece piece, Location to);

    vector<Location> available_locations(Piece piece);

    void make_move(Piece* piece, Location to);


    void add_piece(piece_type type, Location location);

    void king_side_castling();

    void queen_side_castling();

};

void print_board(const State& state);

#endif //CHESS_STATE_H
