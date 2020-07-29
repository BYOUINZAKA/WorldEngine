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
    const auto&& vec = QPointF{std::cos(dir), std::sin(dir)} * step;

    int count = 0;

    for (auto&& real = QPointF{x, y}; count <= max_step; real += vec, ++count) {
        // 超出地图边界，则距离水源无穷远
        if (!model->accepted(real)) return max_step;
        // 找到水源，得到步数
        if (model->at(real).isDeepWater()) break;
    }

    return count;
}
