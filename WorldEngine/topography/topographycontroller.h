#ifndef __TOPOGRAPHY_TOPOGRAPHYCONTROLLER_H
#define __TOPOGRAPHY_TOPOGRAPHYCONTROLLER_H

#include <random>
#include <functional>
#include <QObject>

#include "topography.h"

class TopographyController : public QObject
{
    Q_OBJECT
public:
    explicit TopographyController(Topography *_model = nullptr, QObject *parent = nullptr);

    inline Topography *getModel() const
    {
        return model;
    }

signals:

public slots:

private:
    Topography *model;
    std::mt19937 generator;

public:
    void buildRandomMap(const std::function<double(double, double)>& = [](double d, double m) { return (m - d) / m; });
};

#endif // __TOPOGRAPHY_TOPOGRAPHYCONTROLLER_H
