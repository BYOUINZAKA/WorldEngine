#ifndef DYNAMICOPTIONS_H
#define DYNAMICOPTIONS_H

#include <QObject>

class DynamicOptions : public QObject
{
    Q_OBJECT
public:
    explicit DynamicOptions(QObject *parent = nullptr);

signals:

public slots:
};

#endif // DYNAMICOPTIONS_H