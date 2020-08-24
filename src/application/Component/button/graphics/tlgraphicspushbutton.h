#ifndef TLGRAPHICSPUSHBUTTON_H
#define TLGRAPHICSPUSHBUTTON_H

#include "base/graphics/tlgraphicsbasecontrol.h"
#include "base/graphics/tlgraphicsbuttoninterface.h"

class TLGraphicsPushButton : public TLGraphicsBaseControl, public TLGraphicsButtonInterface
{
    Q_OBJECT
public:
    explicit TLGraphicsPushButton(QGraphicsObject *parent = nullptr);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // TLGRAPHICSPUSHBUTTON_H
