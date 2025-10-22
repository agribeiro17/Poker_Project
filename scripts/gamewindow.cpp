#include "gamewindow.h"
#include "machstate.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent, QString gID, int mode, QString name, QString ip, int port) :
    QDialog(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    gameID = gID;
    //BACKGROUND
    QPixmap bkgnd(":/img/fundo2.1.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palette);
    ui->room_id->setText("Room ID: " + this->gameID);
    style()->polish(ui->room_id);
    ui->room_id->repaint();

    this->MyName = name;
    prevw = parent;
    table = new table_t("");
    this->antevalue = 10;

    //vector of nick labels
    Pl_nicks.append(ui->NameP2);
    Pl_nicks.append(ui->NameP3);
    Pl_nicks.append(ui->NameP4);
    Pl_nicks.append(ui->NameP5);
    Pl_nicks.append(ui->NameP6);
    Pl_nicks.append(ui->NameP7);
    Pl_nicks.append(ui->NameP8);

    //vector of pl states
    Pl_state.append(ui->Player2);
    Pl_state.append(ui->Player3);
    Pl_state.append(ui->Player4);
    Pl_state.append(ui->Player5);
    Pl_state.append(ui->Player6);
    Pl_state.append(ui->Player7);
    Pl_state.append(ui->Player8);

    //vector of pl chips
    Pl_chips.append(ui->P1chips);
    Pl_chips.append(ui->P2chips);
    Pl_chips.append(ui->P3chips);
    Pl_chips.append(ui->P4chips);
    Pl_chips.append(ui->P5chips);
    Pl_chips.append(ui->P6chips);
    Pl_chips.append(ui->P7chips);
    Pl_chips.append(ui->P8chips);

    //vector of table cards
    lnc.append(ui->tableC1);
    lnc.append(ui->tableC2);
    lnc.append(ui->tableC3);
    lnc.append(ui->tableC4);
    lnc.append(ui->tableC5);

    //vector of pop-up messages for players decision
    msgpops.append(ui->msg2);
    msgpops.append(ui->msg3);
    msgpops.append(ui->msg4);
    msgpops.append(ui->msg5);
    msgpops.append(ui->msg6);
    msgpops.append(ui->msg7);
    msgpops.append(ui->msg8);

    //vector of pop-ups
    pops.append(ui->balloonmsg2);
    pops.append(ui->balloonmsg3);
    pops.append(ui->balloonmsg4);
    pops.append(ui->balloonmsg5);
    pops.append(ui->balloonmsg6);
    pops.append(ui->balloonmsg7);
    pops.append(ui->balloonmsg8);


    //Open game dependent on mode
    switch (mode) {
    case 0: //host case
        imh = true;
        svctrl = new Server_Controller(this);
        connect(svctrl, &Server_Controller::bufferChanged, this, &GameWindow::postMsgBuffer);
        connect(svctrl, &Server_Controller::newPlayer, this, &GameWindow::addPlayersToTable);      //THIS IS THE WAY KEEP EVOLVING ON THIS
        ui->debugtxt->appendPlainText("Server Started");
        meH = new Host_t(svctrl);
        break;

    case 1: //private case
        netctrl = new Network_Controller();
        meC = new Client_t();
        meC->nickname = this->MyName;
        table->addPlayer(meC);
        if(netctrl->connectToDevice(ip, port)){
            ui->NameP1->setText(this->MyName);
            ui->debugtxt->appendPlainText("Connected to server.");
            connect(netctrl, &Network_Controller::msgToInter, this, &GameWindow::readToDebugClient); //change signal when game UI fully ready
            meC->setNet(netctrl);
            meC->sendMyName();
            updateChips();
        } else {
            ui->debugtxt->appendPlainText("Failed to connect to server.");
        }
        break;

    case 2:
        ui->debugtxt->appendPlainText("This feature is not ready.");    //public rooms not available for now (maybe not be available at all)
        break;

    default:
        break;
    }
}

GameWindow::~GameWindow()
{
    delete table;
    if(imh){
        delete meH;
        delete svctrl;
    }else{
        delete meC;
        delete netctrl;
    }

    delete ui;
}

void GameWindow::hostDeal()
{
    if(imh){
        smwp = new Whoplay(table);
        smb = new Bet(table, smwp);
    }
    table->deck->shuffle();
    table->giveCards();
    //table->drawCard(0);
    //svctrl->sendCardDrawn(table->str_cards_on_table());
    foreach (Players_t *pl, table->player_list) {
        QString msg = "HAND-" + pl->giveHandStr();
        svctrl->sendMessage(msg, pl->sock);
    }
    antebet();
    firstround = true;
    smwp->updwhoplays();
    int k = smwp->WcurrentState;
    meH->sendYourTime(table->player_list[k]);
    /*timeSM = new QTimer(this);
    connect(timeSM, SIGNAL(timeout()), this, SLOT(updateSM()));
    timeSM->start(500);*/
}

void GameWindow::on_btnClose_clicked()
{
    if(imh){
        foreach(QTcpSocket *sock, this->svctrl->socket_list){
            sock->close();
        }
        this->svctrl->server->close();
    }
    prevw->show();
    close();
    delete this;
}

void GameWindow::antebet()
{
    foreach(Client_t *pl, table->player_list){
        if(pl != nullptr){
            if(pl->chips < 10){
                //int k = table->giveIdxwithID(pl->ID);
                pl->loopstatus = "imout";
            } else{
                pl->chips -= antevalue;
                this->table->total_chips += antevalue;
            }
        }
    }
}

void GameWindow::readToDebugClient(QString msg)     // OUR MSG INTERPRETER CLIENT
{
    QStringList lm = msg.split(u'?');
    for(const QString &message : lm){
        if(message == ""){
            //ui->debugtxt->appendPlainText("Nothing to read");

        } else{
            if(message.startsWith("HAND")){
                auto h = message.split(u'-');
                h.pop_front();
                for(const QString &c : h){
                    this->meC->hand.append(new cards_t(c.mid(1, c.size()-1).toInt(), c.at(0)));
                }
                updateMyH();
                antebet();
                updateChips();
            } else if(message.startsWith("TH")){
                QStringList dc = message.split(u'-');
                dc.pop_front();
                this->table->cards_on_table.clear();
                for(const QString &c : dc){
                    this->table->cards_on_table.append(new cards_t(c.mid(1, c.size()-1).toInt(), c.at(0)));
                }
                table->lastminbet = 0;
                table->minbet = 0;
                table->lastminbetidx = 9;
                meC->mylastbet = 0;
                updateTH();
            } else if(message.startsWith("NICK")){
                QStringList k = message.split(u'-');
                int id = k[1].toInt();
                QString nick = k[2];
                qDebug() << id;
                if(k[2] != meC->nickname){
                    Client_t *pl = table->giveClwithID(id);
                    if(pl != nullptr){
                        pl->setNick(nick);
                        updateNicks();
                        updateChips();
                    }
                }

            } else if(message.startsWith("NEWPLAYER")){
                QStringList k = message.split(u'-');
                int id = k[1].toInt();
                Client_t *pl = new Client_t();
                pl->ID = id;
                table->addPlayer(pl);
                meC->sendMyName();

            } else if(message.startsWith("FOLD")){
                QStringList k = message.split(u'-');
                Client_t *pl = table->giveClwithNick(k[1]);
                pl->status = "Fold";
                this->hideBallon();
                this->showBallon(pl->ID);

            } else if(message.startsWith("WINNER")){
                mip = false;
                QStringList k = message.split(u'-');
                Client_t *pl = table->giveClwithID(k[1].toInt());
                if(pl == nullptr){
                    pl = meC;
                }
                pl->status = "I won!";
                pl->chips += table->total_chips;
                table->total_chips = 0;
                this->hideBallon();
                if(pl->nickname != meC->nickname){
                    this->showBallon(pl->ID);
                } else {
                    ui->debugtxt->appendPlainText("I won");
                }
                updateChips();
                tm = new QTimer(this);
                connect(tm, SIGNAL(timeout()), this, SLOT(cleanupT()));
                tm->start(4000);

            } else if(message.startsWith("DRAW")){
                mip = false;
                QStringList k = message.split(u'-');
                k.pop_front();
                qDebug() << k;
                QVector<Client_t*> dpl;
                int chd = table->total_chips / k.length();
                foreach(QString id, k){
                    dpl.push_front(table->giveClwithID(id.toInt()));
                }
                this->hideBallon();
                foreach(Client_t *pl, dpl){
                    pl->chips += chd;
                    pl->status = "I drawed";
                }
                updateChips();
                ui->debugtxt->appendPlainText("The Game ended in a Draw");

            } else if(message.startsWith("BET")){
                QStringList k = message.split(u'-');
                Client_t *pl = table->giveClwithID(k[3].toInt());
                pl->status = "Raise";
                pl->loopstatus = "done";
                pl->chips -= (k[1].toInt() - k[2].toInt());
                this->table->total_chips += (k[1].toInt() - k[2].toInt());
                this->hideBallon();
                this->showBallon(pl->ID);
                this->table->minbet = k[1].toInt();
                ui->debugtxt->appendPlainText(pl->nickname + " raised with " + k[1] + " chips");
                updateChips();

            } else if(message.startsWith("CHECK")){
                QStringList k = message.split(u'-');
                Client_t *pl = table->giveClwithID(k[1].toInt());
                pl->status = "Check";
                pl->loopstatus = "done";
                this->hideBallon();
                this->showBallon(pl->ID);
                ui->debugtxt->appendPlainText(pl->nickname + " checked");

            } else if(message.startsWith("CALL")){
                QStringList k = message.split(u'-');
                Client_t *pl = table->giveClwithID(k[2].toInt());
                int inc = k[1].toInt();
                pl->status = "Call";
                pl->loopstatus = "done";
                pl->chips -= inc;
                this->table->total_chips += inc;
                qDebug() << this->table->minbet;
                this->hideBallon();
                this->showBallon(pl->ID);
                ui->debugtxt->appendPlainText(pl->nickname + " called");
                updateChips();

            } else if(message.startsWith("YTTP")){
                //Could add animation to represent my time to play
                this->mip = true;
            } else if(message.startsWith("SLAYED")){
                QStringList k = message.split(u'-');
                Client_t *pl = table->giveClwithID(k[1].toInt());
                int v = table->giveIdxwithID(pl->ID);
                table->removeCl(pl->ID);
                this->updateNicks();
            }
        }
    }
}

void GameWindow::postMsgBuffer(QString message, int id)     //OUR MSG INTERPRETER SERVER
{
    QStringList msgp = message.split("?");
    qDebug() << msgp;
    Client_t *cl = table->giveClwithID(id);
    if(msgp[1] == "NICK"){
        //int r = table->nickCounter(msgp[2]);
        /*if(r > 0){
            table->removeCl(id);
            svctrl->sendMessage("RPTD", cl->sock);

        } else{
            cl->nickname = msgp[2];
            QString msg = "NICK-" + msgp[0] + "-" + cl->nickname;
            svctrl->retransmit(msg, msgp[0].toInt());
        }*/
        cl->nickname = msgp[2];
        QString msg = "NICK-" + msgp[0] + "-" + cl->nickname;
        svctrl->retransmit(msg, msgp[0].toInt());
    }
    if(message.endsWith("READY?")){
        table->setReady(id);
        if(table->checkAllReady()){
            hostDeal();
        }
    }
    if(msgp[1].startsWith("FOLD")){
        auto m = msgp[1].split(u'-');
        Client_t *pl = table->giveClwithNick(m[1]);
        pl->status = "Fold";
        pl->loopstatus = "imout";
        this->svctrl->retransmit(msgp[1], msgp[0].toInt());
        int n = table->nactivePl();
        if(n == 1){
            QVector<Client_t*> apls = table->activePls();
            svctrl->sendWinner(apls[0]->ID);
            timeSM = new QTimer(this);
            connect(timeSM, SIGNAL(timeout()), this, SLOT(updateSM()));
            timeSM->start(7000);
        }
        smwp->updwhoplays();
        int k = smwp->WcurrentState;
        meH->sendYourTime(table->player_list[k]);
    }
    if(msgp[1].startsWith("BET")){
        auto m = msgp[1].split(u'-');
        Client_t *pl = table->giveClwithID(msgp[0].toInt());
        int h = table->giveIdxwithID(pl->ID);
        if(smwp->WcurrentState == h && m[1].toInt() > table->minbet ){
            pl->status = "raise";
            pl->loopstatus = "done";
            pl->chips -= m[1].toInt();
            qDebug() << "o h é " + QString::number(h);
            this->table->lastminbetidx = h;
            this->table->total_chips += m[1].toInt();
            QString rm = msgp[1] + "-" + msgp[0];
            this->svctrl->retransmit(rm, msgp[0].toInt());
        }
        smwp->updwhoplays();
        int k = smwp->WcurrentState;
        meH->sendYourTime(table->player_list[k]);

    }
    if(msgp[1].startsWith("CHECK")){
        Client_t *pl = table->giveClwithID(msgp[0].toInt());
        int h = table->giveIdxwithID(pl->ID);
        qDebug() << "Idx " + QString::number(h);
        qDebug() << "state " + QString::number(smwp->WcurrentState);

        if(h == smwp->WcurrentState && table->minbet == 0){
            pl->status = "Check";
            pl->loopstatus = "done";
            QString rm = "CHECK-" + msgp[0];
            this->svctrl->retransmit(rm, msgp[0].toInt());

            smwp->updwhoplays();
            qDebug() << smwp->WcurrentState;
            qDebug() << table->lastminbetidx;
            //FIRST play ON TABLE
            if(smwp->WcurrentState == table->lastminbetidx){
                if(firstround){
                    giveCardsforPls(3);
                    firstround = false;
                    //not sure
                    smwp->WcurrentState = Player1;
                    table->lastminbet = 0;
                    table->minbet = 0;
                    table->lastminbetidx = 9;
                    smwp->fp = true;
                } else{
                    if(table->cards_on_table.length() == 5){
                        WinningHand *wh = new WinningHand();
                        wh->table = table;
                        std::vector<int> svi = wh->determineWinningHand();
                        QVector<int> i = QVector<int>(svi.begin(), svi.end());
                        for(int g = 0; g < i.length(); g++){
                            i[g] = i[g] - 1;
                        }
                        if(i.length() < 2){
                            svctrl->sendWinner(table->player_list[i.first()]->ID);
                            timeSM = new QTimer(this);
                            connect(timeSM, SIGNAL(timeout()), this, SLOT(updateSM()));
                            timeSM->start(7000);
                        } else{
                            this->table->distDraw(i);
                            QVector<int> idd = this->table->IdxtoID(i);
                            this->svctrl->sendDraw(idd);
                            qDebug() << idd;
                            timeSM = new QTimer(this);
                            connect(timeSM, SIGNAL(timeout()), this, SLOT(updateSM()));
                            timeSM->start(7000);
                        }
                    } else{
                        giveCardsforPls(1);
                        //not sure
                        smwp->WcurrentState = Player1;
                        table->lastminbet = 0;
                        table->minbet = 0;
                        table->lastminbetidx = 9;
                        smwp->fp = true;
                    }
                }
            }
            int k = smwp->WcurrentState;
            meH->sendYourTime(table->player_list[k]);
        }
    }
    if(msgp[1].startsWith("CALL")){
        Client_t *pl = table->giveClwithID(msgp[0].toInt());
        int h = table->giveIdxwithID(pl->ID);
        qDebug() << "Idx " + QString::number(h);
        qDebug() << "state " + QString::number(smwp->WcurrentState);

        if(h == smwp->WcurrentState){
            pl->status = "Call";
            pl->loopstatus = "done";
            QStringList cal = msgp[1].split(u'-');
            QString rm = "CALL-" + cal[1] + "-" + msgp[0];
            this->svctrl->retransmit(rm, msgp[0].toInt());

            smwp->updwhoplays();
            qDebug() << smwp->WcurrentState;
            qDebug() << table->lastminbetidx;
            if(smwp->WcurrentState == table->lastminbetidx){
                if(firstround){
                    giveCardsforPls(3);
                    firstround = false;
                    //not sure
                    smwp->WcurrentState = Player1;
                    table->lastminbet = 0;
                    table->minbet = 0;
                    table->lastminbetidx = 9;
                    smwp->fp = true;
                } else{
                    if(table->cards_on_table.length() == 5){
                        WinningHand *wh = new WinningHand();
                        wh->table = table;
                        std::vector<int> svi = wh->determineWinningHand();
                        QVector<int> i = QVector<int>(svi.begin(), svi.end());
                        for(int g = 0; g < i.length(); g++){
                            i[g] = i[g] - 1;
                        }
                        if(i.length() < 2){
                            svctrl->sendWinner(table->player_list[i.first()]->ID);
                            timeSM = new QTimer(this);
                            connect(timeSM, SIGNAL(timeout()), this, SLOT(updateSM()));
                            timeSM->start(7000);
                        } else{
                            this->table->distDraw(i);
                            QVector<int> idd = this->table->IdxtoID(i);
                            qDebug() << idd;
                            this->svctrl->sendDraw(idd);
                            timeSM = new QTimer(this);
                            connect(timeSM, SIGNAL(timeout()), this, SLOT(updateSM()));
                            timeSM->start(7000);
                            //svctrl->sendMessageAll("DRAW");
                        }
                    } else{
                        giveCardsforPls(1);
                        //not sure
                        smwp->WcurrentState = Player1;
                        table->lastminbet = 0;
                        table->minbet = 0;
                        table->lastminbetidx = 9;
                        smwp->fp = true;
                    }
                }
            }
            int k = smwp->WcurrentState;
            meH->sendYourTime(table->player_list[k]);
        }
    }
}

void GameWindow::addPlayersToTable(QTcpSocket *np)
{
    qDebug() << "Adding a player";
    Client_t *cl = new Client_t();
    cl->sock = np;
    cl->ID = np->socketDescriptor();
    QString msg = "NEWPLAYER-" + QString::number(np->socketDescriptor());
    QString msgtonp;
    QVector<QTcpSocket*> vsock;
    vsock = table->socksFromPl();
    foreach (QTcpSocket *sock, vsock) {
        svctrl->sendMessage(msg, sock);
        msgtonp = "NEWPLAYER-" + QString::number(sock->socketDescriptor());
        svctrl->sendMessage(msgtonp, np);
    }
    msgtonp = "YOUARE-" + QString::number(np->socketDescriptor());
    svctrl->sendMessage(msgtonp, np);
    table->addPlayer(cl);
}

void GameWindow::updateSM() //Clean server
{
    foreach(Client_t *pl, table->player_list){
        pl->status = "";
        pl->loopstatus = "";
        pl->hand.pop_front();
        pl->hand.pop_front();
        //pl->hand.empty();
    }
    while(table->cards_on_table.length() > 0){
        table->cards_on_table.pop_front();
    }
    while(table->deck->cards_on_deck.length() > 0){
        table->deck->cards_on_deck.pop_front();
    }
    table->deck->drawc();
    //delete table->deck;
    //table->deck = new deck_t();
    //table->deck->shuffle();
    table->deck->shuffle();
    //qDebug() << table->deck->cards_on_deck;
    smwp = new Whoplay(table);
    smb = new Bet(table, smwp);
    table->giveCards();
    foreach (Players_t *pl, table->player_list) {
        QString msg = "HAND-" + pl->giveHandStr();
        svctrl->sendMessage(msg, pl->sock);
    }
    antebet();
    firstround = true;
    smwp->updwhoplays();
    int k = smwp->WcurrentState;
    meH->sendYourTime(table->player_list[k]);
    timeSM->stop();

}

void GameWindow::cleanupT() //Clean client
{
    this->hideBallon();
    mip = false;
    meC->mylastbet = 0;
    table->lastminbet = 0;
    table->minbet = 0;
    table->lastminbetidx = 9;
    meC->hand.pop_front();
    meC->hand.pop_front();
    ui->lnC1->setProperty("state", "back");
    ui->lnC2->setProperty("state", "back");
    style()->polish(ui->lnC1);
    style()->polish(ui->lnC2);
    ui->lnC1->repaint();
    ui->lnC2->repaint();
    for(int g = 0; g < 5; g++){
        lnc[g]->setProperty("state", "back");
        style()->polish(lnc[g]);
        this->lnc[g]->repaint();
    }
    tm->stop();
}

void GameWindow::updateNicks()
{
    int i=0;
    foreach(Client_t *pl, table->player_list){
        if(pl->ID != meC->ID){
            this->Pl_nicks[i]->setText(pl->nickname);
            this->Pl_state[i]->setProperty("state", "on");
            style()->polish(this->Pl_state[i]);
            this->Pl_state[i]->repaint();
            i++;
        }
    }
}

void GameWindow::updateChips()
{
    int i=0;
    foreach(Client_t *pl, table->player_list){
        this->Pl_chips[i]->setText(QString::number(pl->chips));
        style()->polish(this->Pl_chips[i]);
        this->Pl_chips[i]->repaint();
        i++;
    }
    this->ui->label_3->setText(QString::number(table->total_chips));
    style()->polish(this->ui->label_3);
    this->ui->label_3->repaint();
}

void GameWindow::updateTH()
{
    int g=0;
    foreach(cards_t *c, table->cards_on_table){
        qDebug() << c->card_str();
        lnc[g]->setProperty("state", c->card_str());
        style()->polish(lnc[g]);
        this->lnc[g]->repaint();
        g++;
    }
}

void GameWindow::updateMyH()
{
    QString h = meC->giveHandStr();
    auto sh = h.split(u'-');
    ui->lnC1->setProperty("state", sh[0]);
    ui->lnC2->setProperty("state", sh[1]);
    style()->polish(ui->lnC1);
    style()->polish(ui->lnC2);
    ui->lnC1->repaint();
    ui->lnC2->repaint();
}

void GameWindow::showBallon(int id)
{
    int i = this->table->giveIdxwithID(id) - 1; //As me is pos 0, all players are ofset -1 in labels
    if(i > -1){
        Client_t *pl = this->table->giveClwithID(id);
        this->pops[i]->setProperty("state", "on");
        style()->polish(pops[i]);
        this->pops[i]->repaint();
        this->msgpops[i]->setText(pl->status);
        style()->polish(msgpops[i]);
        this->msgpops[i]->repaint();
    }
}

void GameWindow::hideBallon()
{
    foreach (QFrame *ballon, this->pops){
        ballon->setProperty("state", "off");
        style()->polish(ballon);
        ballon->repaint();
    }
    foreach(QLabel *pop, this->msgpops){
        pop->setText("");
        style()->polish(pop);
        pop->repaint();
    }
}

void GameWindow::on_btnDeal_clicked()
{
    if(imh){
        table->drawCard(5);
        svctrl->sendCardDrawn(table->str_cards_on_table());
        WinningHand *wh = new WinningHand();
        wh->table = table;
        std::vector<int> svi = wh->determineWinningHand();
        QVector<int> i = QVector<int>(svi.begin(), svi.end());
        if(i.length() < 2){
            svctrl->sendWinner(table->player_list[i.first()-1]->ID);
        } else{
            this->table->distDraw(i);
            this->svctrl->sendDraw(i);
            //svctrl->sendMessageAll("DRAW");
        }
    } else{
        netctrl->sendMessage("READY");
    }
}

void GameWindow::giveCardsforPls(int i)
{
    table->drawCard(i);
    svctrl->sendCardDrawn(table->str_cards_on_table());
}

void GameWindow::on_btnFold_clicked()
{
    //ADD STATE MACHINE RULES
    if(!imh){
        if(mip){
            if(this->meC->fold()){
                hideBallon();
                ui->debugtxt->appendPlainText("I folded.");
                mip = false;
            }
        }
    }
}


void GameWindow::on_btnRaise_clicked()
{
    //ADD STATE MACHINE RULES
    int amnt = ui->lineEdit->text().toInt();
    if(!imh){
        if(mip && amnt > table->minbet){
            if(this->meC->bet(amnt)){
                hideBallon();
                this->table->total_chips += (amnt-meC->mylastbet);
                this->table->minbet = amnt;
                ui->debugtxt->appendPlainText("I betted " + QString::number(amnt) + ".");
                updateChips();
                mip = false;
                meC->mylastbet = amnt;
            } else{
                ui->debugtxt->appendPlainText("Error. Please insert valid amount.");
            }
        }
    }
}


void GameWindow::on_btnCheck_clicked()
{
    if(!imh){
        if(mip && table->minbet == 0){
            if(this->meC->check()){
                hideBallon();
                ui->debugtxt->appendPlainText("I checked.");
                mip = false;
            } else{
                ui->debugtxt->appendPlainText("Error.");
            }
        }
    }
}


void GameWindow::on_btnCall_clicked()
{
    if(!imh){
        if(mip){
            if(this->meC->call(table->minbet - meC->mylastbet)){
                hideBallon();
                this->table->total_chips += (table->minbet - meC->mylastbet);
                ui->debugtxt->appendPlainText("I called.");
                mip = false;
                updateChips();
            } else{
                ui->debugtxt->appendPlainText("Error.");
            }
        }
    }
}

