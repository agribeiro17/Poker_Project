#include <QtTest>

// add necessary includes here
#include <QString>
#include "../../Table.h"
#include "../../Network_Controller.h"
#include "../../Server_Controller.h"
#include "qtestcase.h"


class PokerPro : public QObject
{
    Q_OBJECT

public:
    PokerPro();
    ~PokerPro();
    Server_Controller *server;
    Network_Controller *client;
    Network_Controller *hostAsClient;
    QString GetRandomString();
    QStringList msgClient;
    QStringList msgHost;
    void EmuMainWindow();
    table_t *tableT;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void check_table_passwd();
    void check_deck_creation();
    void check_TCPconnection();
    void sendBetClient();
    void sendMsgClient();
    void readMsgClient();
    void readMsgServer();
    void betFair();
    void betUnfair();
    void betRandom();
    void callFair();
    void callUnfair();
    void callAtRandom();
    void callNotAllowed();
    void checkFair();
    void checkUnfair();
    void checkNotAllowed();
    void hostBecomesClientToo();
    void add1Player();
    void add10Player();

private slots:
    void postMsgBuffer(QString message);
    void readToDebugClient(QString msg);
};


void PokerPro::postMsgBuffer(QString message)
{
    msgHost.append(message);
}

void PokerPro::readToDebugClient(QString msg)
{
    msgClient.append(msg);
}

void PokerPro::EmuMainWindow()
{
    tableT = new table_t("");
    hostAsClient = new Network_Controller();
    Client_t *playerT = new Client_t();
    if(hostAsClient->connectToDevice("127.0.0.1", 8080)){
        playerT->setNet(hostAsClient);
        tableT->addPlayer(playerT);
    }

}


QString PokerPro::GetRandomString()
{
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    const int randomStringLength = QRandomGenerator::global()->generate() % 10000; // assuming you want random strings of 12 characters

    QString randomString;
    for(int i=0; i<randomStringLength; ++i)
    {
        int index = QRandomGenerator::global()->generate() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    return randomString;
}

PokerPro::PokerPro()
{

}

PokerPro::~PokerPro()
{

}

void PokerPro::initTestCase()
{
    server = new Server_Controller(nullptr);
    connect(server, &Server_Controller::bufferChanged, this, &PokerPro::postMsgBuffer);
    client = new Network_Controller();
    connect(client, &Network_Controller::msgToInter, this, &PokerPro::readToDebugClient);
}

void PokerPro::cleanupTestCase()
{

}

void PokerPro::check_table_passwd()
{
    table_t table("");
    QCOMPARE(table.password, "");
}

void PokerPro::check_deck_creation()
{
    QString test_card;
    deck_t deck;
    int g=0;
    QVector<QString> naipes{"H", "C", "D", "S"};
    for (int j = 0; j < 4; j++) {
        for (int i = 1; i < 14; i++) {
            test_card = naipes[j] + QString::number(i);
            QCOMPARE(deck.cards_on_deck[g++]->card_str(), test_card);
        }
    }
}

void PokerPro::check_TCPconnection()
{
    bool con = client->connectToDevice("127.0.0.1", 8080);
    QCOMPARE(con, true);
}

void PokerPro::sendBetClient()
{
    for(int i=0; i<10; i++){
        bool c = client->sendBet(i*72120+72);
        QCOMPARE(c, true);
    }
    bool nc = client->sendBet(-100);
    QCOMPARE(nc, false);
}

void PokerPro::sendMsgClient()
{
    QString msg;
    for (int i = 0; i<100;i++){
        msg = GetRandomString();
        long long k = msg.length();
        long long check = client->sendMessage(msg);
        QCOMPARE(check, k);
    }

}

void PokerPro::readMsgClient()
{
    server->sendMessage("Hello World", client->getSocket());
    QTest::qWait(1000);
    QString t;
    if(msgClient.isEmpty()){
        t = "";
    } else {
        t = msgClient.last();
    }
    QCOMPARE(t, "Hello World?");
}

void PokerPro::readMsgServer()
{
    client->sendMessage("Hello World");
    QTest::qWait(1000);
    QString t;
    if(msgHost.isEmpty()){
        t = "";
    } else {
        t = msgHost.last();
    }
    QCOMPARE(t, "Hello World?");
}

void PokerPro::betFair()
{
    Client_t player;
    player.setNet(client);
    QCOMPARE(player.bet(100), true);
}

void PokerPro::betUnfair()
{
    Client_t player;
    player.setNet(client);
    QCOMPARE(player.bet(100000), false);
}

void PokerPro::betRandom()
{
    Client_t player;
    player.setNet(client);
    int rn;
    int totalC = 10000;
    bool test, expect;
    for (int i = 0; i<100; i++){
        rn = QRandomGenerator::global()->generate() % 1000;
        test = player.bet(rn);
        totalC -= rn;
        qDebug() << "Chips real : " + QString::number(totalC);
        qDebug() << "Chips reported : " + QString::number(player.chips);
        if(totalC < 0){
            expect = false;
        } else{
            expect = true;
        }
        QCOMPARE(test, expect);
    }
}

void PokerPro::callFair()
{

}

void PokerPro::callUnfair()
{

}

void PokerPro::callAtRandom()
{

}

void PokerPro::callNotAllowed()
{

}

void PokerPro::checkFair()
{

}

void PokerPro::checkUnfair()
{

}

void PokerPro::checkNotAllowed()
{

}

void PokerPro::hostBecomesClientToo()
{
    EmuMainWindow();
    QCOMPARE(tableT->player_list.length(), 1);
}

void PokerPro::add1Player()
{
    table_t table("");
    Client_t pl1;
    table.addPlayer(&pl1);
    QCOMPARE(table.player_list.length(), 1);
}

void PokerPro::add10Player()
{
    table_t table("");
    Client_t *pl;
    bool t;
    for(int i=0; i<10; i++){
        pl = new Client_t();
        if(i > 7){
            t = false;
        } else{
            t = true;
        }

        QCOMPARE(table.addPlayer(pl), t);
    }

    QCOMPARE(table.player_list.length(), 8);
}



QTEST_APPLESS_MAIN(PokerPro)

#include "tst_pokerpro.moc"
