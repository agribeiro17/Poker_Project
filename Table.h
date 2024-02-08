#ifndef TABLE_H
#define TABLE_H

#include <QString>
#include "cards.h"
#include "players.h"
#include "deck.h"
#include <QVector>

class table_t {
public:

    std::random_device rd;
    uint32_t ID;
    QString password;
    QVector<cards_t*> cards_on_table;
    int total_chips;
    QVector<Client_t*> player_list;
    //ADD int N players
    deck_t* deck;

    table_t(QString pass);
    ~table_t();
    void shuffle();
    bool addPlayer(Client_t *player);
    void giveCards();
    void drawCard(int i=1);
    QStringList str_cards_on_table();
    QVector<QTcpSocket *> socksFromPl();
    bool checkAllReady();
    void setReady(int id);
    Client_t *giveClwithID(int id);
    int giveIdxwithID(int id);
    Client_t *giveClwithNick(QString nick);
    int lastminbet = 0;
    int minbet = 0;
    int lastminbetidx = 9;
    QVector<int> IdxtoID(QVector<int> idxs);
    void distDraw(QVector<int> ids);
    int nickCounter(QString nick);
    void removeCl(int id);
    int nactivePl();
    QVector<Client_t*> activePls();
};

#endif//TABLE_H
