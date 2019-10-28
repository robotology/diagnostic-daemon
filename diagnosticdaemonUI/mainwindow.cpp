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
    udpSocket_=new QUdpSocket(this);
    udpSocket_->bind(QHostAddress::LocalHost, 9001);
    connect(udpSocket_,SIGNAL(readyRead()),this,SLOT(readMsg()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readMsg()
{
    while (udpSocket_->hasPendingDatagrams()) {
        QByteArray buffer;
        buffer.resize(udpSocket_->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        udpSocket_->readDatagram(buffer.data(), buffer.size(),&sender, &senderPort);

        tableListViewModel_->InserRops(buffer);
    }
}


void MainWindow::on_msglist_clicked(const QModelIndex &index)
{
    paramTableModel_->ShowRop(index);
}
