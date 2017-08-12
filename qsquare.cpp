#include "qsquare.h"
#include <QIcon>
#include <QString>
#include <QTextStream>

QString icons_dir("C:/QtProjects/QtChess/icons/");
bool shutdown_board = false;
void piece_chosen(Qsquare* qsquare);
void abort_piece_chosen(Qgame* qgame);//todo
void qgame_move(Qgame* qgame,const Location& location);

Qsquare::Qsquare(Game *game, const Location& location, QWidget *parent) :
    QPushButton(parent),location(location),game(game){

    //create signal-slot
    connect(this,SIGNAL(clicked(bool)),this,SLOT(Qsquare_clicked(bool)));
}

bool Qsquare::possible_square_choice_check() const{

    Square square = game->getCurrent_state().getSquare(location);
    return (!(game->isPiece_chosen())&& //no piece chosen
            (!shutdown_board)&& //board open
            (!(square.is_empty()))&& //chsen square has a piece of the current player
            (square.piece.color==game->Turn()));
}


Location Qsquare::getLocation() const
{
    return location;
}

void Qsquare::setLocation(const Location &value)
{
    location = value;
}

Game *Qsquare::getGame() const
{
    return game;
}

void Qsquare::setGame(Game *value)
{
    game = value;
}

void Qsquare::enterEvent( QEvent* e ){
    if(possible_square_choice_check())
        mark_Qsquare();
}

void Qsquare::leaveEvent(QEvent *e){
    if(possible_square_choice_check())
       update_Qsquare();
}


void Qsquare::update_Qsquare(){

    //set the color of the qsquare
    if((location.row+location.column)%2==0)
         setStyleSheet("background-color:mediumblue;");
    else
         setStyleSheet("background-color:sienna;");
    //set the icon of the current piece in the square
    Square square = game->getCurrent_state().getSquare(location);
    if(!(square.is_empty())){
        string piece_name(color_to_string(square.piece.color)+"_"+piece_type_to_string(square.piece.type));
        setIcon(QIcon(icons_dir+QString::fromStdString(piece_name)+".png")); //icons path
        setIconSize(QSize(40,40));
    }
    else //square is empty
      setIcon(QIcon());
}

void Qsquare::Qsquare_clicked(bool checked){
    if(shutdown_board) //board close
        return;
    if(possible_square_choice_check()){ //piece chosen
        piece_chosen(this);
        return;
    }
     //location of the chosen piece
    Location chosen_piece_location = game->getChosen_Piece().location;
    //player press on the chosen piece location - abort piece choice
   if (chosen_piece_location==location){
        abort_piece_chosen((Qgame*)parentWidget());
        return;
    }
   //availale locations of the chosen piece
   vector<Location> locations = game->current_state_available_locations(game->getChosen_Piece());
   //if player press on one of the piece available locations - make move
    if(std::find(locations.begin(), locations.end(), location) != locations.end())
          qgame_move((Qgame*)parentWidget(),location);
}

void Qsquare::mark_Qsquare(){

    setStyleSheet("background-color:palegreen");

}



