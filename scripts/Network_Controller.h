#ifndef NETWORK_CONTROLLER_H
#define NETWORK_CONTROLLER_H

//#include "Players.h"
#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QList>
#include <QString>
#include <QByteArray>

//This is a network controller for a specific machine with IP same as this->ip
//So, this works like a interface between me (my machine that is running this app) and the person whose machine has the IP this->ip

/*
    To the game each player will have it's Network Controller so if I call player->Network_Controller->connectToDevice()
    it will open a TCP connection to that player
*/

// It should be wise to add signals but I am too lazy to do it, for now I will do it by command

class Network_Controller : public QObject
{
    Q_OBJECT
public:
    explicit Network_Controller(QObject *parent = nullptr);
    bool connectToDevice(QString ip, int port); //connect this machine to machine with "ip"
    qint64 sendMessage(QString msg); //Send message
    QTcpSocket* getSocket();       //return Socket
    bool sendBet(int amnt, int mlb);        //Send Bet message with betted amount = amnt
    bool sendCall(int inc);               //Send Call message
    bool sendFold();               //Send Fold message
    int getPort();                 //return Port
    QString getIP();               //return IP

private slots:
    void readMessage();         //read message

signals:
    void msgToInter(QString message);

private:
    QTcpSocket socket;
    QString ip;
    int port;
};

#endif // NETWORK_CONTROLLER_H
