#include "Players.h"

Host_t::Host_t(Server_Controller *svctrl)
{
    this->svctrl = svctrl;
    this->chips = 10000;
}

void Host_t::sendYourTime(Players_t *pl)
{
    QString msg = "YTTP";
    this->svctrl->sendMessage(msg, pl->sock);
}

Client_t::Client_t()
{
    this->chips = 10000;
    this->status = "UNREADY";
}

void Client_t::setNet(Network_Controller *netctrl)
{
    this->netctrl = netctrl;
    this->ID = QString::number(netctrl->getSocket()->socketDescriptor()).toInt();
    //this->sock = netctrl->getSocket();
}

bool Client_t::bet(int amnt)
{
    if(this->chips >= amnt)
    {
        this->chips = this->chips - (amnt - mylastbet);
        return this->netctrl->sendBet(amnt, mylastbet);
    } else{
        return false;
    }

}

bool Client_t::sendMyName()
{
    QString msg = "NICK?" + this->nickname;
    if(netctrl->sendMessage(msg) > 0){
        return 1;
    }
    return 0;
}

bool Client_t::setNick(QString nick)
{
    QString n = nick;
    this->nickname = n;
    return 1;
}

bool Client_t::fold()
{
    QString msg = "FOLD-" + this->nickname;
    if(netctrl->sendMessage(msg) > 0){
        return 1;
    }
    return 0;
}

bool Client_t::check()
{
    QString msg = "CHECK";
    if(netctrl->sendMessage(msg) > 0){
        return 1;
    }
    return 0;
}

bool Client_t::call(int i)
{
    if(this->chips >= i)
    {
        this->chips -= i;
        return netctrl->sendCall(i);
    } else{
        return false;
    }
}

QString Players_t::giveHandStr()
{
    QString h = this->hand.at(0)->naipe + QString::number(this->hand.at(0)->num) + "-" + this->hand.at(1)->naipe + QString::number(this->hand.at(1)->num);
    return h;
}
