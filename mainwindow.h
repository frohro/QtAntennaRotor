#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QPoint>
#include <QGraphicsItem>
#include <QtAlgorithms>
#include <QSettings>
#include <QApplication>

#include "rotctlsocket.h"
#include "directioncontrolview.h"
#include  "ip_dialog.h"
#include "ui_ip_dialog.h"
#include <math.h>
#include <QSignalSpy>
#include <assert.h>

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
    IP_Dialog *ipDialog;

signals:
    void setBearing(int);
    void newIPAddress(QString);

public slots:
    void getNewDirection(int x, int y);
    void updateProgressBar();
    void updatePointingBearing(int bearing);
    void getIPAddress();
    void updateIPAddress(QString ipAddressString);
    void updatePort(int port);
    void showConnectedButton(bool connected);

protected:
    QGraphicsScene *directionScene;
    int radius; // of QGraphicsView

private slots:
    void on_connectButton_clicked();

    void on_actionHost_Address_triggered();

    void on_actionCompass_triggered();

    void on_actionKL7NA_Great_Circle_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    QString ourSettingsFile;
    void loadSettings();
    void saveSettings();
    QString backgroundImageResource;
};

#endif // MAINWINDOW_H
