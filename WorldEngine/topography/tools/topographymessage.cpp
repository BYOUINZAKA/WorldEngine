#include "topographymessage.h"

#include <QMouseEvent>
#include <QTimer>

#include "ui_topographymessage.h"

TopographyMessage::TopographyMessage(QWidget* parent)
    : QFrame(parent), ui(new Ui::TopographyMessage) {
    ui->setupUi(this);
    hide();
    // setStyleSheet("background-color:rgba(255, 255, 255, 50%);");
}

TopographyMessage::~TopographyMessage() { delete ui; }

void TopographyMessage::setMessage(QMouseEvent* event, int x, int y, const Area& msg) {
    this->move(event->pos());

    ui->pos_lbl->setText(QString{"(%1, %2)"}.arg(x).arg(y));
    ui->altitude_lcd->display(msg.altitude);
    ui->temp_lcd->display(msg.temp);
    ui->damp_lcd->display(msg.damp);

    show();

    dynamic_cast<QWidget*>(parent())->setCursor(Qt::CrossCursor);
}
