#ifndef __UNIT_DYNAMICOPTIONS_H
#define __UNIT_DYNAMICOPTIONS_H

#include <QObject>

class DynamicOptions : public QObject {
    Q_OBJECT
public:
    explicit DynamicOptions(QObject* parent = nullptr);
    static int32_t Length;
    static int32_t Width;

signals:

public slots:

};

#endif  // __UNIT_DYNAMICOPTIONS_H
