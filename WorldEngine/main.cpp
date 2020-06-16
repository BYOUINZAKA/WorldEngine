#include "mainwindow.h"

#include <random>
#include <chrono>

#include <QApplication>
#include <QDebug>
#include <QVarLengthArray>

#include "topography/topographycontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Topography topography;
    TopographyController topographyController(&topography);

    topographyController.buildRandom();

    for(auto &&i : topography)
    {
        qDebug("%lf", i.altitude);
    }

    return a.exec();
}
