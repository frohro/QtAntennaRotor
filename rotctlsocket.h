#ifndef ROTCTLSOCKET_H
#define ROTCTLSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QString>
#include <QStringList>

class RotCtlSocket : public QObject
{
    Q_OBJECT
public:
    explicit RotCtlSocket(QObject *parent = 0);
    void Cleanup();

signals:
    void bearingReturned(int);
    
public slots:
   // void moveToBearing(int);
    void getPresentBearing();
    void Connect();
    void setBearing(int bearing);

private:
    QTcpSocket *socket;
    QString bearing;
    QString azimuth;
    QStringList bearingList;
    float azimuthF;
    
};

#endif // ROTCTLSOCKET_H
