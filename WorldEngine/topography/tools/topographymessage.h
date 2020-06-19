#ifndef TOPOGRAPHYMESSAGE_H
#define TOPOGRAPHYMESSAGE_H

#include <QDialog>

namespace Ui {
    class TopographyMessage;
}

class TopographyMessage : public QDialog
{
    Q_OBJECT

public:
    explicit TopographyMessage(QWidget *parent = 0);
    ~TopographyMessage();

private:
    Ui::TopographyMessage *ui;
};

#endif // TOPOGRAPHYMESSAGE_H
