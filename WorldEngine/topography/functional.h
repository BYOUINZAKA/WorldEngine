#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

#include <type_traits>

namespace Shape
{
    template <int N = 2, typename std::enable_if<(N > 0), int>::type = 0>
    inline double normal()
    {
        return 1.0 / N;
    }

    inline double mountain(double d, double m)
    {
        return (m - d) / m;
    }

    inline double basin(double d, double m)
    {
        return d / m;
    }

} // namespace Shape

namespace Mapping
{
    inline double normal(double r)
    {
        return r;
    }

    template <int N = 1, typename std::enable_if<N >= 0, int>::type = 0>
    inline double extreme(double r)
    {
        return std::tanh((r - 1) * N) + 0.5;
    }

    template <int N, typename std::enable_if<(N < 0), int>::type = 0>
    inline double extreme(double r)
    {
        return std::tanh((r - 1) / (-N)) + 0.5;
    }

    inline double middle(double r)
    {
        return r;
    }

} // namespace Mapping

#endif // FUNCTIONAL_H
