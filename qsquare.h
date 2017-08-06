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
    bool changeColorWhenEnter = true;
    void update_pink_Qsquare();
    void update_blue_Qsquare();
    virtual void enterEvent( QEvent* e );
    virtual void leaveEvent( QEvent* e );
    void promotion();

    Q_OBJECT
public:

    void setLocation(const Location location);
    void setGame(Game* game);
    explicit Qsquare(vector<vector<Qsquare*>>& board,QWidget *parent = nullptr);
    void mark_Qsquare();
    void update_Qsquare();
    vector<vector<Qsquare *> > &getQboard() const;
    void setQboard(const vector<vector<Qsquare *> > &value);
    void update_message();


signals:

public slots:
    void Qsquare_clicked(bool checked);
};
void openQboard(Qsquare square);
void end_promotion(Qsquare square);
#endif // QSQUARES_H
