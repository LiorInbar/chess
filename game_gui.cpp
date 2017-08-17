#include "game_gui.h"

extern bool shutdown_board;
extern QString icons_dir;
extern bool shut_down_buttons;

Game Game_GUI::getGame() const
{
    return game;
}

void Game_GUI::setGame(const Game &value)
{
    game = value;
}

vector<vector<Square_GUI *> > Game_GUI::getQboard() const
{
    return qboard;
}

void Game_GUI::setQboard(const vector<vector<Square_GUI *> > &value)
{
    qboard = value;
}

Game_GUI::Game_GUI(): game(Game())
{
    setFixedSize(1200, 600);
    //messages box
    QPlainTextEdit* messages(new QPlainTextEdit(this));
    messages->setObjectName("messages");
    messages->setGeometry(700, 20, 200, 70);
    messages->setPlainText(QString("white player turn"));

    //Qsquares board
   for(int i=0;i<8;i++){
       qboard.push_back(vector<Square_GUI*>());
       for(int j=0;j<8;j++){
         Square_GUI* qsquare(new Square_GUI(game,Location(i,j),this));
          qboard[i].push_back(qsquare);
          qboard[i][j]->setGeometry(20+j*70, 500-i*70, 70, 70);
          qboard[i][j]->update_Qsquare();
       }
   }
   //promotion buttons (appear when a promotion happen).
   vector<Promotion_button*> promotion_buttons;
   for(int i=0;i<4;i++){
       Promotion_button* button(new Promotion_button(game,this));
       promotion_buttons.push_back(button);
       promotion_buttons[i]->setGeometry(700+i*70, 100, 70, 70);
       promotion_buttons[i]->setVisible(false);
   }
   promotion_buttons[0]->setObjectName("queen");
   promotion_buttons[1]->setObjectName("rook");
   promotion_buttons[2]->setObjectName("knight");
   promotion_buttons[3]->setObjectName("bishop");

   //surrender button
   Surrender_button* sbutton(new Surrender_button(this));
   sbutton->setGeometry(920, 20, 70, 70);
   sbutton->setObjectName("surrender_button");
   sbutton->setText("surrender");

   //draw button
   Draw_button* dbutton(new Draw_button(this));
   dbutton->setGeometry(1000, 20, 70, 70);
   dbutton->setObjectName("draw_button");
   dbutton->setText("draw offer");

   //new_game button
   New_game_button* ngbutton(new New_game_button(this));
   ngbutton->setGeometry(1100, 20, 70, 70);
   ngbutton->setObjectName("new_game_button");
   ngbutton->setText("new game");

   //response_buttons
   Response_button* ybutton(new Response_button(this));
   ybutton->setGeometry(700, 100, 70, 70);
   ybutton->setObjectName("yes_response");
   ybutton->setText("yes");
   ybutton->setVisible(false);

   Response_button* nbutton(new Response_button(this));
   nbutton->setGeometry(800, 100, 70, 70);
   nbutton->setObjectName("no_response");
   nbutton->setText("no");
   nbutton->setVisible(false);



   //and wer'e off
}

void Game_GUI::start_new_game()
{
    shut_down_buttons = false;
    QPlainTextEdit* messages = findChild<QPlainTextEdit*>("messages");
    messages->setPlainText(QString("white player turn"));
    Game new_game;
    game = new_game;
   for(int i=0;i<8;i++){
       for(int j=0;j<8;j++){
          qboard[i][j]->update_Qsquare();
          qboard[i][j]->setGame(game);
}
   }
   string pieces_names[] = {"queen","rook","knight","bishop"};
   for(string piece_name:pieces_names){
        Promotion_button* button = findChild<Promotion_button*>(QString::fromStdString(piece_name));
        button->setVisible(false);
        button->setGame(game);
   }
   Response_button* ybutton = findChild<Response_button*>("yes_response");
   Response_button* nbutton = findChild<Response_button*>("no_response");
   ybutton->setVisible(false);
   nbutton->setVisible(false);
   openQboard();
}

void Game_GUI::update_message(){

    QPlainTextEdit* message_board = findChild<QPlainTextEdit*>("messages");
    //if the game is over, print result
    if(game.getResult() != IN_PROGRESS)
        message_board->setPlainText( QString::fromStdString(result_to_string(game.getResult())));
    //if the game is in progress, print "<current_player> player turn"
    else
      message_board->setPlainText( QString::fromStdString(color_to_string(game.Turn())+" player turn"));

}

void piece_chosen(Square_GUI *qsquare)
{
     Game& game = qsquare->getGame();
    Square square = game.getCurrent_state().getSquare(qsquare->getLocation());
    game.setChosen_Piece(square.getPiece());
    game.setPiece_chosen_check(true);
    //mark the piece's available locations
    vector<Location> locations = game.current_state_available_locations(game.getChosen_Piece());
    for(Location location:locations)
      ((Game_GUI*)(qsquare->parentWidget()))->getQboard()[location.getRow()][location.getColumn()]->mark_Qsquare();
    return;

}

