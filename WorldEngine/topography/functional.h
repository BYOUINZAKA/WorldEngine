#ifndef __TOPOGRAPHY_FUNCTIONAL_H
#define __TOPOGRAPHY_FUNCTIONAL_H

#include <type_traits>

// Pack some callback functions as strategies for initialization of random map.
namespace Shape
{
    template <int N = 1, int D = 2,
              typename std::enable_if<N >= 1 && D >= 1, int>::type = 0>
    constexpr double normal()
    {
        return (double)N / (double)D;
    }

    constexpr double mountain(double d, double m)
    {
        return (m - d) / m;
    }

    constexpr double basin(double d, double m)
    {
        return d / m;
    }

} // namespace Shape

namespace Mapping
{
    constexpr double normal(double r)
    {
        return r;
    }

    template <int N = 1, typename std::enable_if<N >= 1, int>::type = 0>
    constexpr double extreme(double r)
    {
        return std::tanh((r - 1) * N) + 0.5;
    }

    template <int N, typename std::enable_if<N <= 0, int>::type = 0>
    constexpr double extreme(double r)
    {
        return std::tanh((r - 1) / (-N)) + 0.5;
    }

    constexpr double middle(double r)
    {
        return r;
    }

} // namespace Mapping

#endif // __TOPOGRAPHY_FUNCTIONAL_H
