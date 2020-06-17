#include "mainwindow.h"

#include <functional>

#include <QApplication>
#include <QDebug>
#include <QThread>

#include "topography/topographycontroller.h"
#include "topography/topographyview.h"
#include "test.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(800, 800);
    w.show();

    QThread thread;

    Topography tp;
    TopographyView tpv{&tp, &w};

    TopographyController tpc{&tp};

    QObject::connect(&thread, &QThread::started, &tpc, &TopographyController::init);
    QObject::connect(&thread, &QThread::finished, &tpc, &TopographyController::deleteLater);
    QObject::connect(&w, &MainWindow::destroyed, [&thread](){
        thread.quit();
    });

    thread.start();
    tpv.show();

    return a.exec();
}
