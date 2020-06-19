#include "topographymessage.h"
#include "ui_topographymessage.h"

#include <QMouseEvent>
#include <QTimer>

TopographyMessage::TopographyMessage(QWidget *parent)
    : QFrame(parent),
      ui(new Ui::TopographyMessage)
{
    ui->setupUi(this);
    setStyleSheet("background-color:white;");
    hide();
}

TopographyMessage::~TopographyMessage()
{
    delete ui;
}

void TopographyMessage::setMessage(QMouseEvent *event, int x, int y, const Area &msg)
{
    ui->pos_lbl->setText(QString{"(%1, %2)"}.arg(x).arg(y));
    ui->altitude_lcd->display(msg.altitude);
    ui->temp_lcd->display(msg.temp);
    ui->damp_lcd->display(msg.damp);

    move(event->pos());
    show();

    auto p = dynamic_cast<QWidget *>(parent());
    p->setCursor(Qt::CrossCursor);
}
