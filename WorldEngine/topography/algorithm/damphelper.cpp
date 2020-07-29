#include "damphelper.h"

#include <QPoint>
#include <QPointF>
#include <cmath>
#include <utility>
#ifdef QT_DEBUG
#include <QDebug>
#endif

DampHelper::DampHelper(Topography* _model, int _sample_count, float _step, int _max_step)
    : model{_model},
      sample_count(_sample_count),
      step{_step},
      max_step{_max_step},
      dd{(float)(TwicePI / _sample_count)} {}

int DampHelper::stepUp(int x, int y, float dir) {
    int count = 0;
    auto&& dp = QPointF{std::cos(dir), std::sin(dir)} * step;
    for (auto&& real = QPointF{x, y}; count <= max_step; real += dp, ++count) {
        if (!model->accepted(real)) {
            return max_step;
        }
        if (model->at(real).isDeepWater()) {
            break;
        }
    }
    return count;
}
