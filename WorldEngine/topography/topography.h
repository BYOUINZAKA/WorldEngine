#ifndef TOPOGRAPHY_H
#define TOPOGRAPHY_H

#include <QObject>

class Topography : public QObject
{
    Q_OBJECT
public:
    explicit Topography(QObject *parent = nullptr);

signals:

public slots:
};

#endif // TOPOGRAPHY_H