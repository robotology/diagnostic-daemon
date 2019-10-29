#include "paramtablemodel.h"
#include "EOMDiagnosticRopMsg.h"

ParamTableModel::ParamTableModel()
{

}

void ParamTableModel::ShowRop(const QModelIndex &index)
{
    clear();
    QStringList list{"name","value"};
    setHorizontalHeaderLabels(list);

    QStandardItem *root = invisibleRootItem();

    QByteArray param=index.sibling(index.row(),0).data(Qt::UserRole).toByteArray();

    EOMDiagnosticRopMsg rop(param.data(), param.size());
    auto out=ropParser_.parse(rop);

    for(const auto& current:out)
    {
        QStandardItem * name{nullptr};
        QStandardItem * value{nullptr};
        name = new QStandardItem(current.first.c_str());
        value = new QStandardItem(current.second.c_str());

        QList<QStandardItem*> toInsert;
        toInsert.insert(0,name);
        toInsert.insert(1,value);

        //name->setIcon(QIcon(":/icons/info.png"));
        name->setFlags(Qt::NoItemFlags|Qt::ItemIsEnabled);
        value->setFlags(Qt::NoItemFlags|Qt::ItemIsEnabled);
        root->appendRow(toInsert);
    }
}
