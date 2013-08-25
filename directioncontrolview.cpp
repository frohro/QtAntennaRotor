#include "directioncontrolview.h"

DirectionControlView::DirectionControlView(QWidget *parent) :
    QGraphicsView(parent)
{
}

void DirectionControlView::mouseDoubleClickEvent(QMouseEvent *ev)
{
    emit directionSelected(ev->x(),ev->y());
    QGraphicsView::mouseDoubleClickEvent(ev);
}
