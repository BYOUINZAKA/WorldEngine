/*
 * @Author: Hata
 * @Date: 2020-06-19 16:38:30
 * @LastEditors: Hata
 * @LastEditTime: 2020-07-29 15:28:13
 * @FilePath: \WorldEngine\WorldEngine\topography\algorithm\damphelper.h
 * @Description: 基于蒙地卡罗算法的土壤湿度计算辅助类
 */

#ifndef __TOPOGRAPHY_ALGORITHM_DAMPHELPER_H
#define __TOPOGRAPHY_ALGORITHM_DAMPHELPER_H

#include <QVector>
#include <functional>
#include <tuple>
#include <cmath>

#include "functional.h"
#include "topography/topography.h"

class DampHelper {
public:
    constexpr static const double TwicePI =
        3.14159265358979323846264338327950288419716939937510582097494 * 2.0;

private:
    Topography* model;
    int sample_count;
    int max_step;
    float step;
    float dd;

public:
    DampHelper(Topography* _model, int _sample_count = 16, float _step = 20.0f, int _max_step = 50);

    int stepUp(int x, int y, float dir);

    template <class F = double (*)(double, double)>
    void solve(F&& decay = &Decay::solid_linear) {
        const int width = model->getWidth(), length = model->getLength();

        auto it = model->begin();

        for (int y = 0; y < length; ++y) {
            for (int x = 0; x < width; ++x, ++it) {
                if (!it->isDeepWater()) it->damp = spread(x, y, std::forward<F>(decay));
            }
        }
    }

    template <class F>
    float spread(int x, int y, F&& decay) {
        int count = 0;
        float sum = 0.0f;

        cosnt auto height = model->at(y, x).altitude;

        for (float dir = 0.0f; dir < TwicePI; dir += dd, ++count) {
            sum += decay(stepUp(x, y, dir) * step, height) * float(Options::IsWater);
        }

        return sum / count;
    }
};

#endif  // __TOPOGRAPHY_ALGORITHM_DAMPHELPER_H
