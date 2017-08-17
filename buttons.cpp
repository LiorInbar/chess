#include "buttons.h"

bool shut_down_buttons = false;
void surrender(Game_GUI* Game_GUI);
void draw_offer(Game_GUI* Game_GUI);
void response_draw_offer(Response_button* button);
void new_game(Game_GUI* qgame);
void promotion(Promotion_button* button);

Surrender_button::Surrender_button(QWidget *parent):QPushButton(parent)
{
    connect(this,SIGNAL(clicked(bool)),this,SLOT(surrender_clicked(bool)));

}

void Surrender_button::surrender_clicked(bool checked)
{
    if(!shut_down_buttons)
        surrender((Game_GUI *)parentWidget());
}

Draw_button::Draw_button(QWidget *parent):QPushButton(parent)
{
    connect(this,SIGNAL(clicked(bool)),this,SLOT(draw_clicked(bool)));
}

void Draw_button::draw_clicked(bool checked){
    if(!shut_down_buttons)
        draw_offer((Game_GUI *)parentWidget());
}

Response_button::Response_button(QWidget *parent):QPushButton(parent)
{
    connect(this,SIGNAL(clicked(bool)),this,SLOT(response_clicked(bool)));

}

void Response_button::response_clicked(bool checked){
    response_draw_offer(this);
}

New_game_button::New_game_button(QWidget *parent):QPushButton(parent)
{
    connect(this,SIGNAL(clicked(bool)),this,SLOT(new_game_clicked(bool)));

}

void New_game_button::new_game_clicked(bool checked)
{
    new_game((Game_GUI *)parentWidget());
}



Game &Promotion_button::getGame() const
{
    return game;
}

void Promotion_button::setGame(const Game &value)
{
    game = value;
}

Promotion_button::Promotion_button(Game &game, QWidget *parent) :
    QPushButton(parent), game(game){

    connect(this,SIGNAL(clicked(bool)),this,SLOT(promotion_clicked(bool)));

}

/*call game::promotion() with the piece type of the clicked promotion button. */
void Promotion_button::promotion_clicked(bool checked){
    promotion(this);
}
