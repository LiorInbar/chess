#include "square_gui.h"
#include <QIcon>
#include <QString>
#include <Qdir>

#include <QTextStream>

QString icons_dir(QDir::currentPath()+"/icons/");
bool shutdown_board = false;
void piece_chosen(Square_GUI* qsquare);
void abort_piece_chosen(Game_GUI* qgame);//todo
void qgame_move(Game_GUI* qgame,const Location& location);

Square_GUI::Square_GUI(Game &game, const Location& location, QWidget *parent) :
    QPushButton(parent),location(location),game(game){

    //create signal-slot
    connect(this,SIGNAL(clicked(bool)),this,SLOT(Qsquare_clicked(bool)));
}

bool Square_GUI::possible_square_choice_check() const{

    Square square = game.getCurrent_state().getSquare(location);
    return (!(game.isPiece_chosen())&& //no piece chosen
            (!shutdown_board)&& //board open
            (!(square.is_empty()))&& //chsen square has a piece of the current player
            (square.getPiece().getColor()==game.Turn()));
}


Location Square_GUI::getLocation() const
{
    return location;
}

void Square_GUI::setLocation(const Location &value)
{
    location = value;
}

Game &Square_GUI::getGame() const
{
    return game;
}

void Square_GUI::setGame(const Game &value)
{
    game = value;
}

void Square_GUI::enterEvent( QEvent* e ){
    if(possible_square_choice_check())
        mark_Qsquare();
}

void Square_GUI::leaveEvent(QEvent *e){
    if(possible_square_choice_check())
       update_Qsquare();
}


void Square_GUI::update_Qsquare(){

    //set the color of the qsquare
    if((location.getRow()+location.getColumn())%2==0)
         setStyleSheet("background-color:mediumblue;");
    else
         setStyleSheet("background-color:sienna;");
    //set the icon of the current piece in the square
    Square square = game.getCurrent_state().getSquare(location);
    if(!(square.is_empty())){
        string piece_name(color_to_string(square.getPiece().getColor())+"_"+piece_type_to_string(square.getPiece().getType()));
        setIcon(QIcon(icons_dir+QString::fromStdString(piece_name)+".png")); //icons path
        setIconSize(QSize(40,40));
    }
    else //square is empty
      setIcon(QIcon());
}

void Square_GUI::Qsquare_clicked(bool checked){
    if(shutdown_board) //board close
        return;
    if(possible_square_choice_check()){ //piece chosen
        piece_chosen(this);
        return;
    }
     //location of the chosen piece
    Location chosen_piece_location = game.getChosen_Piece().getLocation();
    //player press on the chosen piece location - abort piece choice
   if (chosen_piece_location==location){
        abort_piece_chosen((Game_GUI*)parentWidget());
        return;
    }
   //availale locations of the chosen piece
   vector<Location> locations = game.current_state_available_locations(game.getChosen_Piece());
   //if player press on one of the piece available locations - make move
    if(std::find(locations.begin(), locations.end(), location) != locations.end())
          qgame_move((Game_GUI*)parentWidget(),location);
}

void Square_GUI::mark_Qsquare(){

    setStyleSheet("background-color:palegreen");

}



