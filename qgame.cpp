#include "qgame.h"

extern bool shutdown_board;
extern QString icons_dir;


Game *Qgame::getGame() const
{
    return game;
}

void Qgame::setGame(Game* value)
{
    game = value;
}

vector<vector<Qsquare *> > Qgame::getQboard() const
{
    return qboard;
}

void Qgame::setQboard(const vector<vector<Qsquare *> > &value)
{
    qboard = value;
}

void Qgame::closeQboard()
{
    shutdown_board = true;
}

void Qgame::openQboard()
{
    shutdown_board = false;

}

void Qgame::update_message(){

    QPlainTextEdit* message_board = findChild<QPlainTextEdit*>("messages");
    //if the game is over, print result
    if(game->getResult() != IN_PROGRESS)
        message_board->setPlainText( QString::fromStdString(result_to_string(game->getResult())));
    //if the game is in progress, print "<current_player> player turn"
    else
      message_board->setPlainText( QString::fromStdString(color_to_string(game->Turn())+" player turn"));

}

void piece_chosen(Qsquare *qsquare)
{
    Game* game = qsquare->getGame();
    Square square = game->getCurrent_state().getSquare(qsquare->getLocation());
    game->setChosen_Piece(square.piece);
    game->setPiece_chosen_check(true);
    //mark the piece's available locations
    vector<Location> locations = game->current_state_available_locations(game->getChosen_Piece());
    for(Location location:locations)
      ((Qgame*)(qsquare->parentWidget()))->getQboard()[location.row][location.column]->mark_Qsquare();
    return;

}

void Qgame::begin_promotion(){

    //write message
    findChild<QPlainTextEdit*>("messages")->setPlainText(QString("choose piece for promotion"));

    //create icons and open promotion buttons, and close the board
    Color color = game->getChosen_Piece().color;
    string pieces_names[] = {"queen","rook","knight","bishop"};
    for(string piece_name:pieces_names){
         string piece_and_color_name(color_to_string(color)+"_"+piece_name);
         promotion_button* button = findChild<promotion_button*>(QString::fromStdString(piece_name));
         QIcon icon(icons_dir+QString::fromStdString(piece_and_color_name));
         button->setIcon(icon);
         button->setVisible(true);
    }
    closeQboard();
}


void promotion(promotion_button *button)
{
    Qgame* qgame = (Qgame*)button->parentWidget();
    piece_type type;
    //get the type of piece that the player chose to promote his pawn.
    if(button->objectName()=="queen")
           type = QUEEN;
       else if(button->objectName()=="rook")
           type = ROOK;
       else if(button->objectName()=="bishop")
           type = BISHOP;
       else if(button->objectName()=="knight")
           type = KNIGHT;
        qgame->getGame()->promotion(type);

        //hide promotion buttons
        string pieces_names[] = {"queen","rook","knight","bishop"};
        for(string piece_name:pieces_names){
             promotion_button* button = qgame->findChild<promotion_button*>(QString::fromStdString(piece_name));
             button->setVisible(false);
        }
        //update promotion square and open the board
        Location location = qgame->getGame()->getCurrent_state().getPromotion_location();
        qgame->getQboard()[location.row][location.column]->update_Qsquare();
        qgame->openQboard();
        qgame->update_message();

}


void abort_piece_chosen(Qgame *qgame)
{   Game* game = qgame->getGame();
    //abort piece choice
    game->setPiece_chosen_check(false);
    //unmark the available locations of the previously chosen pieces
    vector<Location> locations = game->current_state_available_locations(game->getChosen_Piece());
    for(Location location:locations)
       qgame->getQboard()[location.row][location.column]->update_Qsquare();
    return;
}

void qgame_move(Qgame* qgame,const Location& location)
{
    Game* game = qgame->getGame();
    //get the type of the move (see type "Move_type")
    Move_Type type = game->getCurrent_state().move_type(game->getChosen_Piece(),location);
    //get the chosen player available locations
    vector<Location> locations = game->current_state_available_locations(game->getChosen_Piece());
    Location previous_location = game->getChosen_Piece().location;
    //make the move
    game->move(location);
    game->setPiece_chosen_check(false);
    //update the current location of the chosen piece and unmark marked squares
    for(Location loc:locations)
       qgame->getQboard()[loc.row][loc.column]->update_Qsquare();
    //update the previous location of the chosen piece
    qgame->getQboard()[previous_location.row][previous_location.column]->update_Qsquare();

    switch (type) {
    //if the move is castling, update the rook previous and current locations
    case QUEENSIDE_CASTLING:
        qgame->getQboard()[location.row][A]->update_Qsquare();
        qgame->getQboard()[location.row][D]->update_Qsquare();
        break;
    case KINGSIDE_CASTLING:
        qgame->getQboard()[location.row][F]->update_Qsquare();
        qgame->getQboard()[location.row][H]->update_Qsquare();
        break;
    //if the move is en_passant, update the location of the captured pawn
    case EN_PASSANT:
        game->getChosen_Piece().color==WHITE ?
            qgame->getQboard()[location.row-1][location.column]->update_Qsquare() :
            qgame->getQboard()[location.row+1][location.column]->update_Qsquare();
        break;
  //if the move is promotion, start promotion process
    case PROMOTION:
        qgame->begin_promotion();
        return;
    case PROMOTION_AND_CAPTURE:
        qgame->begin_promotion();
        return;
    }
    //update message board
    qgame->update_message();
}
