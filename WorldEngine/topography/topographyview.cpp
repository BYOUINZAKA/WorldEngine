#include "topographyview.h"
#include "ui_topographyview.h"

TopographyView::TopographyView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TopographyView)
{
    ui->setupUi(this);
}

TopographyView::~TopographyView()
{
    delete ui;
}
