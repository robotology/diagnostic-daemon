#ifndef MSGLISTMODEL_H
#define MSGLISTMODEL_H

#include <QStandardItemModel>

class MsgListTableModel : public QStandardItemModel
{
public:
    MsgListTableModel();
    void InsertRops(const QByteArray& buffer);

private:
    void InsertSingleRop(const QByteArray& buffer);
};

#endif // MSGLISTMODEL_H
