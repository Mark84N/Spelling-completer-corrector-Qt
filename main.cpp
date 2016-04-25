#include <QApplication>
#include "mymainwindow.h"

//! [0]
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MyMainWindow widget;
    widget.show();

    return a.exec();
}
//! [0]
