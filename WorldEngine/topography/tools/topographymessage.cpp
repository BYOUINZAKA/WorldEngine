#include "topographymessage.h"
#include "ui_topographymessage.h"

TopographyMessage::TopographyMessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TopographyMessage)
{
    ui->setupUi(this);
}

TopographyMessage::~TopographyMessage()
{
    delete ui;
}
