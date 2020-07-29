#ifndef __TOPOGRAPHY_ALGORITHM_FUNCTIONAL_H
#define __TOPOGRAPHY_ALGORITHM_FUNCTIONAL_H

#include <cmath>
#include <type_traits>

// There are some callback functions as strategies for initialization of random map.
namespace Shape {
template <int N = 1, int D = 2, typename std::enable_if<N >= 1 && D >= 1, int>::type = 0>
constexpr double normal() {
    return (double)N / (double)D;
}

constexpr double mountain(double d, double m) { return (m - d) / m; }

constexpr double basin(double d, double m) { return d / m; }

}  // namespace Shape

namespace Mapping {
constexpr double normal(double r) { return r; }

template <int N = 1, typename std::enable_if<N >= 1, int>::type = 0>
constexpr double extreme(double r) {
    return std::tanh((r - 1) * N) + 0.5;
}

template <int N = 1, typename std::enable_if<N >= 0, int>::type = 0>
constexpr double unextreme(double r) {
    return std::tanh((r - 1) / N) + 0.5;
}

/*
    constexpr double middle(double r)
    {
        return r;
    }
    */

}  // namespace Mapping

namespace Decay {
template <int N = 20, typename std::enable_if<N >= 1, int>::type = 0>
constexpr double linear(int step) {
    return step > N ? 0 : 1.0 - step / double(N);
}

template <int Max = 400, typename std::enable_if<Max >= 1, int>::type = 0>
constexpr double plane_linear(double len, double height) {
    return len > Max ? 0 : 1.0 - len / Max;
}

template <int Max = 400, typename std::enable_if<Max >= 1, int>::type = 0>
constexpr double solid_linear(double len, double height) {
    return len = std::sqrt(len * len + height * height), len > Max ? 0 : 1.0 - len / Max;
}

constexpr double plane_inverse(double len, double height) { return 1.0 / (len + 1); }

constexpr double solid_inverse(double len, double height) {
    return len = std::sqrt(len * len + height * height), 1.0 / (len + 1);
}

}  // namespace Decay

#endif  // __TOPOGRAPHY_ALGORITHM_FUNCTIONAL_H
