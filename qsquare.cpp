#include "qsquare.h"
#include <QIcon>
#include <QString>
#include <QTextStream>

QString icons_dir("C:/QtProjects/QtChess/icons/");

bool shutdown_board = false;


Qsquare::Qsquare( vector<vector<Qsquare*>>& board,QWidget *parent) : QPushButton(parent),
  Qboard(board)
{
    connect(this,SIGNAL(clicked(bool)),this,SLOT(Qsquare_clicked(bool)));
}

void Qsquare::enterEvent( QEvent* e ){
    if(!(game->isPiece_chosen())&&(!shutdown_board)){
        Square square = game->getCurrent_state().getSquare(location);
        if(!(square.is_empty())){
            if(square.piece.color==game->Turn()){
               mark_Qsquare();
            }
        }
    }

}

void Qsquare::leaveEvent(QEvent *e)
{
    if(!(game->isPiece_chosen())&&(!shutdown_board)){
        Square square = game->getCurrent_state().getSquare(location);
        if(!(square.is_empty())){
            if(square.piece.color==game->Turn()){
               update_Qsquare();
            }
        }
    }
}


void Qsquare::promotion()
{
    Color color = game->getChosen_Piece().color;
    parentWidget()->findChild<QPlainTextEdit*>("messages")->setPlainText(QString("choose piece for promotion"));
    string piece_name(color_to_string(color)+"_"+"queen");
    parentWidget()->findChild<promotion_button*>("queen")->setIcon(QIcon(icons_dir+QString::fromStdString(piece_name)));
    parentWidget()->findChild<promotion_button*>("queen")->setVisible(true);
    piece_name = (color_to_string(color)+"_"+"rook");
    parentWidget()->findChild<promotion_button*>("rook")->setIcon(QIcon(icons_dir+QString::fromStdString(piece_name)));
    parentWidget()->findChild<promotion_button*>("rook")->setVisible(true);
    piece_name = (color_to_string(color)+"_"+"knight");
    parentWidget()->findChild<promotion_button*>("knight")->setIcon(QIcon(icons_dir+QString::fromStdString(piece_name)));
    parentWidget()->findChild<promotion_button*>("knight")->setVisible(true);
    piece_name = (color_to_string(color)+"_"+"bishop");
    parentWidget()->findChild<promotion_button*>("bishop")->setIcon(QIcon(icons_dir+QString::fromStdString(piece_name)));
    parentWidget()->findChild<promotion_button*>("bishop")->setVisible(true);
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

void Qsquare::Qsquare_clicked(bool checked)
{
    if(shutdown_board)
        return;
    if(!(game->isPiece_chosen())){
        Square square = game->getCurrent_state().getSquare(location);
        if(!(square.is_empty())){
            if(square.piece.color==game->Turn()){
               changeColorWhenEnter=false;
               game->setChosen_Piece(game->getCurrent_state().getSquare(location).piece);
               game->setPiece_chosen_check(true);
               vector<Location> locations = game->current_state_available_locations(game->getChosen_Piece());
               for(Location location:locations)
                  getQboard()[location.row][location.column]->mark_Qsquare();

            }
        }
    }
    else{

        vector<Location> locations = game->current_state_available_locations(game->getChosen_Piece());
        if(game->getChosen_Piece().location==location){
            game->setPiece_chosen_check(false);
            for(Location location:locations){
               getQboard()[location.row][location.column]->update_Qsquare();
            }
        }
        else if(std::find(locations.begin(), locations.end(), location) != locations.end()) {
              Move_Type type = game->getCurrent_state().move_type(game->getChosen_Piece(),location);
              game->move(game->getChosen_Piece(),location);
              game->setPiece_chosen_check(false);

              for(Location location:locations){
                 getQboard()[location.row][location.column]->update_Qsquare();
              }

              Qboard[game->getChosen_Piece().location.row][game->getChosen_Piece().location.column]->update_Qsquare();

              if(type==QUEENSIDE_CASTLING){
                  Qboard[location.row][A]->update_Qsquare();
                  Qboard[location.row][D]->update_Qsquare();
              }
              else if(type==KINGSIDE_CASTLING){
                  Qboard[location.row][F]->update_Qsquare();
                  Qboard[location.row][H]->update_Qsquare();
              }
              else if(type==EN_PASSANT){
                  if(game->getChosen_Piece().color==WHITE){
                      Qboard[location.row-1][location.column]->update_Qsquare();
                  }
                  else{
                      Qboard[location.row+1][location.column]->update_Qsquare();
                  }
              }
              else if(type==PROMOTION||type==PROMOTION_AND_CAPTURE){
                    promotion();
              }
              if(!(type==PROMOTION||type==PROMOTION_AND_CAPTURE)){
                update_message();
            }
        }

   }

}



vector<vector<Qsquare *> > &Qsquare::getQboard() const
{
    return Qboard;
}

void Qsquare::setQboard(const vector<vector<Qsquare *> > &value)
{
    Qboard = value;
}


void Qsquare::mark_Qsquare(){

setStyleSheet("background-color:palegreen");

}


void openQboard(Qsquare *qsquare)
{
     shutdown_board = false;
}

void end_promotion(Qsquare *square)
{
    square->update_Qsquare();
    openQboard( square);
    square->parentWidget()->findChild<promotion_button*>("queen")->setVisible(false);
    square->parentWidget()->findChild<promotion_button*>("rook")->setVisible(false);
    square->parentWidget()->findChild<promotion_button*>("knight")->setVisible(false);
    square->parentWidget()->findChild<promotion_button*>("bishop")->setVisible(false);
    square->update_message();
}

void Qsquare::update_message()
{
    if(game->getResult() != IN_PROGRESS){
        parentWidget()->findChild<QPlainTextEdit*>("messages")->setPlainText( QString::fromStdString(result_to_string(game->getResult())));
    }
    else
      parentWidget()->findChild<QPlainTextEdit*>("messages")->setPlainText( QString::fromStdString
                                                                             (color_to_string(game->Turn())+" player turn"));
  }


