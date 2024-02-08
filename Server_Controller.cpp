#include "Server_Controller.h"

Server_Controller::Server_Controller(QObject *parent)
    : QObject{parent}
{
    wiam = parent;
    server = new QTcpServer();
    if(server->listen(QHostAddress::Any, 7204)){
        connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
        qDebug() << "Server started";
    } else{
        qDebug() << "Server start failed";
    }

}

void Server_Controller::sendMessageAll(QString msg)
{
    msg = msg + "?";
    foreach(QTcpSocket *sock, socket_list){
        QByteArray data = msg.toUtf8();
        qint64 n = sock->write(data);
        qDebug() << n;
        qDebug() << data;
    }
}

void Server_Controller::sendMessage(QString msg, QTcpSocket *sock)
{
    msg = msg + "?";
    QByteArray data = msg.toUtf8();
    qint64 n = sock->write(data);
    qDebug() << n;
    qDebug() << data;
}

void Server_Controller::retransmit(QString msg, int id)
{
    msg = msg + "?";
    qDebug() << msg;
    foreach(QTcpSocket *sock, socket_list){
        if(sock->socketDescriptor() != id){
            QByteArray data = msg.toUtf8();
            qint64 n = sock->write(data);
            qDebug() << data;
        }
    }
}

void Server_Controller::sendCardDrawn(QStringList cards)
{
    QString msg = "TH";
    for (QString c : cards) {
        msg = msg + "-" + c;
    }
    sendMessageAll(msg);
}

void Server_Controller::sendWinner(int id)
{
    QString msg = "WINNER-" + QString::number(id);
    sendMessageAll(msg);
}

void Server_Controller::newConnection()
{
    while(server->hasPendingConnections()){
        add_client(server->nextPendingConnection());
    }
}

void Server_Controller::readMessage()
{
    QTcpSocket *sock = reinterpret_cast<QTcpSocket*>(sender());

    QByteArray data_client = sock->readAll();
    QString msg_client = QString::fromStdString(data_client.toStdString());
    QString msg_to_buffer = QString::number(sock->socketDescriptor()) + "?" + msg_client;
    qDebug() << msg_to_buffer;
    int id = (int) sock->socketDescriptor();
    //needs to be changed signal&slot to read from buffer (or not use buffer at all and make message interpretation on the spot (to be determined when game logic advances))
    emit bufferChanged(msg_to_buffer, id);
}

void Server_Controller::add_client(QTcpSocket *sock)
{
    socket_list.append(sock);
    qDebug() << "Client connected : " + QString::number(sock->socketDescriptor());
    connect(sock, SIGNAL(readyRead()), this, SLOT(readMessage()));

    emit newPlayer(sock);
}

void Server_Controller::sendDraw(QVector<int> ids)
{
    QString msg = "DRAW";
    foreach(int id, ids){
        msg += "-" + QString::number(id);
    }
    sendMessageAll(msg);
}

void Server_Controller::sendDefeat(int id)
{
    QString msg = "SLAYED-" + QString::number(id);
    sendMessageAll(msg);
}
