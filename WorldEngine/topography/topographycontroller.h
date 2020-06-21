#ifndef __TOPOGRAPHY_TOPOGRAPHYCONTROLLER_H
#define __TOPOGRAPHY_TOPOGRAPHYCONTROLLER_H

#include <random>
#include <cmath>
#include <functional>
#include <utility>
#include <chrono>
#include <type_traits>

#include <QObject>
#include <QTimer>

#include "topography.h"
#include "algorithm/functional.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif
class TopographyController : public QObject
{
    Q_OBJECT
public:
    explicit TopographyController(Topography *_model, int msec = 1000, QObject *parent = nullptr);

signals:
    void inited();

public slots:
    void init();
    void initTemp();
    void initDamp();


    void meteor(int x, int y, double r, std::function<double(double)> mapping, double p);
    void buildRandomMap(std::function<double()>);
    void buildRandomMap(std::function<double(double, double)> = &Shape::mountain);
    void convolution(int level = 1);
    float convolution(int x, int y, int level) const;

private:
    Topography *model;
    QTimer core_timer;

    std::mt19937 generator;

public:
    template <template <typename T>
              class F = std::uniform_int_distribution,
              typename T = int,
              typename Result = typename F<T>::result_type,
              Result (F<T>::*)(decltype(generator) &) = &F<T>::operator()>
    void meteors(
        int n,
        std::pair<float, float> range = {10.0, 80.0},
        double p = 0.5,
        std::function<double(double)> mapping = &Mapping::extreme)
    {
        auto dis = 1.0 / range.first - 1.0 / range.second;

        F<T> random_x{0, model->getWidth() - 1};
        F<T> random_y{0, model->getLength() - 1};
        std::uniform_real_distribution<> random_r{1e-6, 1.0};
        for (int _ = 0; _ < n; ++_)
        {
            auto x = random_x(generator);
            auto y = random_y(generator);
            auto r = 1.0 / (random_r(generator) * dis + 1.0 / range.second);
            this->meteor(x, y, r, mapping, p);
        }
    }

    template <typename T, typename... Args>
    typename std::enable_if<std::is_integral<T>::value>::type
    multiConvolution(T &&arg, Args &&... args)
    {
        convolution(std::forward<T>(arg));
        multiConvolution(std::forward<Args>(args)...);
    }

    template <typename T>
    typename std::enable_if<std::is_integral<T>::value>::type
    multiConvolution(T &&arg)
    {
        convolution(std::forward<T>(arg));
    }

    template <typename... Args>
    void intelligentBuild(int meteoCount, Args &&... args)
    {
        meteors(meteoCount);
        multiConvolution(std::forward<Args>(args)...);
    }

    inline Topography *getModel() const
    {
        return model;
    }
};

#endif // __TOPOGRAPHY_TOPOGRAPHYCONTROLLER_H
