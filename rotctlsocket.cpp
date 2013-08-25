#include "rotctlsocket.h"
#include <QString>
#include <QRegExp>

RotCtlSocket::RotCtlSocket(QObject *parent) :
    QObject(parent)
{
}

void RotCtlSocket::Connect()
{
    socket = new QTcpSocket(this);

    // connect
    qDebug() << "In Connect routine....";
    socket->connectToHost("192.168.2.237", 4533 );
    if(socket->waitForConnected(3000))
    {
        qDebug() << "Connected!";
    }

    else
    {
        qDebug() << "Not connected!";
    }

    // send


    // receive

    // close
}

void RotCtlSocket::Cleanup()
{
    socket->close();
}

void RotCtlSocket::getPresentBearing()
{
    socket->write("p \n");
    socket->waitForBytesWritten(1000);
    socket->waitForReadyRead(3000);
    qDebug() << "Reading: " << socket->bytesAvailable();
    bearing = socket->readAll();
    bearingList = bearing.split(QRegExp("\\s+"));
    azimuth = bearingList.at(0);
    qDebug() << "azimuth" << azimuth << "azimuth.toFloat()" << azimuth.toFloat();
    azimuthF = azimuth.toFloat();
    if (azimuthF >= 180) azimuthF = azimuthF-360;
    emit bearingReturned(azimuthF);
    qDebug() << bearing;
}

void RotCtlSocket::setBearing(int bearing)
{
    char cmd_str[10];
    qDebug() << "In setBearing(int bearing)";
    int len_of_cmd_str = sprintf(cmd_str,"P %d 0\n",bearing);
    socket->write(cmd_str);
    socket->waitForBytesWritten(1000);
    socket->waitForReadyRead(3000);
    qDebug() << socket->readAll();
}
