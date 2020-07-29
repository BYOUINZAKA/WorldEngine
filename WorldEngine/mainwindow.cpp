#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent},
      ui{new Ui::MainWindow},
      map_thread{this},
      tp_model{},
      tp_view{&tp_model},
      tp_con{&tp_model} {
    ui->setupUi(this);

    tp_con.moveToThread(&map_thread);

    QObject::connect(&map_thread, &QThread::started, &tp_con, &TopographyController::init);
    QObject::connect(&map_thread, &QThread::finished, &tp_con, &TopographyController::deleteLater);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_button_buildmap_clicked() {
    map_thread.start();
    tp_view.show();
}
