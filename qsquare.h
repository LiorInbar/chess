#ifndef QSQUARES_H
#define QSQUARES_H

#include <QWidget>
#include <QPushButton>
#include <QStatusBar>
#include <QPlainTextEdit>

#include "game.h"
#include "promotion_button.h"



class Qsquare : public QPushButton
{
    Game* game;
    vector<vector<Qsquare*>>& Qboard;
    Location location;
    virtual void enterEvent( QEvent* e );
    virtual void leaveEvent( QEvent* e );
    void promotion();
    void mark_Qsquare();
    bool possible_square_choice_check();

    Q_OBJECT
public:

    explicit Qsquare(vector<vector<Qsquare*>>& board,QWidget *parent = nullptr);

    void setLocation(const Location location);
    void setGame(Game* game);
    vector<vector<Qsquare *> > &getQboard() const;
    void setQboard(const vector<vector<Qsquare *> > &value);
    void update_message();
    void update_Qsquare();

signals:

public slots:
    void Qsquare_clicked(bool checked);
};


void openQboard(Qsquare square);
void end_promotion(Qsquare square);
#endif // QSQUARES_H
