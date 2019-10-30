#ifndef SETTINGSMODEL_H
#define SETTINGSMODEL_H

#include <QStandardItemModel>

#include "EOMDiagnosticUdpMsg.h"

class SettingsTableModel : public QStandardItemModel
{
public:
    SettingsTableModel();
    void ShowSettings(const std::string& name,const std::string& value);
};

#endif // MSGLISTMODEL_H
