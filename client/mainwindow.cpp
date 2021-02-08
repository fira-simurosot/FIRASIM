#include "mainwindow.h"
#include <QGridLayout>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent),
    udpsocket(this)
{
    QGridLayout* layout = new QGridLayout(this);
    edtIp = new QLineEdit("127.0.0.1", this);
    edtPort = new QLineEdit("20011", this);
    edtId = new QLineEdit("0", this);
    edtV1 = new QLineEdit("0", this);
    edtV2 = new QLineEdit("0", this);

    this->setWindowTitle(QString("FIRASim Sample Client - v 1.0"));

    lblIp = new QLabel("Simulator Address", this);
    lblPort = new QLabel("Simulator Port", this);
    lblId = new QLabel("Id", this);
    lblV1 = new QLabel("Wheel Left (rad/s)", this);
    lblV2 = new QLabel("Wheel Right (rad/s)", this);
    cmbTeam = new QComboBox(this);
    cmbTeam->addItem("Yellow");
    cmbTeam->addItem("Blue");
    txtInfo = new QTextEdit(this);
    btnSend = new QPushButton("Send", this);
    btnReset = new QPushButton("Reset", this);
    btnConnect = new QPushButton("Connect", this);
    txtInfo->setReadOnly(true);
    txtInfo->setHtml("This program is part of <b>FIRASim VSS Simulator</b> package.<br />For more information please refer to <a href=\"http://eew.aut.ac.ir/~parsian/grsim/\">https://github.com/fira-simurosot/FIRASim</a><br /><font color=\"gray\">This program is free software under the terms of GNU General Public License Version 3.</font>");
    txtInfo->setFixedHeight(70);
    layout->addWidget(lblIp, 1, 1, 1, 1);layout->addWidget(edtIp, 1, 2, 1, 1);
    layout->addWidget(lblPort, 1, 3, 1, 1);layout->addWidget(edtPort, 1, 4, 1, 1);
    layout->addWidget(lblId, 2, 1, 1, 1);layout->addWidget(edtId, 2, 2, 1, 1);layout->addWidget(cmbTeam, 2, 3, 1, 2);
    layout->addWidget(lblV1, 3, 1, 1, 1);layout->addWidget(edtV1, 3, 2, 1, 1);
    layout->addWidget(lblV2, 3, 3, 1, 1);layout->addWidget(edtV2, 3, 4, 1, 1);
    layout->addWidget(btnConnect, 9, 1, 1, 2);layout->addWidget(btnSend, 9, 3, 1, 1);layout->addWidget(btnReset, 9, 4, 1, 1);
    layout->addWidget(txtInfo, 10, 1, 1, 4);
    timer = new QTimer (this);
    timer->setInterval(20);
    connect(edtIp, SIGNAL(textChanged(QString)), this, SLOT(disconnectUdp()));
    connect(edtPort, SIGNAL(textChanged(QString)), this, SLOT(disconnectUdp()));
    connect(timer, SIGNAL(timeout()), this, SLOT(sendPacket()));
    connect(btnConnect, SIGNAL(clicked()), this, SLOT(reconnectUdp()));
    connect(btnSend, SIGNAL(clicked()), this, SLOT(sendBtnClicked()));
    connect(btnReset, SIGNAL(clicked()), this, SLOT(resetBtnClicked()));
    btnSend->setDisabled(true);
    sending = false;
    reseting = false;
}

MainWindow::~MainWindow()
{

}

void MainWindow::disconnectUdp()
{
    udpsocket.close();
    sending = false;
    btnSend->setText("Send");
    btnSend->setDisabled(true);
}

void MainWindow::sendBtnClicked()
{
    sending = !sending;
    if (!sending)
    {
        timer->stop();
        btnSend->setText("Send");
    }
    else {
        timer->start();
        btnSend->setText("Pause");
    }
}

void MainWindow::resetBtnClicked()
{
    reseting = true;
    edtV1->setText("0");
    edtV2->setText("0");
}


void MainWindow::reconnectUdp()
{
    _addr = QHostAddress(edtIp->text());
    _port = edtPort->text().toUShort();
    btnSend->setDisabled(false);
}

void MainWindow::sendPacket()
{
    if (reseting)
    {
        sendBtnClicked();
        reseting = false;
    }
    fira_message::sim_to_ref::Packet packet;
    bool yellow = false;
    if (cmbTeam->currentText()=="Yellow") yellow = true;
    fira_message::sim_to_ref::Command* command = packet.mutable_cmd()->add_robot_commands();
    
    command->set_yellowteam(yellow);
    command->set_id(edtId->text().toInt());
    command->set_wheel_left(edtV1->text().toDouble());
    command->set_wheel_right(edtV2->text().toDouble());

    QByteArray dgram;
    dgram.resize(packet.ByteSize());
    packet.SerializeToArray(dgram.data(), dgram.size());
    udpsocket.writeDatagram(dgram, _addr, _port);
}
