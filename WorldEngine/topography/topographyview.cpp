#include "topographyview.h"
#include "ui_topographyview.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QPainter>
#include <QOpenGLFunctions>
#include <QString>
#include <QPen>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>

#include "unit/colors.h"

TopographyView::TopographyView(Topography *model, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::TopographyView),
      model{model},
      enlarge{1}
{
    ui->setupUi(this);
    this->show();
    this->setFixedSize(this->width(), this->height());

    base_translator = QTransform{this->width() / (double)model->getWidth(), 0, 0,
                                 this->height() / (double)model->getLength(), 0, 0};
    translator = setTranslatorByMulti(base_translator, enlarge, 0, 0, enlarge, 0, 0);

    center.setX(model->getWidth() / 2.0);
    center.setY(model->getLength() / 2.0);
    connect(model, &Topography::refreshed, [this]() { QWidget::update(); });
}

TopographyView::TopographyView(const TopographyController &tpc, QWidget *parent)
    : TopographyView{tpc.getModel(), parent}
{
}

TopographyView::~TopographyView()
{
    delete ui;
}

void TopographyView::paintEvent(QPaintEvent *event)
{
    QPainter painter{this};
    painter.setClipRect(0, 0, this->width(), this->height());
    painter.setTransform(translator);

    auto width = model->getWidth(), length = model->getLength();

    double w = width / 2.0 / enlarge, l = length / 2.0 / enlarge;
    int i_begin = center.y() - l, i_end = l + center.y() + 0.5;
    int j_begin = center.x() - w, j_end = l + center.x() + 0.5;

    for (int i = i_begin, y = 0; i <= i_end; ++i, ++y)
    {
        for (int j = j_begin, x = 0; j <= j_end; ++j, ++x)
        {
            if (model->enable(i, j))
                painter.setPen(QPen{ColorTraits::fromAltitude(model->at(i, j))});
            else
                painter.setPen(QPen{Qt::black});
            painter.drawPoint(x, y);
        }
    }

    QWidget::paintEvent(event);
}

void TopographyView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        this->setCursor(Qt::ClosedHandCursor);
    }
}

void TopographyView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        this->setCursor(Qt::OpenHandCursor);
        event->accept();
        QTimer::singleShot(50, [this]() {
            this->setCursor(Qt::ArrowCursor);
        });
    }
}

void TopographyView::wheelEvent(QWheelEvent *event)
{
    int new_value = enlarge + event->delta() / 120.0;
    if (new_value <= kMaxEnlarge && new_value >= kMinEnlarge)
    {
        enlarge = new_value;
        translator = setTranslatorByMulti(base_translator, enlarge, 0, 0, enlarge, 0, 0);
        QWidget::repaint();
    }
    event->accept();
}
