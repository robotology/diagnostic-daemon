#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QUdpSocket>
#include "msglisttablemodel.h"
#include "paramtablemodel.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    MsgListTableModel *tableListViewModel_;
    ParamTableModel *paramTableModel_;
    QUdpSocket *udpSocket_;

private slots:
    void readMsg();

    void on_msglist_clicked(const QModelIndex &index);
};
#endif // MAINWINDOW_H
