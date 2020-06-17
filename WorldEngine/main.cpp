#include "mainwindow.h"

#include <QApplication>
#include <QDebug>

#include "topography/topographycontroller.h"
#include "topography/topographyview.h"
#include "test.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(800, 800);
    w.show();

    Topography tp;

    TopographyView tpv{&tp, &w};
    tpv.show();

    TopographyController tpc{&tp};
    tpc.buildRandomMap();

    tpc.intelligentBuild(1000, 1, 2, 3);

    return a.exec();
}
