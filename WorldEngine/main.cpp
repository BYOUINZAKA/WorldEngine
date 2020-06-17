#include "mainwindow.h"

#include <QApplication>
#include <QDebug>

#include "topography/topographycontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    TopographyController topographyController;
    TopographyView topographyView{topographyController, &w};

    topographyController.buildRandomMap();
    qDebug("%f", topographyView.avgHeight());
    qDebug("%d", topographyController.getModel()->getRealSize() * sizeof(Area));
    return a.exec();
}
