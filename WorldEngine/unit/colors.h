#ifndef __UNIT_COLORS_H
#define __UNIT_COLORS_H

#include <type_traits>

#include <QColor>

#include "area.h"
#include "options.h"

#ifdef QT_DEBUG
#include <cassert>
#endif

template <class T>
typename std::enable_if<
    std::is_convertible<T, Area>::value, QColor>::type
altitudeToColor(T &&area)
{
    auto altitude = std::forward<T>(area).altitude;
    using altitude_t = decltype(altitude);

    auto horizon = static_cast<altitude_t>(Options::Horizontal);
    auto maxAltitude = static_cast<altitude_t>(Options::MaxAltitude);
    auto minAltitude = static_cast<altitude_t>(Options::MinAltitude);

    if (altitude < horizon)
    {
            int value = 155 + int(100.0 * (altitude - minAltitude) / (horizon - minAltitude));
#ifdef QT_DEBUG
            assert(value >= 0 && value <= 255);
#endif
            return QColor{0, 0, value};
    }
    else
    {
            int value = 255 - int(255.0 * (maxAltitude - altitude) / (maxAltitude - horizon));
#ifdef QT_DEBUG
            assert(value >= 0 && value <= 255);
#endif
            return QColor{value, 255, 0};
    }
}

#endif
