#ifndef QGAME_H
#define QGAME_H

#include <QWidget>

#include "qsquare.h"
#include "promotion_button.h"
#include "buttons.h"
#include "game.h"


class Qgame: public QWidget
{
   Game game;
    vector<vector<Qsquare*>> qboard;


public:
    Qgame();
    /*start new game*/
    void start_new_game();
    /*update the message in the messages window.*/
    void update_message();

    /* deactivate the Qsquares event handlers (used
     * after promotions) */
    void closeQboard();
    /*activate the Qsquares*/
    void openQboard();
    /*remove the promotion buttons and open the board*/
    void begin_promotion();
    /*game end operations*/
     void end_game();

    //abort piece choice
    friend void abort_piece_chosen(Qgame* qgame);
    //move the chosen piece to location
    friend void qgame_move(Qgame* qgame,const Location& location);
    //piece on 'qsquare' was chosen
    friend void piece_chosen(Qsquare* qsquare);
    //activate promotion to the piece that is marked by button
    friend void promotion(promotion_button* button);
    //current player surrender
    friend void surrender(Qgame* qgame);
    //current player offer draw
    friend void draw_offer(Qgame* qgame);
    //activate player response to draw offer
    friend void response_draw_offer(response_button* button);
    friend void new_game(Qgame* qgame);


    Game getGame() const;
    void setGame(const Game &value);
    vector<vector<Qsquare *> > getQboard() const;
    void setQboard(const vector<vector<Qsquare *> > &value);
};




#endif // QGAME_H
