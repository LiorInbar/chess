#include "buttons.h"

bool shut_down_buttons = false;
void surrender(Qgame* Qgame);
void draw_offer(Qgame* Qgame);
void response_draw_offer(response_button* button);
void new_game(Qgame* qgame);


surrender_button::surrender_button(QWidget *parent):QPushButton(parent)
{
    connect(this,SIGNAL(clicked(bool)),this,SLOT(surrender_clicked(bool)));

}

void surrender_button::surrender_clicked(bool checked)
{
    if(!shut_down_buttons)
        surrender((Qgame *)parentWidget());
}

draw_button::draw_button(QWidget *parent):QPushButton(parent)
{
    connect(this,SIGNAL(clicked(bool)),this,SLOT(draw_clicked(bool)));
}

void draw_button::draw_clicked(bool checked){
    if(!shut_down_buttons)
        draw_offer((Qgame *)parentWidget());
}

response_button::response_button(QWidget *parent):QPushButton(parent)
{
    connect(this,SIGNAL(clicked(bool)),this,SLOT(response_clicked(bool)));

}

void response_button::response_clicked(bool checked){
    response_draw_offer(this);
}

new_game_button::new_game_button(QWidget *parent):QPushButton(parent)
{
    connect(this,SIGNAL(clicked(bool)),this,SLOT(new_game_clicked(bool)));

}

void new_game_button::new_game_clicked(bool checked)
{
    new_game((Qgame *)parentWidget());
}
