#ifndef __UNIT_COLORS_H
#define __UNIT_COLORS_H

#include <type_traits>

#include <QColor>

#include "area.h"
#include "options.h"
#include "optional.h"

#ifdef QT_DEBUG
#include <cassert>
#endif

namespace ColorTraits
{
    template <typename T>
    constexpr
        typename std::enable_if<
            std::is_arithmetic<typename std::remove_reference<T>::type>::value ||
                std::is_arithmetic<typename std::remove_cv<T>::type>::value,
            bool>::type
        accepted(T &&value)
    {
        return std::forward<T>(value) >= 0 && std::forward<T>(value) <= 255;
    }

    template <typename T, typename... Args>
    constexpr
        typename std::enable_if<
            (sizeof...(Args) < 3) &&
                (std::is_arithmetic<typename std::remove_reference<T>::type>::value ||
                 std::is_arithmetic<typename std::remove_cv<T>::type>::value),
            bool>::type
        accepted(T &&value, Args &&... args)
    {
        return accepted(std::forward<T>(value)) && accepted(std::forward<Args>(args)...);
    }

    template <typename... Args>
    constexpr typename std::enable_if<
        sizeof...(Args) <= 3,
        Optional<QColor>>::type
    forwardSaveColor(Args &&... args)
    {
        return accepted(std::forward<Args>(args)...)
                   ? Optional<QColor>{QColor{static_cast<int>(std::forward<Args>(args))...}}
                   : Optional<QColor>{};
    }

    template <class T>
    typename std::enable_if<
        std::is_convertible<T, Area>::value, QColor>::type
    fromAltitude(T &&area)
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
            assert(accepted(0, 50, value));
#endif
            return QColor{0, 50, value};
        }
        else
        {
            int value = 255 - int(100.0 * (maxAltitude - altitude) / (maxAltitude - horizon));
            int damp = 255 * std::forward<T>(area).damp / 1000.0;
#ifdef QT_DEBUG
            assert(accepted(value, 250, damp));
#endif
            return QColor{value, 250, damp};
        }
    }
} // namespace ColorTraits

#endif
