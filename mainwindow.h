#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QPoint>
#include <QGraphicsItem>
#include <QtAlgorithms>

#include "rotctlsocket.h"
#include "directioncontrolview.h"
#include <math.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    RotCtlSocket myRotCtlSocket;
    QTimer *rotationEstimateTimer;
signals:
    void setBearing(int);

public slots:
    void getNewDirection(int x, int y);
    void updateProgressBar();
    void updatePointingBearing(int bearing);

protected:
    QGraphicsScene *directionScene;
    int radius; // of QGraphicsView
    //QLineF directionLine;

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
