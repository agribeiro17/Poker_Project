#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStyle>
#include "gamewindow.h"
#include <QNetworkInterface>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString MyName = "";

private slots:
    void on_playButton_clicked();

    void on_optionsButton_clicked();

    void on_creditsButton_clicked();

    void on_exitButton_clicked();

    void on_back_clicked();

    void on_back_2_clicked();

    void on_back_3_clicked();

    void on_exit_button_clicked();

    void on_signIn_clicked();

    void on_privateButton_clicked();

    void on_hostButton_clicked();

    void on_back_4_clicked();

    void on_btnConnect_clicked();

    void on_lnIP_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    GameWindow *mGameWindow;
    GameWindow *lpbGameWindow;
    QString whatismyip();
};
#endif // MAINWINDOW_H
