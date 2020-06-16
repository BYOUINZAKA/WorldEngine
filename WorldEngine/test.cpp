#include "test.h"

int test::value = 0;

test::test(QObject *parent) : QObject(parent)
{

}


void test::setValue(int value)
{
    test::value = value;
}


contr::contr(QObject *parent) : QObject(parent)
{
    connect(this, &contr::set, [&](int value){test::value = value;});
}
