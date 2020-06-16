#ifndef TEST_H
#define TEST_H

#include <QObject>

class test : public QObject
{
    Q_OBJECT
public:
    explicit test(QObject *parent = nullptr);
    static int value;

signals:

public slots:
    void setValue(int value);
};

class contr : public QObject
{
    Q_OBJECT
public:
    explicit contr(QObject *parent = nullptr);

signals:
    void set(int value);

public slots:
};

#endif // TEST_H
