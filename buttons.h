#ifndef BUTTONS_H
#define BUTTONS_H

#include <QWidget>
#include <QPushButton>
#include "game.h"

class Game_GUI;
class Square_GUI;


class Surrender_button : public QPushButton
{
    Q_OBJECT
public:
    explicit Surrender_button(QWidget *parent = nullptr);

signals:

public slots:
    void surrender_clicked(bool checked);
};


class Draw_button : public QPushButton
{
    Q_OBJECT
public:
    explicit Draw_button(QWidget *parent = nullptr);

signals:

public slots:
    void draw_clicked(bool checked);
};

class New_game_button : public QPushButton
{
    Q_OBJECT
public:
    explicit New_game_button(QWidget *parent = nullptr);

signals:

public slots:
    void new_game_clicked(bool checked);
};

class Response_button : public QPushButton
{
    Q_OBJECT
public:
    explicit Response_button(QWidget *parent = nullptr);

signals:

public slots:
    void response_clicked(bool checked);
};

class Promotion_button : public QPushButton
{
    Game& game;
    Q_OBJECT
public:

    explicit Promotion_button(Game& game, QWidget *parent = nullptr);



    Game &getGame() const;
    void setGame(const Game &value);

signals:

public slots:

    /*Handler of clicking events on the button*/
    void promotion_clicked(bool checked);

};


#endif // BUTTONS_H
