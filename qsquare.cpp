#include "qsquare.h"
#include <QIcon>
#include <QString>
#include <QTextStream>

QString icons_dir("C:/QtProjects/QtChess/icons/");

bool shutdown_board = false;


Qsquare::Qsquare( vector<vector<Qsquare*>>& board,QWidget *parent) :
    QPushButton(parent), Qboard(board){

    connect(this,SIGNAL(clicked(bool)),this,SLOT(Qsquare_clicked(bool)));
}

bool Qsquare::possible_square_choice_check() const{

    Square square = game->getCurrent_state().getSquare(location);
    return (!(game->isPiece_chosen())&&(!shutdown_board)
    &&(!(square.is_empty()))&&(square.piece.color==game->Turn()));
}


void Qsquare::enterEvent( QEvent* e ){
    if(possible_square_choice_check())
         mark_Qsquare();
}

void Qsquare::leaveEvent(QEvent *e){
    if(possible_square_choice_check())
       update_Qsquare();
}


void Qsquare::promotion()
{
    parentWidget()->findChild<QPlainTextEdit*>("messages")->setPlainText(QString("choose piece for promotion"));
    Color color = game->getChosen_Piece().color;
    string pieces_names[] = {"queen","rook","knight","bishop"};

    for(string piece_name:pieces_names){
         string piece_and_color_name(color_to_string(color)+"_"+piece_name);
         promotion_button* button = parentWidget()->findChild<promotion_button*>(QString::fromStdString(piece_name));
         QIcon icon(icons_dir+QString::fromStdString(piece_and_color_name));
         button->setIcon(icon);
         button->setVisible(true);
    }
    shutdown_board=true;
}


 void Qsquare::setLocation(const Location location){
     Qsquare::location=location;
 }

 void Qsquare::setGame(Game* game){
     Qsquare::game=game;
 }

void Qsquare::update_Qsquare(){
    if((location.row+location.column)%2==0)
         setStyleSheet("background-color:mediumblue;");
    else
         setStyleSheet("background-color:sienna;");
    Square square = game->getCurrent_state().getSquare(location);
    if(!(square.is_empty())){
        string piece_name(color_to_string(square.piece.color)+"_"+piece_type_to_string(square.piece.type));
        setIcon(QIcon(icons_dir+QString::fromStdString(piece_name)+".png"));
        setIconSize(QSize(40,40));
    }
    else{
      setIcon(QIcon());
    }
}

void Qsquare::Qsquare_clicked(bool checked){

     Square square = game->getCurrent_state().getSquare(location);
    if(shutdown_board)
        return;
    if(possible_square_choice_check()){
           game->setChosen_Piece(square.piece);
           game->setPiece_chosen_check(true);
           vector<Location> locations = game->current_state_available_locations(game->getChosen_Piece());
           for(Location location:locations)
              getQboard()[location.row][location.column]->mark_Qsquare();
           return;
         }
    vector<Location> locations = game->current_state_available_locations(game->getChosen_Piece());
    Location chosen_piece_location = game->getChosen_Piece().location;
   if (chosen_piece_location==location){
        game->setPiece_chosen_check(false);
        for(Location location:locations)
           getQboard()[location.row][location.column]->update_Qsquare();
        return;
    }
    if(std::find(locations.begin(), locations.end(), location) != locations.end()) {
      Move_Type type = game->getCurrent_state().move_type(game->getChosen_Piece(),location);
      game->move(location);
      game->setPiece_chosen_check(false);
      for(Location location:locations){
         getQboard()[location.row][location.column]->update_Qsquare();
      }

      Qboard[chosen_piece_location.row][chosen_piece_location.column]->update_Qsquare();
      switch (type) {
      case QUEENSIDE_CASTLING:
          Qboard[location.row][A]->update_Qsquare();
          Qboard[location.row][D]->update_Qsquare();
          break;
      case KINGSIDE_CASTLING:
          Qboard[location.row][F]->update_Qsquare();
          Qboard[location.row][H]->update_Qsquare();
          break;
      case EN_PASSANT:
          game->getChosen_Piece().color==WHITE ?
              Qboard[location.row-1][location.column]->update_Qsquare() :
              Qboard[location.row+1][location.column]->update_Qsquare();
          break;
      case PROMOTION:
          promotion();
          return;
      case PROMOTION_AND_CAPTURE:
          promotion();
          return;
      }
      update_message();
   }
}

void Qsquare::mark_Qsquare(){

    setStyleSheet("background-color:palegreen");

}


void openQboard(Qsquare *qsquare){
     shutdown_board = false;
}

void end_promotion(Qsquare *square){

    square->update_Qsquare();
    openQboard( square);
    square->parentWidget()->findChild<promotion_button*>("queen")->setVisible(false);
    square->parentWidget()->findChild<promotion_button*>("rook")->setVisible(false);
    square->parentWidget()->findChild<promotion_button*>("knight")->setVisible(false);
    square->parentWidget()->findChild<promotion_button*>("bishop")->setVisible(false);
    square->update_message();
}

void Qsquare::update_message(){

    QPlainTextEdit* message_board = parentWidget()->findChild<QPlainTextEdit*>("messages");
    if(game->getResult() != IN_PROGRESS)
        message_board->setPlainText( QString::fromStdString(result_to_string(game->getResult())));
    else
      message_board->setPlainText( QString::fromStdString(color_to_string(game->Turn())+" player turn"));

}

vector<vector<Qsquare *> > &Qsquare::getQboard() const{
    return Qboard;
}

void Qsquare::setQboard(const vector<vector<Qsquare *> > &value){
    Qboard = value;
}


