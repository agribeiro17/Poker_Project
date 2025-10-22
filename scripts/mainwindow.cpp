#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "gamewindow.h"
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    QPixmap bkgnd(":/img/fundo2.1.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palette);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_playButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_optionsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_creditsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}


void MainWindow::on_exitButton_clicked()
{
    exit(EXIT_FAILURE);
}


void MainWindow::on_back_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_back_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_back_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_exit_button_clicked()
{
    exit(EXIT_FAILURE);
}


void MainWindow::on_signIn_clicked()
{
    MyName = ui->username_edit->text();
    if(MyName == "" || MyName.contains("_"))
    {
        QMessageBox::critical(this, "Username not set", "Please insert a valid username. Cannot have '_'.");
    } else
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
}

void MainWindow::on_privateButton_clicked()
{
    //CHANGEEEEEEEEE
    ui->stackedWidget->setCurrentIndex(5);

}


void MainWindow::on_hostButton_clicked()
{
    QString gameID = whatismyip();
    mGameWindow = new GameWindow(this, gameID, 0);
    hide();
    mGameWindow->show();
    lpbGameWindow = new GameWindow(this, gameID, 1, this->MyName, "127.0.0.1", 7204);
    lpbGameWindow->show();
}


void MainWindow::on_back_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_btnConnect_clicked()
{
    auto ip = ui->lnIP->text();
    mGameWindow = new GameWindow(this, ip, 1, this->MyName, ip, 7204);
    hide();
    mGameWindow->show();
}


void MainWindow::on_lnIP_textChanged(const QString &arg1)
{
    QHostAddress address(arg1);
    QString state = "0";
    if(QAbstractSocket::IPv4Protocol == address.protocol()){
        state = "2";
    } else{

    }
    ui->lnIP->setProperty("state", state);
    style()->polish(ui->lnIP);
}

QString MainWindow::whatismyip()
{
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost){
            qDebug() << address.toString();
            return address.toString();
        }
    }
    return nullptr;
}

