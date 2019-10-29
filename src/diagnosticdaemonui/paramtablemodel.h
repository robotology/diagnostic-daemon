#ifndef PARAMTABLEMODEL_H
#define PARAMTABLEMODEL_H

#include <QStandardItemModel>
#include "RopParser.h"

class ParamTableModel : public QStandardItemModel
{
public:
    ParamTableModel();

    void ShowRop(const QModelIndex &index);

private:
    RopParser ropParser_;
};

#endif // PARAMTABLEMODEL_H
