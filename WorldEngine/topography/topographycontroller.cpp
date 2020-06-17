#include "topographycontroller.h"
#include <algorithm>
#include <chrono>
#include <cmath>

#include "unit/options.h"

TopographyController::TopographyController(Topography *_model, QObject *parent)
    : QObject(parent),
      model(_model == nullptr ? new Topography{this} : _model),
      generator{std::chrono::system_clock::now().time_since_epoch().count()}
{
}

void TopographyController::buildRandomMap(const std::function<double(double, double)> &strategy)
{
    auto width = model->getWidth(), length = model->getLength();
    auto it = model->begin();

    double halfWidth = width / 2.0,
            halfLength = length / 2.0;
    double maxDis = std::sqrt(halfWidth * halfWidth + halfLength * halfLength);
    double heightDis = static_cast<double>(Options::MaxAltitude);

    std::uniform_real_distribution<> random(0, 1);

    for (int i = 0; i < length; ++i)
    {
        for (int j = 0; j < width; ++j, ++it)
        {
            double idis = i - halfLength + 0.5, jdis = j - halfWidth + 0.5;
            it->setAltitude(
                        random(generator) * heightDis * strategy(std::sqrt(idis * idis + jdis * jdis), maxDis));
        }
    }
}

void TopographyController::buildRandomMap(const std::function<double()> &strategy)
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
}

void TopographyController::meteor(int x, int y, double r, const std::function<double(double)> &mapping, double p)
{
    auto check = [&](int i, int j){ return i >= 0 && i < model->getLength() && j >= 0 && j < model->getWidth(); };

    std::uniform_real_distribution<> random{0, 1};
    double rr = r * p;
    double mid = model->at(y, x).altitude;

    for(int i = y - r; i < y + r + 0.5; ++i)
    {
        for(int j = x - r; j < x + r + 0.5; ++j)
        {
            if(!check(i, j))
                continue;

            double _r = r - mapping(random(generator)) * rr;
            double dis = std::sqrt(std::pow(j - x, 2) + std::pow(i - y, 2));

            if(dis <= _r){
                double reduce = std::sqrt(_r * _r - dis * dis);
                model->at(i, j).altitude = std::max(float(Options::MinAltitude),
                                                    std::min(float(mid - reduce), model->at(i, j).altitude));
            }
        }
    }
}

float TopographyController::convolution(int x, int y, int level) const
{
    auto width = model->getWidth(), length = model->getLength();
    assert(x >= level && y >= level && x < width - level && y < length - level);
    double sum = 0.0;
    for(int i = y - level; i <= y + level; ++i)
    {
        for(int j = x - level; j <= x + level; ++j)
        {
            sum += model->at(i, j).altitude;
        }
    }
    return sum / std::pow(2 * level + 1, 2);
}

void TopographyController::convolution(int level)
{
    auto width = model->getWidth(), length = model->getLength();
    for(int y = level; y < length - level; ++y)
        for(int x = level; x < width - level; ++x)
            model->at(y, x).altitude = convolution(x, y, level);
}