void Game_GUI::begin_promotion(){

    //write message
    findChild<QPlainTextEdit*>("messages")->setPlainText(QString("choose piece for promotion"));

    //create icons and open promotion buttons, and close the board
    Color color = game.getChosen_Piece().getColor();
    string pieces_names[] = {"queen","rook","knight","bishop"};
    for(string piece_name:pieces_names){
         string piece_and_color_name(color_to_string(color)+"_"+piece_name);
        Promotion_button* button = findChild<Promotion_button*>(QString::fromStdString(piece_name));
         QIcon icon(icons_dir+QString::fromStdString(piece_and_color_name));
         button->setIcon(icon);
         button->setVisible(true);
    }
    closeQboard();
}

void Game_GUI::end_game()//todo
{
    shut_down_buttons=true;
    update_message();
}

void new_game(Game_GUI *qgame)
{
    qgame->start_new_game();
}

void response_draw_offer(Response_button* button)
{
    Game_GUI* qgame = (Game_GUI*)(button->parentWidget());
    if(button->objectName()=="yes_response"){
        qgame->game.setResult(DRAW);
        qgame->end_game();
    }
    else{
        qgame->openQboard();
        Response_button* ybutton = qgame->findChild<Response_button*>("yes_response");
        Response_button* nbutton = qgame->findChild<Response_button*>("no_response");
        ybutton->setVisible(false);
        nbutton->setVisible(false);
        qgame->update_message();
        shut_down_buttons = false;
    }
}


void promotion(Promotion_button *button)
{
    Game_GUI* qgame = (Game_GUI*)button->parentWidget();
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
        qgame->game.promotion(type);

        //hide promotion buttons
        string pieces_names[] = {"queen","rook","knight","bishop"};
        for(string piece_name:pieces_names){
             Promotion_button* button = qgame->findChild<Promotion_button*>(QString::fromStdString(piece_name));
             button->setVisible(false);
        }
        //update promotion square and open the board
        Location location = qgame->game.getCurrent_state().getPromotion_location();
        qgame->getQboard()[location.getRow()][location.getColumn()]->update_Qsquare();
        qgame->openQboard();
        qgame->update_message();

}


void abort_piece_chosen(Game_GUI *qgame)
{ 
    Game& game = qgame->game;
    //abort piece choice
    game.setPiece_chosen_check(false);
    //unmark the available locations of the previously chosen pieces
    vector<Location> locations = game.current_state_available_locations(game.getChosen_Piece());
    for(Location location:locations)
       qgame->getQboard()[location.getRow()][location.getColumn()]->update_Qsquare();
    return;
}

void qgame_move(Game_GUI *qgame, const Location& location)
{
    Game& game = qgame->game;
    //get the type of the move (see type "Move_type")
    Move_Type type = game.getCurrent_state().move_type(game.getChosen_Piece(),location);
    //get the chosen player available locations
    vector<Location> locations = game.current_state_available_locations(game.getChosen_Piece());
    Location previous_location = game.getChosen_Piece().getLocation();
    //make the move
    game.move(location);

    game.setPiece_chosen_check(false);
    //update the current location of the chosen piece and unmark marked squares
    for(Location loc:locations)
       qgame->getQboard()[loc.getRow()][loc.getColumn()]->update_Qsquare();
    //update the previous location of the chosen piece
    qgame->getQboard()[previous_location.getRow()][previous_location.getColumn()]->update_Qsquare();

    switch (type) {
    //if the move is castling, update the rook previous and current locations
    case QUEENSIDE_CASTLING:
        qgame->getQboard()[location.getRow()][A]->update_Qsquare();
        qgame->getQboard()[location.getRow()][D]->update_Qsquare();
        break;
    case KINGSIDE_CASTLING:
        qgame->getQboard()[location.getRow()][F]->update_Qsquare();
        qgame->getQboard()[location.getRow()][H]->update_Qsquare();
        break;
    //if the move is en_passant, update the location of the captured pawn
    case EN_PASSANT:
        game.getChosen_Piece().getColor()==WHITE ?
            qgame->getQboard()[location.getRow()-1][location.getColumn()]->update_Qsquare() :
            qgame->getQboard()[location.getRow()+1][location.getColumn()]->update_Qsquare();
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

void Game_GUI::closeQboard()
{
    shutdown_board = true;
}

void Game_GUI::openQboard()
{
    shutdown_board = false;

}

void draw_offer(Game_GUI *qgame)//todo
{
    qgame->closeQboard();
    QPlainTextEdit* message_board = qgame->findChild<QPlainTextEdit*>("messages");
    Color offer = qgame->game.Turn();
    Color offered = offer == WHITE ? BLACK : WHITE;
    QString message(QString::fromStdString(color_to_string(offer)+" player offer draw, "
                                             +color_to_string(offered)+" accept?"));
    message_board->setPlainText(message);
    Response_button* ybutton = qgame->findChild<Response_button*>("yes_response");
    Response_button* nbutton = qgame->findChild<Response_button*>("no_response");
    ybutton->setVisible(true);
    nbutton->setVisible(true);
    qgame->closeQboard();
    shut_down_buttons=true;

}

void surrender(Game_GUI *qgame)//todo
{
    qgame->closeQboard();
    QPlainTextEdit* message_board = qgame->findChild<QPlainTextEdit*>("messages");
    Color winner = qgame->game.Turn() == WHITE ? BLACK : WHITE;
    message_board->setPlainText( QString::fromStdString(color_to_string(winner)+" player win!"));
    qgame->game.setResult(winner==WHITE ? WHITE_WIN : BLACK_WIN);
    qgame->end_game();
}
