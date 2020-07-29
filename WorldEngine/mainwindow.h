#ifndef __MAINWINDOW_H
#define __MAINWINDOW_H

#include <QMainWindow>
#include <QThread>

namespace Ui {
class MainWindow;
}

class Topography;
class TopographyView;
class TopographyController;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private slots:
    void on_button_buildmap_clicked();

private:
    Ui::MainWindow* ui;

    // 地图逻辑线程，与主程序的生命周期一致，不会被销毁
    QThread map_thread;

    Topography* tp_model;
    TopographyView* tp_view;
    TopographyController* tp_con;
};

#endif  // __MAINWINDOW_H
