#ifndef __TOPOGRAPHY_TOPOGRAPHYCONTROLLER_H
#define __TOPOGRAPHY_TOPOGRAPHYCONTROLLER_H

#include <random>
#include <chrono>
#include <functional>
#include <QObject>

#include "topography.h"

class TopographyController : public QObject
{
    Q_OBJECT
public:
    explicit TopographyController(Topography *_model = nullptr, QObject *parent = nullptr);
    void buildRandom(std::function<double(double, double)> strategy =
                         [](double d, double m) { return (m - d) / m; });

    inline Topography *getModel() const
    {
        return model;
    }

signals:

public slots:

private:
    Topography *model;
    std::mt19937 generator;
};

#endif // __TOPOGRAPHY_TOPOGRAPHYCONTROLLER_H
