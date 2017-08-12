#include <QApplication>
#include <QProgressBar>
#include <QPushButton>
#include "game.h"
#include "qgame.h"
#include "qsquare.h"
#include "promotion_button.h"
#include <QDir>
#include <QMainWindow>
#include <QTextStream>

/*QTextStream out(stdout);
    out << "x" << endl;*/


int main(int argc, char **argv){

    /*set aplication*/
     QApplication app (argc, argv);
     app.setApplicationDisplayName("chess");

    //main widget
     Qgame window;
     window.setFixedSize(1200, 600);

     //messages box
     QPlainTextEdit* messages = new QPlainTextEdit(&window);
     messages->setObjectName("messages");
     messages->setGeometry(700, 20, 200, 70);
     messages->setPlainText(QString("white player turn"));

     Game game; //umm, game
    window.setGame(&game);
     //Qsquares board
    vector<vector<Qsquare*>> qsquares;
    for(int i=0;i<8;i++){
        qsquares.push_back(vector<Qsquare*>());
        for(int j=0;j<8;j++){
           qsquares[i].push_back(new Qsquare(&game,Location(i,j),&window));
           qsquares[i][j]->setGeometry(20+j*70, 500-i*70, 70, 70);
           qsquares[i][j]->update_Qsquare();
        }
    }
    window.setQboard(qsquares);

    //promotion buttons (appear when a promotion happen).
    vector<promotion_button*> promotion_buttons;
    for(int i=0;i<4;i++){
        promotion_buttons.push_back(new promotion_button(&game,&window));
        promotion_buttons[i]->setGeometry(700+i*70, 100, 70, 70);
        promotion_buttons[i]->setVisible(false);
    }
    promotion_buttons[0]->setObjectName("queen");
    promotion_buttons[1]->setObjectName("rook");
    promotion_buttons[2]->setObjectName("knight");
    promotion_buttons[3]->setObjectName("bishop");

    //and wer'e off
    window.show();
    return app.exec();
}
