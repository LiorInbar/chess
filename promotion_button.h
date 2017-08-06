#ifndef PROMOTION_BUTTON_H
#define PROMOTION_BUTTON_H
#include <QWidget>
#include <QPushButton>
#include "game.h"
class Qsquare;
void openQboard(Qsquare);

class promotion_button : public QPushButton
{
    Game* game;
    vector<vector<Qsquare*>>& Qboard;

    Q_OBJECT
public:
    explicit promotion_button(vector<vector<Qsquare*>>& board,QWidget *parent = nullptr);

    Game *getGame() const;
    void setGame(Game *value);

    vector<vector<Qsquare *> > &getQboard() const;
    void setQboard(const vector<vector<Qsquare *> > &value);

signals:

public slots:

    void promotion_clicked(bool checked);

};

#endif // PROMOTION_BUTTON_H
