#ifndef TOPOGRAPHYVIEW_H
#define TOPOGRAPHYVIEW_H

#include <QWidget>

namespace Ui {
    class TopographyView;
}

class TopographyView : public QWidget
{
    Q_OBJECT

public:
    explicit TopographyView(QWidget *parent = 0);
    ~TopographyView();

private:
    Ui::TopographyView *ui;
};

#endif // TOPOGRAPHYVIEW_H
