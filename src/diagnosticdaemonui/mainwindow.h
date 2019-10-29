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
    QUdpSocket *rxudpSocket_;
    QUdpSocket *txudpSocket_;

    static constexpr uint16_t rxport_{9001};
    static constexpr uint16_t txport_{9000};

private slots:
    void readMsg();

    void on_msglist_clicked(const QModelIndex &index);
    void on_stoptrace_clicked();
    void on_starttrace_clicked();
    void on_alwaysflush_clicked();
    void on_noflush_clicked();
    void on_clear_clicked();
};
#endif // MAINWINDOW_H
