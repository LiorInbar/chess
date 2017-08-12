#include "promotion_button.h"

void promotion(promotion_button* button);

promotion_button::promotion_button(Game *game, QWidget *parent) :
    QPushButton(parent), game(game){

    connect(this,SIGNAL(clicked(bool)),this,SLOT(promotion_clicked(bool)));

}

/*call game::promotion() with the piece type of the clicked promotion button. */
void promotion_button::promotion_clicked(bool checked){
    promotion(this);
}

