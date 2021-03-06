#include "paramtablemodel.h"

ParamTableModel::ParamTableModel()
{

}

void ParamTableModel::ShowRop(const QModelIndex &index)
{
    clear();
    QStringList list{"name","value"};
    setHorizontalHeaderLabels(list);

    /*QStandardItem item;
    item.setData(QColor(Qt::green), Qt::BackgroundRole);
    item.setData(QColor(Qt::red), Qt::FontRole);
    */

   /*
    QStandardItem *root = invisibleRootItem();

    QByteArray param=index.sibling(index.row(),0).data(Qt::UserRole).toByteArray();

    EOMDiagnosticRopMsg rop(param.data(), param.size());
    auto out=ropParser_.parse(rop);

    for(const auto& current:out)
    {
        QStandardItem * name{nullptr};
        QStandardItem * value{nullptr};
        name = new QStandardItem(std::get<0>(current).c_str());
        value = new QStandardItem(std::get<1>(current).c_str());
        name->setData(QColor(255,245,230), Qt::BackgroundRole);
        auto myfont=QFont("Monospace",9,1,true);
        myfont.setBold(true);
        name->setFont(myfont);
        value->setFont(QFont("Monospace",9,1,false));
        if(std::get<1>(current).c_str()==ropSeverity.at(DiagnosticRopSeverity::error))
        {
            value->setData(QColor(212,30,30), Qt::ForegroundRole);
        }
        if(std::get<0>(current)!="time")
        {
            value->setFlags(Qt::NoItemFlags|Qt::ItemIsEnabled);
        }
        else
            value->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);

        name->setFlags(Qt::NoItemFlags|Qt::ItemIsEnabled);

        QList<QStandardItem*> toInsert;
        toInsert.insert(0,name);
        toInsert.insert(1,value);

        //name->setIcon(QIcon(":/icons/info.png"));

        root->appendRow(toInsert);
    }*/
}
