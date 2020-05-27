#ifndef DISCOVEREDVOYAGERLIST_H
#define DISCOVEREDVOYAGERLIST_H

//Qt framework
#include <QWidget>
#include <QTableWidgetItem>

//Internal headers
#include "Discovery/DeviceDiscovery.h"

namespace Ui {
class DiscoveredVoyagerList;
}

class DiscoveredVoyagerList : public QWidget
{
    Q_OBJECT

public:
    explicit DiscoveredVoyagerList(QWidget *parent = nullptr);
    ~DiscoveredVoyagerList();

signals:
    void devicePicked(QString ip);

private slots:
    void on_btn_refresh_clicked();

    void on_tableWidget_cellDoubleClicked(int row, int column);

private:
    Ui::DiscoveredVoyagerList *ui;
};

#endif // DISCOVEREDVOYAGERLIST_H
