//
// Created by Lior on 19/03/2017.
//

#ifndef CHESS_STATE_H
#define CHESS_STATE_H

#include "location_piece_square.h"

typedef enum {UP, DOWN, LEFT, RIGHT} Direction;
typedef enum {REGULAR, CAPTURE, EN_PASSANT, KINGSIDE_CASTLING, QUEENSIDE_CASTLING, PROMOTION, PROMOTION_AND_CAPTURE} Move_Type;




class State {
private:
    vector<vector<Square>> board;
    Color turn;
    vector<Piece> white_pieces;
    vector<Piece> black_pieces;
    bool en_passant_flag = false; // current en passant possibility and location
    Location en_passant_location;



    vector<Location> direct_course_pawn(const Piece& piece) const;
    vector<Location> direct_course_knight(const Piece& piece) const;
    vector<Location> direct_course_bishop(const Piece& piece) const;
    vector<Location> direct_course_rook(const Piece& piece) const;
    vector<Location> direct_course_queen(const Piece& piece) const;
    vector<Location> direct_course_king(const Piece& piece) const;


    void remove_piece_in_location(Location location);
    void capture_piece(const Location& from, const Location& to);
    void capture_piece_en_passant(const Location& from, const Location& to);
    void move_piece(const Location& from, const Location& to);
    void add_piece(const piece_type type, const Location& location, const Color color);
    /*location of player's king*/
    Location king_location(const Color color) const;
    void king_side_castling();
    void queen_side_castling();

public:
    State()= default;
    State(Color new_turn, vector<Piece> new_white_pieces,
          vector<Piece> new_black_pieces);
    ~State()= default;
    State(const State& original_state);
    State(State&& state) noexcept = default;
    State& operator=(State&& state)= default;

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
    State& operator=(const State& state);
    Square getSquare (const Location& location) const;
    bool player_piece_on_location(const Location& location, const Color color) const;
    vector<Location> direct_course(const Piece& piece) const;
    int total_available_moves_for_current_player() const;
    /*check if player is in check position (against him)*/
    bool is_in_check(const Color player) const;
    bool is_mate() const;
    bool is_stale_mate() const;
    /*  check if location is in a direct threat from a piece of threatening_player
     * location is threatened by a player if one of the player's pieces threatening it.
     * except for pawns, a piece is threatening a square if it has a direct course to it (see
     * function "direct_course")*/
    bool threatened_square(const Location& location, Color threatening_player) const;
    bool possible_queenside_castling() const;
    bool possible_kingside_castling() const;
    Move_Type move_type(const Piece& piece, const Location& to) const;
    const vector<Location> available_locations(const Piece& piece) const;
    void make_move(Piece& piece, const Location& to);
    void promotion(const Location& location, piece_type type);

};

void print_board(const State& state);

#endif //CHESS_STATE_H
