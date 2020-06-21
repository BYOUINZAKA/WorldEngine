#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QThread>
#include <QTimer>

#include "topography/topographycontroller.h"
#include "topography/topographyview.h"
#include "unit/optional.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QThread thread{&w};

    Topography model;
    TopographyView view{&model, 1};

    TopographyController controller{&model};
    controller.moveToThread(&thread);

    QObject::connect(&thread, &QThread::started, &controller, &TopographyController::init);
    QObject::connect(&thread, &QThread::finished, &controller, &TopographyController::deleteLater);

    thread.start();

    return a.exec();
}
