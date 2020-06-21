#ifndef DAMPHELPER_H
#define DAMPHELPER_H

#include <functional>
#include <tuple>

#include <QVector>

#include "topography/topography.h"
#include "functional.h"

class DampHelper
{
public:
    constexpr static const double TwicePI = 3.14159265358979323846264338327950288419716939937510582097494 * 2.0;

private:
    Topography *model;
    std::function<double(int)> decay;
    float step;
    int sample_count;
    int max_step;

public:
    DampHelper(Topography *_model,
               int _N = 16,
               float _step = 20.0f,
               std::function<double(int)> _decay = &Decay::linear);

    void solve(int level = 0);
    float spread(int x, int y);
    int stepUp(int x, int y, float dir);
    void setByWindow(int x, int y, float value, int level);

    // double dfs(int i, int j);
};

#endif // DAMPHELPER_H
