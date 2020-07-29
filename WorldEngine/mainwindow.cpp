#include "mainwindow.h"

#include "topography/topographyview.h"
#include "topography/topography.h"
#include "topography/topographycontroller.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent},
      ui{new Ui::MainWindow},
      map_thread{this},
      tp_model{new Topography{this}},
      tp_view{new TopographyView{tp_model}},
      tp_con{new TopographyController{tp_model}} {
    ui->setupUi(this);

    tp_con->moveToThread(&map_thread);

    connect(&map_thread, &QThread::started, tp_con, &TopographyController::init);
    connect(&map_thread, &QThread::finished, tp_con, &TopographyController::deleteLater);
}

MainWindow::~MainWindow() {
    delete ui;
    delete tp_view;
}

void MainWindow::on_button_buildmap_clicked() {
    if (!map_thread.isRunning()) {
        map_thread.start();
    }
    tp_view->show();
}
