#ifndef TOPOGRAPHYVIEW_H
#define TOPOGRAPHYVIEW_H

#include <functional>

#include <QWidget>
#include <QTransform>
#include <QPoint>

#include "unit/optional.h"
#include "topography.h"
#include "topographycontroller.h"
#include "tools/topographymessage.h"

namespace Ui
{
    class TopographyView;
}

class TopographyView : public QWidget
{
    Q_OBJECT

public:
    explicit TopographyView(Topography *model, int enlarge = 1, QWidget *parent = nullptr);
    explicit TopographyView(const TopographyController &tpc, int enlarge = 1, QWidget *parent = nullptr);
    ~TopographyView();

signals:
    void imageMoved();
    void sendAreaMessage(QMouseEvent *event, int x, int y, const Area &msg);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    Ui::TopographyView *ui;
    TopographyMessage *area_message_box;

    Topography *model;
    QTransform base_translator, translator;

    int enlarge;

    QPoint center;
    QPointF base_step;
    Optional<QPoint> active_pos;

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
