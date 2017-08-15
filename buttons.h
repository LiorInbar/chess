#ifndef BUTTONS_H
#define BUTTONS_H

#include <QWidget>
#include <QPushButton>
class Qgame;


class surrender_button : public QPushButton
{
    Q_OBJECT
public:
    explicit surrender_button(QWidget *parent = nullptr);

signals:

public slots:
    void surrender_clicked(bool checked);
};


class draw_button : public QPushButton
{
    Q_OBJECT
public:
    explicit draw_button(QWidget *parent = nullptr);

signals:

public slots:
    void draw_clicked(bool checked);
};

class new_game_button : public QPushButton
{
    Q_OBJECT
public:
    explicit new_game_button(QWidget *parent = nullptr);

signals:

public slots:
    void new_game_clicked(bool checked);
};

class response_button : public QPushButton
{
    Q_OBJECT
public:
    explicit response_button(QWidget *parent = nullptr);

signals:

public slots:
    void response_clicked(bool checked);
};




#endif // BUTTONS_H
