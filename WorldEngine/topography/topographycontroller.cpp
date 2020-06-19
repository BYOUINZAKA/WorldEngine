#include "topographycontroller.h"
#include <algorithm>
#include <chrono>
#include <cmath>

#include <QThread>

#include "unit/options.h"

TopographyController::TopographyController(Topography *_model, QObject *parent)
    : QObject{parent},
      model{_model == nullptr ? new Topography{this} : _model},
      generator{static_cast<uint32_t>(std::chrono::system_clock::now().time_since_epoch().count())}
{
    connect(this, &TopographyController::inited, model, &Topography::refreshed);
}

void TopographyController::buildRandomMap(std::function<double(double, double)> strategy)
{
    auto width = model->getWidth(), length = model->getLength();
    auto it = model->begin();

    double half_width = width / 2.0,
           half_length = length / 2.0;
    double maxDis = std::sqrt(half_width * half_width + half_length * half_length);
    double height_dis = static_cast<double>(Options::MaxAltitude);

    std::uniform_real_distribution<> random(0, 1);

    for (int i = 0; i < length; ++i)
    {
        for (int j = 0; j < width; ++j, ++it)
        {
            double idis = i - half_length + 0.5, jdis = j - half_width + 0.5;
            it->setAltitude(
                random(generator) * height_dis * strategy(std::sqrt(idis * idis + jdis * jdis), maxDis));
        }
    }

    emit model->refreshed();
}

void TopographyController::buildRandomMap(std::function<double()> strategy)
{
    auto width = model->getWidth(), length = model->getLength();
    auto it = model->begin();

    std::uniform_real_distribution<> random(0, static_cast<double>(Options::MaxAltitude));

    for (int i = 0; i < length; ++i)
    {
        for (int j = 0; j < width; ++j, ++it)
        {
            it->setAltitude(random(generator) * strategy());
        }
    }

    emit model->refreshed();
}

void TopographyController::meteor(int x, int y, double r, std::function<double(double)> mapping, double p)
{
    std::uniform_real_distribution<> random{0, 1};
    double rr = r * p;
    double mid = model->at(y, x).altitude;

    int i_begin = std::max(0.0, y - r), i_end = std::min((double)model->getLength(), y + r + 0.5);
    int j_begin = std::max(0.0, x - r), j_end = std::min((double)model->getWidth(), x + r + 0.5);

    for (int i = i_begin; i < i_end; ++i)
    {
        for (int j = j_begin; j < j_end; ++j)
        {
            double _r = r - mapping(random(generator)) * rr;
            double dis = std::sqrt(std::pow(j - x, 2) + std::pow(i - y, 2));

            if (dis <= _r)
            {
                double reduce = std::sqrt(_r * _r - dis * dis);
                model->at(i, j).altitude = std::max(float(Options::MinAltitude),
                                                    std::min(float(mid - reduce), model->at(i, j).altitude));
            }
        }
    }

    emit model->refreshed();
}

float TopographyController::convolution(int x, int y, int level) const
{
    auto width = model->getWidth(), length = model->getLength();
    assert(x >= level && y >= level && x < width - level && y < length - level);
    double sum = 0.0;
    for (int i = y - level; i <= y + level; ++i)
    {
        for (int j = x - level; j <= x + level; ++j)
        {
            sum += model->at(i, j).altitude;
        }
    }
    emit model->refreshed();

    return sum / std::pow(2 * level + 1, 2);
}

void TopographyController::convolution(int level)
{
    auto width = model->getWidth(), length = model->getLength();
    for (int y = level; y < length - level; ++y)
        for (int x = level; x < width - level; ++x)
            model->at(y, x).altitude = convolution(x, y, level);
}

void TopographyController::init()
{
    buildRandomMap(&Shape::basin);
    meteors(25, {80, 200}, 0.5, &Mapping::extreme<2>);
    multiConvolution(1, 2);
    meteors(500, {10, 80}, 0.5, &Mapping::normal);
    multiConvolution(1, 2, 3);
    emit inited();
}
