#ifndef TOPOGRAPHYVIEW_H
#define TOPOGRAPHYVIEW_H

#include <functional>

#include <QWidget>
#include <QTransform>
#include <QPoint>

#include "unit/optional.h"
#include "topography.h"
#include "topographycontroller.h"

namespace Ui
{
    class TopographyView;
}

class TopographyView : public QWidget
{
    Q_OBJECT

public:
    explicit TopographyView(Topography *model, QWidget *parent = 0);
    explicit TopographyView(const TopographyController &tpc, QWidget *parent = 0);
    ~TopographyView();

signals:
    void imageMoved();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    Ui::TopographyView *ui;
    Topography *model;
    QTransform base_translator, translator;
    std::function<QPoint(QPoint)> pos_translator;

    int enlarge;
    QPoint center;

public:
    enum
    {
        kMaxEnlarge = 10,
        kMinEnlarge = 1,
        kEnlargeStep = 1
    };

    template <typename... Enable>
    constexpr
        typename std::enable_if<
            sizeof...(Enable) == 6,
            QTransform>::type &&
        setTranslatorByMulti(const QTransform &base, Enable &&... enables)
    {
        return std::move(base * QTransform{static_cast<qreal>(std::forward<Enable>(enables))...});
    }
};

#endif // TOPOGRAPHYVIEW_H
