// #define ISDEBUGGING
#include "topography.h"
#ifdef QT_DEBUG
#include <QDebug>
#endif
#include <algorithm>
#include "unit/dynamicoptions.h"

Topography::Topography(SizeType _length, SizeType _width, QObject *parent)
    : QObject(parent),
      length(_length),
      width(_width),
      map(_length * _width)
{
}

Topography::Topography(QObject *parent)
    : Topography{DynamicOptions::Length, DynamicOptions::Width, parent}
{
}

float Topography::avgHeight() const
{
    double sum = 0.0;
    std::for_each(map.cbegin(), map.cend(), [&sum](const Area &area) {
        sum += area.altitude;
    });
    return sum / getRealSize();
}
