#ifndef MAPITEM_H
#define MAPITEM_H

#include <QGraphicsItem>

class MapItem : public QGraphicsItem
{
public:
    MapItem();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // MAPITEM_H
