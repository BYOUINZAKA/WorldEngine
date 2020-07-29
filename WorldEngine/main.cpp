#include <QApplication>
#include <QDebug>
#include <QThread>
#include <QTimer>

#include "mainwindow.h"
#include "topography/topographycontroller.h"
#include "topography/topographyview.h"
#include "unit/optional.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
