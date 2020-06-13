#ifndef __UNIT_AREA_H_
#define __UNIT_AREA_H_

#include "options.h"

struct Area
{
    float altitude;
    float temp;
    float damp;

    inline Area(float _altitude) : altitude(GetValidValue(Altitude, _altitude)) {}
    Area(float _altitude, float _temp, float _damp);
};

#endif
