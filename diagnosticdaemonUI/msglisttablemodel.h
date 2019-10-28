#ifndef MSGLISTMODEL_H
#define MSGLISTMODEL_H

#include <QStandardItemModel>

#include "EOMDiagnosticUdpMsg.h"

class MsgListTableModel : public QStandardItemModel
{
public:
    MsgListTableModel();
    void InserRops(const QByteArray& buffer);

private:
    void InsertSingleRop(const QByteArray& buffer);
};

#endif // MSGLISTMODEL_H
