#include "topographyview.h"
#include "ui_topographyview.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QPainter>
#include <QOpenGLFunctions>
#include <QString>
#include <QPen>

#include "unit/colors.h"

TopographyView::TopographyView(Topography *model, QWidget *parent)
    : QOpenGLWidget(parent),
      ui(new Ui::TopographyView),
      model{model}
{
    this->resize(800, 800);
    translator = QTransform{this->width() / (double)model->getWidth(), 0, 0, this->height() / (double)model->getLength(), 0, 0};
    ui->setupUi(this);
    connect(model, &Topography::refreshed, this, &TopographyView::paintGL);
}

TopographyView::TopographyView(const TopographyController &tc, QWidget *parent)
    : TopographyView{tc.getModel(), parent}
{
}

TopographyView::~TopographyView()
{
    delete ui;
}

void TopographyView::paintGL()
{
    QPainter painter{this};
    painter.setTransform(translator);

    auto width = model->getWidth(), length = model->getLength();
    auto it = model->begin();

    for (int i = 0; i < length; ++i)
    {
        for (int j = 0; j < width; ++j, ++it)
        {
            painter.setPen(QPen{altitudeToColor(*it)});
            painter.drawPoint(j, i);
        }
    }
}
