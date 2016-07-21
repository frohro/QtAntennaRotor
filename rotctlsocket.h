#ifndef ROTCTLSOCKET_H
#define ROTCTLSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QLineEdit>
#include <QAbstractSocket>
#include <QHostAddress>

class RotCtlSocket : public QObject
{
    Q_OBJECT
public:
    explicit RotCtlSocket(QObject *parent = 0);
    void Cleanup();
    bool connected;

signals:
    void bearingReturned(int);
    void isConnected(bool connected);
    
public slots:
    void getPresentBearing();
    void Connect();
    void setBearing(int bearing);
    void setIPAddress(QString ipAddress);

private:
    QTcpSocket *socket;
    QString bearing;
    QString azimuth;
    QStringList bearingList;
    float azimuthF;
    QHostAddress ipAddress;
    int port;

};

#endif // ROTCTLSOCKET_H
