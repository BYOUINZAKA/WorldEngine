#ifndef __UNIT_COLORS_H
#define __UNIT_COLORS_H

#include <type_traits>

#include <QColor>

#include "area.h"
#include "optional.h"
#include "options.h"

#ifdef QT_DEBUG
#include <cassert>
#endif

namespace ColorTraits {
/*
    constexpr QColor colors[] = {
        QColor{0,0,143},
        QColor{0,0,255},
        QColor{0, 0, 155},
        QColor{200,255, 0},
        QColor{200,0,0},
        QColor{128,0,0}
    };
    */

template <typename T>
constexpr typename std::enable_if<std::is_arithmetic<typename std::remove_reference<T>::type>::value
        || std::is_arithmetic<typename std::remove_cv<T>::type>::value,
    bool>::type
accepted(T&& value)
{
    return std::forward<T>(value) >= 0 && std::forward<T>(value) <= 255;
}

template <typename T, typename... Args>
constexpr typename std::enable_if<(sizeof...(Args) < 3)
        && (std::is_arithmetic<typename std::remove_reference<T>::type>::value
            || std::is_arithmetic<typename std::remove_cv<T>::type>::value),
    bool>::type
accepted(T&& value, Args&&... args)
{
    return accepted(std::forward<T>(value)) && accepted(std::forward<Args>(args)...);
}

template <typename... Args>
constexpr typename std::enable_if<sizeof...(Args) <= 3, Optional<QColor>>::type forwardSaveColor(
    Args&&... args)
{
    return accepted(std::forward<Args>(args)...)
        ? Optional<QColor> { QColor { static_cast<int>(std::forward<Args>(args))... } }
        : Optional<QColor> {};
}

template <class T>
typename std::enable_if<std::is_convertible<T, Area>::value, QColor>::type fromAltitude(T&& area)
{
    auto altitude = std::forward<T>(area).altitude;
    using altitude_t = decltype(altitude);

    auto horizon = static_cast<altitude_t>(Options::Horizontal);
    auto maxAltitude = static_cast<altitude_t>(Options::MaxAltitude);
    auto minAltitude = static_cast<altitude_t>(Options::MinAltitude);

    if (altitude < horizon) {
        int value = 100 - 100.0 * (altitude - minAltitude) / (horizon - minAltitude);
#ifdef QT_DEBUG
        assert(value >= 0 && value <= 255);
#endif
        return QColor { 0, 250 - value * 2, 250 - value };
    } else {
        int value = 255 - int(200.0 * (maxAltitude - altitude) / (maxAltitude - horizon));
        // int value = 150.0 * (maxAltitude - altitude) / (maxAltitude -
        // horizon);
#ifdef QT_DEBUG
        assert(accepted(value, 250, 0));
#endif
        // return QColor{255 - value, value, 0};
        return QColor { value, 250 - value / 2, 0 };
    }
}
} // namespace ColorTraits

#endif
