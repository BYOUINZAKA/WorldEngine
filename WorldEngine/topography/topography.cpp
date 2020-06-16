// #define ISDEBUGGING
#include "topography.h"
#include "unit/dynamicoptions.h"

#ifdef ISDEBUGGING
#   include <QDebug>
#endif

Topography::Topography(SizeType _length, SizeType _width, QObject *parent)
    : QObject(parent),
      length(_length),
      width(_width),
      map(_length * _width)
{}

Topography::Topography(QObject *parent)
    : Topography{DynamicOptions::Length, DynamicOptions::Width, parent}
{}
