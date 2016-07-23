#include "rotctlsocket.h"
#include <QString>
#include <QRegExp>

RotCtlSocket::RotCtlSocket(QObject *parent) :
    QObject(parent)
{
    socket = new QTcpSocket(this);
}

void RotCtlSocket::Connect()
{
    qDebug() << "In Connect routine....";
    if(socket->state() == QTcpSocket::ConnectedState)
    {
        socket->disconnectFromHost();
        if(socket->waitForDisconnected(3000))
            qDebug() << "Socket disconnected, before reconnecting.";
        else
            qDebug() << "Socket would not disconnect to reconnect.";
    }
    socket->connectToHost(ipAddress, port );
    if(socket->waitForConnected(3000))
    {
        qDebug() << "Connected!";
        connected = true;
    }
    else
    {
        qDebug() << "Not connected!";
        connected = false;
    }
    emit isConnected(connected);
}

void RotCtlSocket::Cleanup()
{
    socket->close();
}

void RotCtlSocket::getPresentBearing()
{
    qDebug() << "Getting the heading...";
    if(socket->state() == QAbstractSocket::ConnectedState){
        qDebug() << "Still connected...";
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
    else
    {
        qDebug() << "Not connected.";
        emit isConnected(connected);
    }
}

void RotCtlSocket::setBearing(int bearing)
{
    char cmd_str[10];
    qDebug() << "In setBearing(int bearing)";
    if(sprintf(cmd_str,"P %d 0\n",bearing)<0)
        qDebug() << "Problem getting bearing!";
    if(socket->state() == QAbstractSocket::ConnectedState){
        socket->write(cmd_str);
        socket->waitForBytesWritten(1000);
        socket->waitForReadyRead(3000);
        qDebug() << socket->readAll();
    }
    else
    {
        qDebug() << "Socket not connected!  Cannot set bearing.";
    }
}

void RotCtlSocket:: setIPAddress(QString ipAddressString)
{
    qDebug() << "port in setIPAddress is: " << port;
    if(bool goodAddress = ipAddress.setAddress(ipAddressString)&&(port != 65535)) // To account for first time when port not set.
    {
        qDebug() << "IP Address or Hostname Seems okay!";
        qDebug() << "In RotCtlSocket::setIPAddress(QString), IPAddress is:" << getIPAddress();
        Connect();
    }
//    else if(!goodAddress)  // This doesn't seem to work right.  Above it passes, but here it doesn't.
//        qDebug() << "Problem with IP Address!\n";
}

void RotCtlSocket:: setPort(int portChange)
{
    if((portChange < 65536)&&(portChange > 0))
    {
        qDebug() << "Port number Seems okay!";
        port = portChange;
        Connect();
    }
    else
        qDebug() << "Problem with port number!\n";
}

QString RotCtlSocket::getIPAddress()
{
    return ipAddress.toString();
}

int RotCtlSocket::getPort()
{
    return port;
}
