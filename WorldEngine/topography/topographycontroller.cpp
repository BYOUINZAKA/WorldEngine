#include "topographycontroller.h"
#include <algorithm>
#include <chrono>
#include <cmath>

#include <unit/options.h>

TopographyController::TopographyController(Topography *_model, QObject *parent)
    : QObject(parent),
      model(_model == nullptr ? new Topography{this} : _model),
      generator{std::chrono::system_clock::now().time_since_epoch().count()}
{
}


void TopographyController::buildRandomMap(const std::function<double(double, double)>& strategy)
{
    auto width = model->getWidth(), length = model->getLength();
    auto it = model->begin();

    double halfWidth = width / 2.0, halfLength = length / 2.0;
    double maxDis = std::sqrt(halfWidth * halfWidth) + std::sqrt(halfLength * halfLength);
    double heightDis = static_cast<double>(Options::MaxAltitude);

    std::uniform_real_distribution<> random(0, 1);

    for (int i = 0; i < length; ++i)
    {
        for (int j = 0; j < width; ++j, ++it)
        {
            double idis = i - halfLength, jdis = j - halfWidth;
            it->altitude = random(generator)
                           * heightDis
                           * strategy(std::sqrt(idis * idis) + std::sqrt(jdis * jdis), maxDis);
        }
    }
}
