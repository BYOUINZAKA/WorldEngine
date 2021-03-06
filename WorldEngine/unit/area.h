#ifndef __UNIT_AREA_H
#define __UNIT_AREA_H

#include "options.h"

constexpr float getDefaultTemp(float _altitude) {
    return GET_VALID_VALUE(Altitude, _altitude) > (float)Options::Horizontal
               // Temperatures decrease by 0.6 degrees per 100 meters
               ? (float)Options::BaseTemp -
                     (GET_VALID_VALUE(Altitude, _altitude) - (float)Options::Horizontal) /
                         166.66666666667
               : (float)Options::WaterTemp;
}

struct Area {
    float altitude;
    float temp;
    float damp;

    constexpr Area() : Area{0, 0, 0} {}

    constexpr Area(float _altitude)
        : altitude(GET_VALID_VALUE(Altitude, _altitude)),
          temp(getDefaultTemp(_altitude)),
          damp(0) {}
          
    constexpr Area(float _altitude, float _temp, float _damp)
        : altitude(GET_VALID_VALUE(Altitude, _altitude)),
          temp(GET_VALID_VALUE(Temp, _temp)),
          damp(GET_VALID_VALUE(Damp, _damp)) {}

    inline float defaultTemp() {
        temp = getDefaultTemp(altitude);
        return temp;
    }

    inline void setAltitude(float _altitude) {
        altitude = GET_VALID_VALUE(Altitude, _altitude);
        defaultTemp();
    }

    inline float height() const { return altitude - float(Options::MinAltitude); }

    inline bool isDeepWater() const { return altitude < float(Options::Horizontal); }
};

#endif
