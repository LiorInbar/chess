#ifndef QSQUARES_H
#define QSQUARES_H

#include <QWidget>
#include <QPushButton>
#include <QStatusBar>
#include <QPlainTextEdit>

#include "game.h"
#include "promotion_button.h"

//The Qsquare class represent a square in the board as a Qt widget

class Qsquare : public QPushButton
{
    Location location; //the location (board coordinates) of the Qsquare.
    Game* game;

    /*handlers of enter and leave events*/
    virtual void enterEvent( QEvent* e );
    virtual void leaveEvent( QEvent* e );


    Q_OBJECT
public:

    /*perform marking of chosen Qsquares and possible
     * Qsquare to move into for the piece in the chosen Qsquare*/
    void mark_Qsquare();

    /*check if the Qsquare has a piece of the current player,
     * and therefore the player can choose it*/
    bool possible_square_choice_check() const;

    explicit Qsquare(Game* game,const Location& location, QWidget *parent = nullptr);

    Location getLocation() const;
    void setLocation(const Location &value);

    /*update the current state of the Qsquare*/
    void update_Qsquare();


    Game *getGame() const;
    void setGame(Game *value);

signals:

public slots:

    /*handler of clicking events on the Qsquare*/
    void Qsquare_clicked(bool checked);
};



#endif // QSQUARES_H
