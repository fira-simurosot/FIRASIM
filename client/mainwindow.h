#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QTimer>
#include <QCheckBox>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QtNetwork>
#include "packet.pb.h"
#include "command.pb.h"
#include "replacement.pb.h"
#include "common.pb.h"


class MainWindow : public QDialog
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void reconnectUdp();
    void sendPacket();
    void sendBtnClicked();
    void resetBtnClicked();
    void disconnectUdp();
private:
    bool sending, reseting;
    QUdpSocket udpsocket;
    QHostAddress _addr;
    quint16 _port;
    QLineEdit* edtIp;
    QLineEdit* edtPort;
    QLineEdit* edtId;
    QLineEdit* edtV1, *edtV2;
    QLabel* lblIp;
    QLabel* lblPort;
    QLabel* lblId;
    QComboBox* cmbTeam;
    QLabel* lblV1, *lblV2;
    QTextEdit* txtInfo;
    QPushButton* btnSend;
    QPushButton* btnReset;
    QPushButton* btnConnect;
    QTimer* timer;
};

#endif // MAINWINDOW_H
