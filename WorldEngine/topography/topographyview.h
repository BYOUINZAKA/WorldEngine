#ifndef __TOPOGRAPHY_TOPOGRAPHYVIEW_H
#define __TOPOGRAPHY_TOPOGRAPHYVIEW_H

#include <functional>

#include <QPoint>
#include <QTransform>
#include <QWidget>

#include "unit/optional.h"
#include "unit/area.h"

class Topography;
class TopographyMessage;
class TopographyController;

namespace Ui {
class TopographyView;
}

class TopographyView : public QWidget {
    Q_OBJECT

public:
    enum { kMaxEnlarge = 10, kMinEnlarge = 1, kEnlargeStep = 1 };

public:
    explicit TopographyView(Topography* model, int enlarge = 1, QWidget* parent = nullptr);
    explicit TopographyView(TopographyController* tpc, int enlarge = 1,
                            QWidget* parent = nullptr);
    ~TopographyView();

signals:
    void imageMoved();
    // The interactive interface with the TopographyMessage.
    void sendAreaMessage(QMouseEvent* event, int x, int y, const Area& msg);

protected:
    void paintEvent(QPaintEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    Ui::TopographyView* ui;
    TopographyMessage* area_message_box;
    Topography* model;

    QPoint center;
    QTransform translator;

    // The magnification.
    int32_t enlarge;

    // This is a vector that converts real matrix to map matrix.
    QTransform base_translator;

    // Packaging an optional pos, It will save a global pos when the user presses the left mouse button.
    Optional<QPoint> active_pos;

private:
    template <typename... Args>
    constexpr
        typename std::enable_if<(sizeof...(Args) == 6) || (sizeof...(Args) == 9), QTransform>::type
        setTranslatorByMulti(const QTransform& base, Args&&... args) {
        return base * QTransform{static_cast<qreal>(std::forward<Args>(args))...};
    }
};

#endif  // __TOPOGRAPHY_TOPOGRAPHYVIEW_H
