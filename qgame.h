#ifndef QGAME_H
#define QGAME_H

#include <QWidget>

#include "qsquare.h"
#include "promotion_button.h"
#include "game.h"


class Qgame: public QWidget
{
    Game* game;
    vector<vector<Qsquare*>> qboard;

public:
    Qgame(){}
    /*update the message in the messages window.*/
    void update_message();

    /* deactivate the Qsquares event handlers (used
     * after promotions) */
    void closeQboard();
    /*activate the Qsquares*/
    void openQboard();
    /*remove the promotion buttons and open the board*/
    void begin_promotion();

    vector<vector<Qsquare *> > getQboard() const;
    void setQboard(const vector<vector<Qsquare *> > &value);
    Game* getGame() const;
    void setGame(Game *value);
};

//piece on 'qsquare' was chosen
void piece_chosen(Qsquare* qsquare);
//abort piece choice
void abort_piece_chosen(Qgame *qgame);
//move the chosen piece to location
void qgame_move(Qgame* qgame,const Location& location);
//activate promotion to the piece that is marked by button
void promotion(promotion_button *button);

#endif // QGAME_H
