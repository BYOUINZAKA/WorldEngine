#include "topographyview.h"

#include "ui_topographyview.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QPen>
#include <QString>
#include <QTimer>
#include <QWheelEvent>

#include "topography.h"
#include "unit/colors.h"
#include "tools/topographymessage.h"
#include "topographycontroller.h"

using namespace std;

TopographyView::TopographyView(Topography* model, int enlarge, QWidget* parent)
    : QWidget(parent),
      ui(new Ui::TopographyView),
      area_message_box(new TopographyMessage(this)),
      model{model},
      enlarge{max(int(kMinEnlarge), min(enlarge, int(kMaxEnlarge)))} {
    ui->setupUi(this);
    setFixedSize(this->width(), this->height());

    base_translator = QTransform{this->width() / (double)model->getWidth(),   0, 0,
                                 this->height() / (double)model->getLength(), 0, 0};

    translator = setTranslatorByMulti(base_translator, enlarge, 0, 0, enlarge, 0, 0);

    center = QPoint{model->getWidth() / 2.0, model->getLength() / 2.0};

    connect(model, &Topography::refreshed, [this] { QWidget::update(); });
    connect(this, &TopographyView::sendAreaMessage, area_message_box,
            &TopographyMessage::setMessage);
}

TopographyView::TopographyView(TopographyController* tpc, int enlarge, QWidget* parent)
    : TopographyView{tpc->getModel(), enlarge, parent} {}

TopographyView::~TopographyView() { delete ui; }

void TopographyView::paintEvent(QPaintEvent* event) {
    QPainter painter{this};
    painter.setClipRect(0, 0, this->width(), this->height());
    painter.setTransform(translator);

    auto width = model->getWidth(), length = model->getLength();

    double w = width / 2.0 / enlarge, l = length / 2.0 / enlarge;

    int32_t i_begin = center.y() - l, i_end = l + center.y() + 0.5;
    int32_t j_begin = center.x() - w, j_end = l + center.x() + 0.5;

    for (auto i = i_begin, y = 0; i <= i_end; ++i, ++y) {
        for (auto j = j_begin, x = 0; j <= j_end; ++j, ++x) {
            if (model->accepted(i, j)) {
                painter.setPen(QPen{ColorTraits::fromAltitude(model->at(i, j))});
                painter.drawPoint(x, y);
            }
        }
    }

    QWidget::paintEvent(event);
}

void TopographyView::mousePressEvent(QMouseEvent* event) {
    if (!area_message_box->close()) area_message_box->hide();

    if (event->button() == Qt::LeftButton) {
        this->setCursor(Qt::ClosedHandCursor);
        active_pos = event->globalPos();

        QWidget::update();

    } else if (event->button() == Qt::RightButton) {
        QPoint dpos = QPoint{width() / 2.0, height() / 2.0} - event->pos();

        QPointF mid_pos{dpos.x() / double(enlarge) / base_translator.m11(),
                        dpos.y() / double(enlarge) / base_translator.m22()};

        QPoint pos{center.x() - mid_pos.x(), center.y() - mid_pos.y()};

        // 把当前点的土壤信息传递给消息框
        if (model->accepted(pos)) {
            emit sendAreaMessage(event, pos.x(), pos.y(), model->at(pos.y(), pos.x()));
        }
    }
}

void TopographyView::mouseMoveEvent(QMouseEvent* event) {
    // 只有在按下左键时才会触发移动事件
    if ((event->buttons() & Qt::LeftButton) && active_pos.hasValue() &&
        event->globalPos() != *active_pos) {
        center += (*active_pos - event->globalPos()) / (enlarge * enlarge);
        QWidget::update();
    }
}

void TopographyView::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && bool(active_pos)) {
        // 松开左键时，等待短时间再变回原本的箭头
        active_pos = null_optional;
        this->setCursor(Qt::OpenHandCursor);
        QTimer::singleShot(10, [this] { this->setCursor(Qt::ArrowCursor); });
        QWidget::update();
    }
}

void TopographyView::wheelEvent(QWheelEvent* event) {
    if (!area_message_box->close()) area_message_box->hide();

    const int32_t new_value = enlarge + event->delta() / 120.0;

    if (new_value >= kMinEnlarge && new_value <= kMaxEnlarge) {
        enlarge = new_value;
        translator = setTranslatorByMulti(base_translator, enlarge, 0, 0, enlarge, 0, 0);
        QWidget::update();
    }

    event->accept();
}
