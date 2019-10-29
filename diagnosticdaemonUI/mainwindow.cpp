#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QStandardItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Models
    tableListViewModel_ = new MsgListTableModel();
    ui->msglist->setModel(tableListViewModel_);

    paramTableModel_= new ParamTableModel();
    ui->paramTable->setModel(paramTableModel_);

    //View property
    //ui->msglist->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    //ui->msglist->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->paramTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //UDP channel
    rxudpSocket_=new QUdpSocket(this);
    rxudpSocket_->bind(QHostAddress::LocalHost, rxport_);
    connect(rxudpSocket_,SIGNAL(readyRead()),this,SLOT(readMsg()));
    txudpSocket_=new QUdpSocket(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readMsg()
{
    while (rxudpSocket_->hasPendingDatagrams()) {
        QByteArray buffer;
        buffer.resize(rxudpSocket_->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        rxudpSocket_->readDatagram(buffer.data(), buffer.size(),&sender, &senderPort);

        tableListViewModel_->InserRops(buffer);
    }
}


void MainWindow::on_msglist_clicked(const QModelIndex &index)
{
    paramTableModel_->ShowRop(index);
}

void MainWindow::on_stoptrace_clicked()
{
    std::array<uint8_t, EOMDiagnosticUdpMsg::getSize()> udpMsg;
    EOMDiagnosticUdpMsg msg;
    EOMDiagnosticRopMsg::Info rop;
    rop=EOMDiagnosticRopMsg::Info{(uint16_t)DiagnosticRopCode::stoplog,(uint16_t)DiagnosticRopSeverity::trace,1,1,14,15,16,17,18};
    msg.addRop(rop);
    udpMsg.fill(0);
    msg.createUdpPacketData(udpMsg);

    // Build a packet to send
    QByteArray buffer((char*)udpMsg.data(),EOMDiagnosticUdpMsg::getSize());
    txudpSocket_->writeDatagram(buffer.data(), EOMDiagnosticUdpMsg::getSize(),QHostAddress::LocalHost, txport_);
}

void MainWindow::on_starttrace_clicked()
{
    std::array<uint8_t, EOMDiagnosticUdpMsg::getSize()> udpMsg;
    EOMDiagnosticUdpMsg msg;
    EOMDiagnosticRopMsg::Info rop;
    rop=EOMDiagnosticRopMsg::Info{(uint16_t)DiagnosticRopCode::startlog,(uint16_t)DiagnosticRopSeverity::trace,1,1,14,15,16,17,18};
    msg.addRop(rop);
    udpMsg.fill(0);
    msg.createUdpPacketData(udpMsg);

    // Build a packet to send
    QByteArray buffer((char*)udpMsg.data(),EOMDiagnosticUdpMsg::getSize());
    //txudpSocket_->writeDatagram(buffer.data(), EOMDiagnosticUdpMsg::getSize(),QHostAddress::LocalHost, txport_);

    tableListViewModel_->InserRops(buffer);
}
