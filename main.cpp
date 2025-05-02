#include "MainWindow.h"

#include <QApplication>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication::setStyle("Fusion");
    QApplication::setFont(QFont("Cambria Math", 10));

    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Matrix Symbolic Calculator");
    w.show();
    return a.exec();
}
