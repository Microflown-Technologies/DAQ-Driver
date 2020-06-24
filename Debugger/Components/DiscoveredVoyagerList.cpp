#include "DiscoveredVoyagerList.h"
#include "ui_DiscoveredVoyagerList.h"

DiscoveredVoyagerList::DiscoveredVoyagerList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DiscoveredVoyagerList)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    on_btn_refresh_clicked();
}

DiscoveredVoyagerList::~DiscoveredVoyagerList()
{
    delete ui;
}

void DiscoveredVoyagerList::on_btn_refresh_clicked()
{
    ui->tableWidget->setRowCount(0);
    auto voyagerList = DeviceDiscovery::discover(1, true);
    for(auto voyager: voyagerList) {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::fromStdString(voyager->ipAddress())));
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::fromStdString(voyager->deviceName())));
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::fromStdString(voyager->deviceSerial())));
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, new QTableWidgetItem(QString::fromStdString(voyager->driverVersion())));
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, new QTableWidgetItem(QString::fromStdString(voyager->softwareVersion())));
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 5, new QTableWidgetItem(QString::fromStdString(voyager->modelName())));
    }
}

void DiscoveredVoyagerList::on_tableWidget_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column)
    QString ip = ui->tableWidget->item(row, 0)->text();
    emit devicePicked(ip);
}
