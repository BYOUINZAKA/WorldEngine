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

TopographyView::TopographyView(Topography *model, int enlarge, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::TopographyView),
      model{model},
      enlarge{enlarge > kMaxEnlarge ? kMaxEnlarge : (enlarge < kMinEnlarge ? kMinEnlarge : enlarge)},
      area_message_box(new TopographyMessage(this))
{
    ui->setupUi(this);
    show();
    setFixedSize(this->width(), this->height());

    base_step = QPointF{this->width() / (double)model->getWidth(), this->height() / (double)model->getLength()};
    base_translator = QTransform{base_step.x(), 0, 0, base_step.y(), 0, 0};
    translator = setTranslatorByMulti(base_translator, enlarge, 0, 0, enlarge, 0, 0);
    center.setX(model->getWidth() / 2.0);
    center.setY(model->getLength() / 2.0);

    connect(model, &Topography::refreshed, [this] { QWidget::update(); });
    connect(this, &TopographyView::sendAreaMessage, area_message_box, &TopographyMessage::setMessage);
}

TopographyView::TopographyView(const TopographyController &tpc, int enlarge, QWidget *parent)
    : TopographyView{tpc.getModel(), enlarge, parent}
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
            if (model->accepted(i, j))
            {
                painter.setPen(QPen{ColorTraits::fromAltitude(model->at(i, j))});
                painter.drawPoint(x, y);
            }
        }
    }

    QWidget::paintEvent(event);
}

void TopographyView::mousePressEvent(QMouseEvent *event)
{
    if (!area_message_box->close())
        area_message_box->hide();
    if (event->button() == Qt::LeftButton)
    {
        this->setCursor(Qt::ClosedHandCursor);
        active_pos = event->globalPos();
        QWidget::update();
    }
    else if (event->button() == Qt::RightButton)
    {
        QPoint dpos = QPoint{width() / 2.0, height() / 2.0} - event->pos();
        QPointF midpos{dpos.x() / double(enlarge) / base_step.x(), dpos.y() / double(enlarge) / base_step.y()};
        QPoint pos{center.x() - midpos.x(), center.y() - midpos.y()};

        if (model->accepted(pos))
        {
            emit sendAreaMessage(event, pos.x(), pos.y(), model->at(pos.y(), pos.x()));
        }
    }
}

void TopographyView::mouseMoveEvent(QMouseEvent *event)
{
    if (active_pos.hasValue() && event->globalPos() != *active_pos)
    {
        center += (*active_pos - event->globalPos()) / (enlarge * enlarge);
        QWidget::update();
    }
}

void TopographyView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && bool(active_pos))
    {
        active_pos = null_optional;
        this->setCursor(Qt::OpenHandCursor);
        QTimer::singleShot(10, [this] {
            this->setCursor(Qt::ArrowCursor);
        });
        QWidget::update();
    }
}

void TopographyView::wheelEvent(QWheelEvent *event)
{
    if (!area_message_box->close())
        area_message_box->hide();
    int new_value = enlarge + event->delta() / 120.0;
    if (new_value <= kMaxEnlarge && new_value >= kMinEnlarge)
    {
        enlarge = new_value;
        translator = setTranslatorByMulti(base_translator, enlarge, 0, 0, enlarge, 0, 0);
        QWidget::repaint();
    }
    event->accept();
}
