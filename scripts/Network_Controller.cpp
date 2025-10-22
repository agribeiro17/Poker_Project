#include "Network_Controller.h"

Network_Controller::Network_Controller(QObject *parent)
    : QObject{parent}
{

}

bool Network_Controller::connectToDevice(QString ip, int port){
    this->ip = ip;
    this->port = port;
    socket.connectToHost(QHostAddress(ip),port);
    socket.open(QIODevice::ReadWrite);
    if(socket.isOpen()) {
        connect(&socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
        return true;
    } else{
        return false;
    }
}

qint64 Network_Controller::sendMessage(QString msg){

    if(socket.isOpen()){
        msg = msg + "?";
        QByteArray data = msg.toUtf8();
        qint64 n = socket.write(data);
        qDebug() << data;
        qDebug() << n;
        return n;
    } else{
        return -1;
    }
}

void Network_Controller::readMessage(){
    if(socket.isOpen()){
        QByteArray data_from_socket = socket.readAll();
        qDebug() << data_from_socket;
        QString msg = QString::fromStdString(data_from_socket.toStdString());
        qDebug() << msg;

        emit msgToInter(msg);
    } else{
        qDebug() << "Error reading";
    }
}

bool Network_Controller::sendBet(int amnt, int mlb)
{
    if(amnt<1){
        return false;
    }
    QString msg = "BET-" + QString::number(amnt) + "-" + QString::number(mlb);
    if(sendMessage(msg)){
        return true;
    } else{
        return false;
    }
}

bool Network_Controller::sendCall(int inc)
{
    QString msg = "CALL-" + QString::number(inc);
    if(sendMessage(msg)){
        return true;
    } else{
        return false;
    }
}

bool Network_Controller::sendFold()
{
    QString msg = "FOLD";
    if(sendMessage(msg)){
        return true;
    } else{
        return false;
    }
}

int Network_Controller::getPort()
{
    return port;
}

QString Network_Controller::getIP()
{
    return ip;
}

QTcpSocket* Network_Controller::getSocket()
{
    return &socket;
}
