#ifndef DAMPHELPER_H
#define DAMPHELPER_H

#include <QVector>

#include "topography/topography.h"

class DampHelper
{
    Topography *model;
    QVector<int8_t> flags;

public:
    explicit DampHelper(Topography *model);
};

#endif // DAMPHELPER_H
