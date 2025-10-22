#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QDialog>
#include <QStyle>
#include <QStringList>
#include <QLabel>
#include "Network_Controller.h"
#include "Server_Controller.h"
#include "Table.h"
#include "players.h"
#include "WinningHand.h"
#include "machstate.h"
#include <QTimer>
#include <QMessageBox>

namespace Ui {
class GameWindow;
}

class GameWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent, QString gID, int mode, QString name = "", QString ip = "0", int port = 0);
    ~GameWindow();
    void sendMainW(QWidget *mainw);
    void hostDeal();
    void updateNicks();
    void updateChips();
    void updateTH();
    void updateMyH();
    void showBallon(int id);
    void hideBallon();
    void antebet();
    void giveCardsforPls(int i);
    QString gameID;
    QTimer *timeSM;
    QTimer *tm;

private slots:
    void on_btnClose_clicked();
    void readToDebugClient(QString msg);
    void postMsgBuffer(QString message, int id);
    void on_btnDeal_clicked();
    void addPlayersToTable(QTcpSocket *np);
    void updateSM();
    void cleanupT();

    void on_btnFold_clicked();

    void on_btnRaise_clicked();

    void on_btnCheck_clicked();

    void on_btnCall_clicked();

private:
    Ui::GameWindow *ui;
    QWidget *prevw;
    Network_Controller *netctrl;
    Server_Controller *svctrl;
    table_t *table;
    //Players_t *player;
    Host_t *meH;
    Client_t *meC;
    bool imh = false;
    QString MyName;
    QVector<QLabel*> Pl_chips;
    QVector<QLabel*> Pl_nicks;
    QVector<QLabel*> Pl_state;
    QVector<QLabel*> lnc;
    QVector<QLabel*> msgpops;
    QVector<QFrame*> pops;
    int antevalue;
    Whoplay *smwp;
    Bet *smb;
    bool firstround;
    bool mip = false;
};

#endif // GAMEWINDOW_H
