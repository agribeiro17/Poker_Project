#include "Table.h"
#include <algorithm>

table_t::table_t(QString pass) {
    ID = 0;
    password = pass;
    total_chips = 0;
    deck = new deck_t();
}

table_t::~table_t()
{
    delete deck;
    while(this->player_list.length() > 0){
        this->player_list.pop_front();
    }
    while(this->cards_on_table.length() > 0){
        this->cards_on_table.pop_front();
    }
}

void table_t::shuffle() {
    std::shuffle(this->player_list.begin(), this->player_list.end(), rd);
}

bool table_t::addPlayer(Client_t *player)
{
    if(player_list.length() > 7){
        return false;
    } else{
        player_list.append(player);
        return true;
    }
}

void table_t::giveCards()
{
    int i=0;
    foreach (Client_t *pl, this->player_list) {
        qDebug() << i;
        pl->hand.append(this->deck->cards_on_deck.at(i));
        pl->hand.append(this->deck->cards_on_deck.at(i+1));
        this->deck->cards_on_deck.pop_front();
        this->deck->cards_on_deck.pop_front();
        //Test
        qDebug() << "Player hand :" + pl->giveHandStr();
    }
}

void table_t::drawCard(int i)
{
    for(int j=0; j<i; j++){
        cards_on_table.append(this->deck->cards_on_deck.at(0));
        this->deck->cards_on_deck.pop_front();
    }
}

QStringList table_t::str_cards_on_table()
{
    QStringList cl;
    foreach(cards_t *card, this->cards_on_table){
        QString c = card->naipe + QString::number(card->num);
        cl.append(c);
    }
    return cl;
}

QVector<QTcpSocket*> table_t::socksFromPl()
{
    QVector<QTcpSocket*> vsock;
    foreach(Client_t *pl, player_list){
        vsock.append(pl->sock);
    }
    return vsock;
}

bool table_t::checkAllReady()
{
    if(this->player_list.length() > 1){
        foreach (Client_t *pl, this->player_list) {
            if(pl->status == "UNREADY"){
                return false;
            }
        }
        return true;
    }
    return false;
}

void table_t::setReady(int id)
{
    foreach (Client_t *pl, this->player_list) {
        if(pl->sock->socketDescriptor() == id){
            pl->status = "READY";
        }
    }
}

Client_t *table_t::giveClwithID(int id)
{
    foreach (Client_t *pl, this->player_list) {
        if(pl->ID == id){
            return pl;
        }
    }
    return nullptr;
}

int table_t::giveIdxwithID(int id)
{
    int i=0;
    if(id >= 0){
        foreach (Client_t *pl, this->player_list) {
            if(pl->ID == id){
                return i;
            }
            i++;
        }
    }
    return -1;
}

Client_t *table_t::giveClwithNick(QString nick)
{
    foreach (Client_t *pl, this->player_list) {
        if(pl->nickname == nick){
            return pl;
        }
    }
    return nullptr;
}

QVector<int> table_t::IdxtoID(QVector<int> idxs)
{
    QVector<int> ids;
    for(int i = 0; i < idxs.length(); i++){
        int j = idxs.at(i);
        ids.push_front(this->player_list[j]->ID);
    }
    return ids;
}

void table_t::distDraw(QVector<int> ids)
{
    Client_t *pl;
    int chd = this->total_chips / ids.length();
    qDebug() << "I'm here";
    qDebug() << ids;
    foreach(int id, ids){
        pl = this->player_list[id];
        pl->chips += chd;
    }
    qDebug() << "I am dumb";
    this->total_chips = 0;
}

int table_t::nickCounter(QString nick)
{
    int i = 0;
    QString n = nick + "_";
    foreach(Client_t *pl, this->player_list){
        if(pl->nickname.startsWith(n) || pl->nickname == nick){
            i++;
        }
    }
    return i;
}

void table_t::removeCl(int id)
{
    int i = giveIdxwithID(id);
    this->player_list.removeAt(i);
}

int table_t::nactivePl()
{
    int i = 0;

    foreach(Client_t *pl, this->player_list){
        if(pl->loopstatus != "imout"){
            i++;
        }
    }

    return i;
}

QVector<Client_t *> table_t::activePls()
{
    QVector<Client_t*> apls;

    foreach(Client_t *pl, this->player_list){
        if(pl->loopstatus != "imout"){
            apls.append(pl);
        }
    }

    return apls;
}






