#include "damphelper.h"

#include <utility>
#include <cmath>

#include <QPoint>
#include <QPointF>
#ifdef QT_DEBUG
#   include <QDebug>
#endif

DampHelper::DampHelper(Topography *_model, int _sample_count, float _step, std::function<double(int)> _decay)
    : model{_model},
      sample_count(_sample_count),
      step{_step},
      decay{_decay},
      max_step{0}
{
    int limit = std::sqrt(model->getLength() * model->getLength() + model->getWidth() * model->getWidth()) / sample_count;
    assert(decay(0) <= 1.0 && decay(limit) <= 1e-6);
    for(; decay(max_step) > 1e-6 && max_step <= limit; ++max_step);
}

int DampHelper::stepUp(int x, int y, float dir)
{
    int count = 0;
    auto &&dp = QPointF{std::cos(dir), std::sin(dir)} * step;
    for(auto &&real = QPointF{x, y}; count <= max_step; real += dp){
        if(!model->accepted(real)){
            return max_step;
        }
        if(model->at(real).isDeepWater()){
            return count;
        }
        ++count;
    }
}

float DampHelper::spread(int x, int y)
{
    int count = 0;
    float dd = TwicePI / sample_count;
    float sum = 0.0f;
    for(float dir = 0.0f; dir < TwicePI; dir += dd, ++count){
        sum += decay(stepUp(x, y, dir)) * float(Options::IsWater);
    }
    return sum / count;
}

void DampHelper::solve(int level)
{
    auto width = model->getWidth(), length = model->getLength();
    int len = 2 * level + 1;
    for(int y = level; y < length - level; y+=len)
    {
        for(int x = level; x < width - level; x+=len)
        {
            if(!model->at(y, x).isDeepWater())
            {
                // it->damp = spread(x, y);
                setByWindow(x, y, spread(x, y), level);
            }
        }
    }
#ifdef QT_DEBUG
    qDebug("end");
#endif
}

void DampHelper::setByWindow(int x, int y, float value, int level)
{
    int i_begin = y - level, i_end = y + level + 1;
    int j_begin = x - level, j_end = x + level + 1;
    for(int i = i_begin; i < i_end; ++i)
    {
        for(int j = j_begin; j < j_end; ++j)
        {
            if(!model->at(y, x).isDeepWater())
            model->at(i, j).damp = value;
        }
    }
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
