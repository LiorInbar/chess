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

    window.show();

    return app.exec();
}
