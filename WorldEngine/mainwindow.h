#ifndef __MAINWINDOW_H
#define __MAINWINDOW_H

#include <QMainWindow>
#include <QThread>

#include "topography/topographyview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private slots:
    void on_button_buildmap_clicked();

private:
    Ui::MainWindow* ui;

    QThread map_thread;

    Topography tp_model;
    TopographyView tp_view;
    TopographyController tp_con;
};

#endif  // __MAINWINDOW_H
