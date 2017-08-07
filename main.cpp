#include <QApplication>
#include <QProgressBar>
#include "window.h"
#include <QPushButton>
#include "game.h"
#include "qsquare.h"
#include "promotion_button.h"
#include <QDir>
#include <QMainWindow>
#include <QTextStream>

/*QTextStream out(stdout);
    out << "x" << endl;*/


int main(int argc, char **argv){

     QApplication app (argc, argv);
     app.setApplicationDisplayName("chess");

     QWidget window;
     window.setFixedSize(1200, 600);

     QPlainTextEdit* messages = new QPlainTextEdit(&window);
     messages->setObjectName("messages");
     messages->setGeometry(700, 20, 200, 70);
     messages->setPlainText(QString("white player turn"));

     Game game;

    vector<vector<Qsquare*>> qsquares;
    for(int i=0;i<8;i++){
        qsquares.push_back(vector<Qsquare*>());
        for(int j=0;j<8;j++){
           qsquares[i].push_back(new Qsquare(qsquares,&window));
           qsquares[i][j]->setGeometry(20+j*70, 500-i*70, 70, 70);
           qsquares[i][j]->setLocation(Location(i,j));
           qsquares[i][j]->setGame(&game);
           qsquares[i][j]->setQboard(qsquares);
           qsquares[i][j]->update_Qsquare();
        }
    }

    vector<promotion_button*> promotion_buttons;
    for(int i=0;i<4;i++){
        promotion_buttons.push_back(new promotion_button(qsquares,&window));
        promotion_buttons[i]->setGeometry(700+i*70, 100, 70, 70);
        promotion_buttons[i]->setVisible(false);
        promotion_buttons[i]->setGame(&game);
    }
    promotion_buttons[0]->setObjectName("queen");
    promotion_buttons[1]->setObjectName("rook");
    promotion_buttons[2]->setObjectName("knight");
    promotion_buttons[3]->setObjectName("bishop");

    window.show();
    return app.exec();
}
