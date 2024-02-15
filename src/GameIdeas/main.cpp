
#include "include/gameideas.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameIdeas gameIdeasWindow;
    gameIdeasWindow.show();
    return a.exec();
}
