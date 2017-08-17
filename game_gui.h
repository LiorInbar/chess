#ifndef QGAME_H
#define QGAME_H

#include <QWidget>

#include "square_gui.h"
#include "buttons.h"
#include "game.h"


class Game_GUI: public QWidget
{
   Game game;
    vector<vector<Square_GUI*>> qboard;


public:
    Game_GUI();
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
    friend void abort_piece_chosen(Game_GUI* qgame);
    //move the chosen piece to location
    friend void qgame_move(Game_GUI* qgame,const Location& location);
    //piece on 'qsquare' was chosen
    friend void piece_chosen(Square_GUI* qsquare);
    //activate promotion to the piece that is marked by button
    friend void promotion(Promotion_button* button);
    //current player surrender
    friend void surrender(Game_GUI* qgame);
    //current player offer draw
    friend void draw_offer(Game_GUI* qgame);
    //activate player response to draw offer
    friend void response_draw_offer(Response_button* button);
    friend void new_game(Game_GUI* qgame);


    Game getGame() const;
    void setGame(const Game &value);
    vector<vector<Square_GUI *> > getQboard() const;
    void setQboard(const vector<vector<Square_GUI *> > &value);
};




#endif // QGAME_H
