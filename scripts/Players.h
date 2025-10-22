#ifndef PLAYERS_H
#define PLAYERS_H

#include <QString>
#include <QVector>
#include "cards.h"
#include "Server_Controller.h"
#include "Network_Controller.h"

using namespace std;


class Players_t {
	public:
        QString nickname;
		int chips;
        QVector<cards_t*> hand;
        QTcpSocket *sock;
        QString status; //deve ser alterado consoante a maquina de estados que vai controlar o jogo
        QString loopstatus; //deve ser alterado consoante a maquina de estados que vai controlar o jogo
        QString giveHandStr();

};

class Host_t : public Players_t {
    public:
        Host_t(Server_Controller *svctrl);
        void sendYourTime(Players_t *pl);

        Server_Controller *svctrl;

};

class Client_t : public Players_t {
    public:
        Client_t();

        long long ID;
        Network_Controller *netctrl;
        QString net_status; //deve ser alterado consoante a maquina de estados que vai controlar conectividade
        void setNet(Network_Controller *netctrl);
        bool bet(int amnt);
        bool sendMyName();
        bool setNick(QString nick);
        bool fold();
        bool check();
        bool call(int i);
        int mylastbet = 0;
};

#endif//PLAYERS_H
