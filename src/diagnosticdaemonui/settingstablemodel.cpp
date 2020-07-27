#include "settingstablemodel.h"
#include <QTextCodec>

SettingsTableModel::SettingsTableModel()
{
    QStringList list{"","raw"};
    setHorizontalHeaderLabels(list);
}

void SettingsTableModel::ShowSettings(const std::string& name,const std::string& value)
{
    QStandardItem* nameItem = new QStandardItem(QString(name.c_str()));
    nameItem->setFlags(nameItem->flags() &  ~Qt::ItemIsEditable);
    QStandardItem* valueItem = new QStandardItem(QString(value.c_str()));
    valueItem->setFlags(valueItem->flags() &  ~Qt::ItemIsEditable);

    QList<QStandardItem*> toInsert;
    toInsert.insert(0,nameItem);
    toInsert.insert(1,valueItem);
    insertRow(0,toInsert);
}
