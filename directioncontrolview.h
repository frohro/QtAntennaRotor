#ifndef DIRECTIONCONTROLVIEW_H
#define DIRECTIONCONTROLVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QEvent>
#include <QDebug>

class QObject;

class DirectionControlView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit DirectionControlView(QWidget *parent = NULL);

    void mouseDoubleClickEvent(QMouseEvent *ev = NULL);

signals:
    void directionSelected(int x, int y);

public slots:
    
};

#endif // DIRECTIONCONTROLVIEW_H
