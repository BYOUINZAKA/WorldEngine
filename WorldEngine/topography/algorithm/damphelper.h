#ifndef DAMPHELPER_H
#define DAMPHELPER_H

#include <functional>
#include <tuple>

#include <QVector>

#include "functional.h"
#include "topography/topography.h"

class DampHelper {
public:
    constexpr static const double TwicePI
        = 3.14159265358979323846264338327950288419716939937510582097494 * 2.0;

private:
    Topography* model;
    int sample_count;
    float step;
    int max_step;
    float dd;

public:
    DampHelper(Topography* _model, int _sample_count = 16, float _step = 20.0f, int _max_step = 50);

    int stepUp(int x, int y, float dir);

    template <class F = double (*)(double, double)> void solve(F&& decay = &Decay::solid_linear)
    {
        auto width = model->getWidth(), length = model->getLength();
        auto it = model->begin();
        for (int y = 0; y < length; ++y) {
            for (int x = 0; x < width; ++x, ++it) {
                if (!it->isDeepWater())
                    it->damp = spread(x, y, std::forward<F>(decay));
            }
        }
#ifdef QT_DEBUG
        qDebug("solve end.");
#endif
    }

    template <class F> float spread(int x, int y, F&& decay)
    {
        int count = 0;
        float sum = 0.0f;
        auto height = model->at(y, x).altitude;
        for (float dir = 0.0f; dir < TwicePI; dir += dd, ++count) {
            sum += decay(stepUp(x, y, dir) * step, height) * float(Options::IsWater);
        }
        return sum / count;
    }
};

#endif // DAMPHELPER_H
