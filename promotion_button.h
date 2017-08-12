#ifndef PROMOTION_BUTTON_H
#define PROMOTION_BUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QTextStream>
#include "game.h"

class Qsquare;
class Qgame;

class promotion_button : public QPushButton
{
    Game* game;
    Q_OBJECT
public:

    explicit promotion_button(Game* game, QWidget *parent = nullptr);

signals:

public slots:

    /*Handler of clicking events on the button*/
    void promotion_clicked(bool checked);

};

#endif // PROMOTION_BUTTON_H
