#include "colors.h"
#include <cassert>

QColor altitudeToColor(const Area &area)
{
    auto altitude = area.altitude;
    using altitude_t = decltype(altitude);

    auto horizon = static_cast<altitude_t>(Options::Horizontal);
    auto maxAltitude = static_cast<altitude_t>(Options::MaxAltitude);

    if (altitude < horizon)
    {
        return QColor{0, 50, 200};
    }
    else
    {
        int tran = 255.0 * (maxAltitude - altitude) / (maxAltitude - horizon);
        assert(tran >= 0 && tran <= 255);
        return QColor{255 - tran, 255, 0};
    }
}
