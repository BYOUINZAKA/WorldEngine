#include "topographycontroller.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iterator>

#include <QThread>

#include "algorithm/damphelper.h"
#include "algorithm/functional.h"
#include "topography.h"
#include "unit/options.h"

using namespace std;

TopographyController::TopographyController(Topography* _model, int msec, QObject* parent)
    : QObject{parent},
      model{_model},
      generator{static_cast<uint32_t>(chrono::system_clock::now().time_since_epoch().count())},
      core_timer{_model} {
    connect(&core_timer, &QTimer::timeout, model, &Topography::refreshed);
    connect(this, &TopographyController::inited, model, &Topography::refreshed);

    core_timer.setInterval(msec);
    core_timer.start();
}

void TopographyController::buildRandomMap(function<double(double, double)> strategy) {
    const int width = model->getWidth(), length = model->getLength();
    auto it = model->begin();

    const double half_width = width / 2.0, half_length = length / 2.0;
    const double maxDis = sqrt(half_width * half_width + half_length * half_length);
    const double height_dis = static_cast<double>(Options::MaxAltitude);

    uniform_real_distribution<> random(0, 1);

    for (int i = 0; i < length; ++i) {
        for (int j = 0; j < width; ++j, ++it) {
            double idis = i - half_length + 0.5, jdis = j - half_width + 0.5;
            it->setAltitude(random(generator) * height_dis *
                            strategy(sqrt(idis * idis + jdis * jdis), maxDis));
        }
    }
}

void TopographyController::buildRandomMap(function<double()> strategy) {
    auto width = model->getWidth(), length = model->getLength();
    auto it = model->begin();

    uniform_real_distribution<> random(0, static_cast<double>(Options::MaxAltitude));

    for (int i = 0; i < length; ++i) {
        for (int j = 0; j < width; ++j, ++it) {
            it->setAltitude(random(generator) * strategy());
        }
    }
}

void TopographyController::meteor(int x, int y, double r, function<double(double)> mapping,
                                  double p) {
    const double rr = r * p;
    const double mid = model->at(y, x).altitude;

    const int i_begin = max(0.0, y - r), i_end = min((double)model->getLength(), y + r + 0.5);
    const int j_begin = max(0.0, x - r), j_end = min((double)model->getWidth(), x + r + 0.5);

    uniform_real_distribution<> random{0, 1};

    for (auto i = i_begin; i < i_end; ++i) {
        for (auto j = j_begin; j < j_end; ++j) {
            double _r = r - mapping(random(generator)) * rr;
            double dis = sqrt(pow(j - x, 2) + pow(i - y, 2));

            if (dis <= _r) {
                double reduce = sqrt(_r * _r - dis * dis);
                model->at(i, j).altitude = max(float(Options::MinAltitude),
                                               min(float(mid - reduce), model->at(i, j).altitude));
            }
        }
    }
}

float TopographyController::convolution(int x, int y, int level) const {
    const int i_begin = y - level, i_end = y + level + 1;
    const int j_begin = x - level, j_end = x + level + 1;

    double sum = 0.0;

    for (int i = i_begin; i < i_end; ++i) {
        for (int j = j_begin; j < j_end; ++j) {
            sum += model->accepted(i, j) ? model->at(i, j).altitude : 0.0;
        }
    }
    return sum / pow(2 * level + 1, 2);
}

void TopographyController::convolution(int level) {
    const auto width = model->getWidth(), length = model->getLength();
    for (int y = 0; y < length; ++y) {
        for (int x = 0; x < width; ++x) {
            model->at(y, x).altitude = convolution(x, y, level);
        }
    }
}

void TopographyController::initTemp() {
    for (auto&& e : *model) e.defaultTemp();
}

void TopographyController::initDamp() {
    const auto is_water = static_cast<float>(Options::IsWater);
    for (auto&& e : *model) {
        if (e.isDeepWater()) e.damp = is_water;
    }
#ifdef QT_DEBUG
    DampHelper damp_helper{model, 16, 20.0f};
#else
    DampHelper damp_helper{model, 32, 20.0f};
#endif
    damp_helper.solve(&Decay::solid_inverse);
}

void TopographyController::init() {
    buildRandomMap(&Shape::basin);
    meteors(20, {100, 250}, 0.8, &Mapping::normal);
    multiConvolution(1, 2, 3);
    meteors(300, {50, 150}, 0.8, &Mapping::normal);
    multiConvolution(1, 2, 3, 4, 5);

    initTemp();
    initDamp();

    emit inited();
}
