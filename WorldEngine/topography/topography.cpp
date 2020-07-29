// #define ISDEBUGGING
#include "topography.h"
/*********************/

#ifdef QT_DEBUG
#include <QDebug>
#endif
#include <algorithm>

#include "unit/dynamicoptions.h"

using namespace std;

Topography::Topography(SizeType _length, SizeType _width, QObject* parent)
    : QObject(parent), length(_length), width(_width), map(_length * _width) {
    emit refreshed();
}

Topography::Topography(QObject* parent)
    : Topography{DynamicOptions::Length, DynamicOptions::Width, parent} {}

float Topography::avgHeight() const {
    double sum = 0.0;
    for(auto&& e : map) sum += e.altitude;
    return sum / getRealSize();
}
