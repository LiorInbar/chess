#include "promotion_button.h"

class Qsquare;
void openQboard(Qsquare* qsquare);
void end_promotion(Qsquare* square);


Game *promotion_button::getGame() const
{
    return game;
}

void promotion_button::setGame(Game *value)
{
    game = value;
}

vector<vector<Qsquare *> > &promotion_button::getQboard() const
{
    return Qboard;
}

void promotion_button::setQboard(const vector<vector<Qsquare *> > &value)
{
    Qboard = value;
}

promotion_button::promotion_button(vector<vector<Qsquare*>>& board,QWidget *parent) : QPushButton(parent),Qboard(board)
{
    connect(this,SIGNAL(clicked(bool)),this,SLOT(promotion_clicked(bool)));

}

void promotion_button::promotion_clicked(bool checked)
{
    piece_type type;
    vector<Move> moves = game->Turn() == WHITE ? game->getBlack_moves() : game->getWhite_moves();
    Location location = moves[moves.size()-1].getTo();
   if(objectName()=="queen")
       type = QUEEN;
   else if(objectName()=="rook")
       type = ROOK;
   else if(objectName()=="bishop")
       type = BISHOP;
   else if(objectName()=="knight")
       type = KNIGHT;
    Color color = game->Turn() == WHITE ? BLACK : WHITE;
    game->add_piece(type,color,location);
    end_promotion(Qboard[location.row][location.column]);

}
