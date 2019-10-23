#include "widget.h"

#include <QApplication>

int main(int argc, char* argv[]) {
    qsrand(static_cast<uint>(time(nullptr)));
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
