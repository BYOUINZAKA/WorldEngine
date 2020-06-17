#ifndef TOPOGRAPHYVIEW_H
#define TOPOGRAPHYVIEW_H

#include <QOpenGLWidget>
#include <QTransform>

#include "topography.h"
#include "topographycontroller.h"

namespace Ui
{
    class TopographyView;
}

class TopographyView : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit TopographyView(Topography *model, QWidget *parent = 0);
    explicit TopographyView(const TopographyController &tc, QWidget *parent = 0);
    ~TopographyView();

protected:
    void paintGL() override;

private:
    Ui::TopographyView *ui;
    Topography *model;
    QTransform translator;
};

#endif // TOPOGRAPHYVIEW_H
