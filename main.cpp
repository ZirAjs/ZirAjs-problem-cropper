#include "mainwindow.h"
#include "./eventfilter.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EventFilter * filter = new EventFilter(&w);
    a.installEventFilter(filter)
    MainWindow w;
    w.show();
    return a.exec();
}
