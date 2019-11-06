#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QStandardItem>
#include "pugixml.hpp"
#include "settingstablemodel.h"

namespace uiconfsintax
{
    static constexpr char configurationfile[]{"./uiconfig.xml"}; 
    static constexpr char udp[]{"//udp"};
};

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

    settingsTableModel_= new SettingsTableModel();
    ui->settings->setModel(settingsTableModel_);

    longdescriptionModel_= new LongdescriptionModel();
    ui->longdescription->setModel(longdescriptionModel_);

    //View property
    //ui->msglist->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    //ui->msglist->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->paramTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->settings->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //Settings
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(uiconfsintax::configurationfile);
    if(result.status == pugi::status_file_not_found)
    {
        std::cout<<"ERROR: config.xml not found"<<std::endl;
        return;
    }
    if(result.status != pugi::status_ok)
    {
        std::cout<<"ERROR: config.xml reading"<<std::endl;
        return;
    }

    pugi::xpath_node udpconf = doc.select_node(uiconfsintax::udp);   
    pugi::xml_node node=udpconf.node();
    rxport_=node.attribute("rxport").as_int();    
    txport_=node.attribute("txport").as_int();
    address_=node.attribute("address").value();

    //UDP channel
    rxudpSocket_=new QUdpSocket(this);
    rxudpSocket_->bind(QHostAddress::LocalHost, rxport_);
    connect(rxudpSocket_,SIGNAL(readyRead()),this,SLOT(readMsg()));
    txudpSocket_=new QUdpSocket(this);

    //**Show net config
    std::stringstream ss;
    ss<<rxport_;
    settingsTableModel_->ShowSettings("rxport",ss.str());
    ss.str("");
    ss<<txport_;
    settingsTableModel_->ShowSettings("txport",ss.str());
    ss.str("");
    ss<<address_;
    settingsTableModel_->ShowSettings("address",ss.str());
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

        tableListViewModel_->InsertRops(buffer);
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
    txudpSocket_->writeDatagram(buffer.data(), EOMDiagnosticUdpMsg::getSize(),QHostAddress::LocalHost, txport_);

    //tableListViewModel_->InserRops(buffer);
}

void MainWindow::on_alwaysflush_clicked()
{
    std::array<uint8_t, EOMDiagnosticUdpMsg::getSize()> udpMsg;
    EOMDiagnosticUdpMsg msg;
    EOMDiagnosticRopMsg::Info rop;
    rop=EOMDiagnosticRopMsg::Info{(uint16_t)DiagnosticRopCode::forceflush,(uint16_t)DiagnosticRopSeverity::trace,1,1,14,15,16,17,18};
    msg.addRop(rop);
    udpMsg.fill(0);
    msg.createUdpPacketData(udpMsg);

    // Build a packet to send
    QByteArray buffer((char*)udpMsg.data(),EOMDiagnosticUdpMsg::getSize());
    txudpSocket_->writeDatagram(buffer.data(), EOMDiagnosticUdpMsg::getSize(),QHostAddress::LocalHost, txport_);

    //tableListViewModel_->InserRops(buffer);
}

void MainWindow::on_noflush_clicked()
{
    std::array<uint8_t, EOMDiagnosticUdpMsg::getSize()> udpMsg;
    EOMDiagnosticUdpMsg msg;
    EOMDiagnosticRopMsg::Info rop;
    rop=EOMDiagnosticRopMsg::Info{(uint16_t)DiagnosticRopCode::unforceflush,(uint16_t)DiagnosticRopSeverity::trace,1,1,14,15,16,17,18};
    msg.addRop(rop);
    udpMsg.fill(0);
    msg.createUdpPacketData(udpMsg);

    // Build a packet to send
    QByteArray buffer((char*)udpMsg.data(),EOMDiagnosticUdpMsg::getSize());
    txudpSocket_->writeDatagram(buffer.data(), EOMDiagnosticUdpMsg::getSize(),QHostAddress::LocalHost, txport_);

    //tableListViewModel_->InserRops(buffer);
}

void MainWindow::on_clear_clicked()
{
    tableListViewModel_->clear();
}
