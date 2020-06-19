#ifndef TOPOGRAPHYMESSAGE_H
#define TOPOGRAPHYMESSAGE_H

#include <QFrame>

#include "unit/area.h"

namespace Ui
{
    class TopographyMessage;
}

class TopographyMessage : public QFrame
{
    Q_OBJECT

public:
    explicit TopographyMessage(QWidget *parent = 0);
    ~TopographyMessage();

private:
    Ui::TopographyMessage *ui;

public slots:
    void setMessage(QMouseEvent *event, int x, int y, const Area &msg);

public:
    enum
    {
        kMaxDisplayTime = 5000
    };
};

#endif // TOPOGRAPHYMESSAGE_H
