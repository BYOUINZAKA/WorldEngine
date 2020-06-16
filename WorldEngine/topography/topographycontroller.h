#ifndef TOPOGRAPHYCONTROLLER_H
#define TOPOGRAPHYCONTROLLER_H

#include <QObject>

class TopographyController : public QObject
{
    Q_OBJECT
public:
    explicit TopographyController(QObject *parent = nullptr);

signals:

public slots:
};

#endif // TOPOGRAPHYCONTROLLER_H