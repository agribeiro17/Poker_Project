#ifndef SERVER_CONTROLLER_H
#define SERVER_CONTROLLER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QByteArray>
#include <QStringList>

class Server_Controller : public QObject
{
    Q_OBJECT
public:
    explicit Server_Controller(QObject *parent = nullptr);
    void sendMessageAll(QString msg);                //Sends a Message to all clients connected
    void sendMessage(QString msg, QTcpSocket *sock); //Sends a Message to a specific player (maybe change architecture
                                                     //                          to send player and make this private)
    void retransmit(QString msg, int id); //send to everyone, except id, msg sent from id
    void extracted(QStringList &cards, QString &msg);
    void sendCardDrawn(QStringList cards);
    void sendWinner(int id);
    void sendDraw(QVector<int> ids);
    void sendDefeat(int id);
    
    signals:
    void bufferChanged(QString message, int id);
    void newPlayer(QTcpSocket *np);

private slots:
    void newConnection();
    void readMessage();             //Reads message from client and puts it in a msg buffer (needs to be redesigned)
    //Currently missing slot to deal with disconnects from clients

private: //Attributes

public:  //Attributes
    QObject *wiam;
    QList<QTcpSocket*> socket_list;
    QTcpServer* server;

private: //Methods
    void add_client(QTcpSocket *sock);

};

#endif // SERVER_CONTROLLER_H
