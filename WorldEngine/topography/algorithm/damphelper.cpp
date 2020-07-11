#include "damphelper.h"

#include <cmath>
#include <utility>

#include <QPoint>
#include <QPointF>
#ifdef QT_DEBUG
#include <QDebug>
#endif

DampHelper::DampHelper(Topography* _model, int _sample_count, float _step, int _max_step)
    : model { _model }
    , sample_count(_sample_count)
    , step { _step }
    , max_step { _max_step }
    , dd { (float)(TwicePI / _sample_count) }
{
}

int DampHelper::stepUp(int x, int y, float dir)
{
    int count = 0;
    auto&& dp = QPointF { std::cos(dir), std::sin(dir) } * step;
    for (auto&& real = QPointF { x, y }; count <= max_step; real += dp, ++count) {
        if (!model->accepted(real)) {
            return max_step;
        }
        if (model->at(real).isDeepWater()) {
            break;
        }
    }
    return count;
}

/*
double DampHelper::dfs(int i, int j)
{
    auto &&flag_ij = flags[i * width + j];
    auto &&model_ij = model->at(i, j);
    flag_ij = kSeen;
    double sum = 0.0;
    int count = 0;
    for(auto &&d: dir)
    {
        int di = i + d[0];
        int dj = j + d[0];
        if(model->accepted(di, dj))
        {
            auto &&flag = flags[di * width + dj];
            if(flag == kChanged)
            {
                auto &&m = model->at(di, dj);
                sum += m.damp;
            }
            else if(flag == kStrange)
            {
                sum += dfs(di, dj);
            }
            ++count;
        }
    }
#ifdef QT_DEBUG
    assert(count != 0);
#endif
    double res = sum / count;
    model_ij.damp = res < 1e-6 ? 0 : res;
    flag_ij = kChanged;
    return model_ij.damp;
}
*/

/*
void DampHelper::solve()
{
    for(int i = 0; i < length; ++i)
    {
        for(int j = 0; j < width; ++j)
        {
            if(flags[i * width + j] == kStrange)
            {
#ifdef QT_DEBUG
                qDebug() << i << j;
#endif
                dfs(i, j);
            }
        }
    }
#ifdef QT_DEBUG
    qDebug() << "end";
#endif
}
*/
