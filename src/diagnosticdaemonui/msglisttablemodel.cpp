#include "msglisttablemodel.h"

#include <QTextCodec>

MsgListTableModel::MsgListTableModel()
{
    QStringList list{"","raw"};
    setHorizontalHeaderLabels(list);
}

void MsgListTableModel::InsertRops(const QByteArray& buffer)
{
    /*
    EOMDiagnosticUdpMsg msg;
    msg.parse((uint8_t *)buffer.data(),buffer.size());

    unsigned int ropNumber=msg.getCurrentRopNumber();

    for(unsigned int index=0;index<ropNumber;index++)
    {
        EOMDiagnosticRopMsg rop;
        msg.getRop(rop,index);
        if(rop.isEmptyRop())
            continue;

        QByteArray qRop((const char*)rop.data(),rop.getSize());
        InsertSingleRop(qRop);
    }
    */
}

void MsgListTableModel::InsertSingleRop(const QByteArray& buffer)
{
    /*
    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
    QString dataAsString = codec->toUnicode(buffer.toHex(':'));
    QStandardItem* ropItem = new QStandardItem(dataAsString);
    ropItem->setData(buffer,Qt::UserRole);  
    ropItem->setFlags(ropItem->flags() &  ~Qt::ItemIsEditable);

    QList<QStandardItem*> toInsert;
    toInsert.insert(0,ropItem);

    switch(buffer[2])
    {
    case (int)DiagnosticRopSeverity::critical:
         ropItem->setIcon(QIcon(":/icons/critical.png"));
        break;
    case (int)DiagnosticRopSeverity::error:
         ropItem->setIcon(QIcon(":/icons/error.png"));
         break;
    case (int)DiagnosticRopSeverity::warning:
         ropItem->setIcon(QIcon(":/icons/warning.png"));
        break;
    case (int)DiagnosticRopSeverity::trace:
         ropItem->setIcon(QIcon(":/icons/trace.png"));
        break;
    case (int)DiagnosticRopSeverity::info:
         ropItem->setIcon(QIcon(":/icons/info.png"));
        break;
    case (int)DiagnosticRopSeverity::debug:
         ropItem->setIcon(QIcon(":/icons/debug.png"));
        break;
    }
    in
    sertRow(0,toInsert);
    */
}
