#include "Controls.h"
#include "ui_Controls.h"

Controls::Controls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Controls)
{
    ui->setupUi(this);
    connect(&m_uiTimer, &QTimer::timeout, this, &Controls::uiTimerTimeout);
    connect(&m_processTimer, &QTimer::timeout, this, &Controls::processTimerTimeout);
    connect(&m_daqDriver.time(), &Time::timeSynced, this, &Controls::onTimesynced);
    m_processTimer.start(1);
    m_uiTimer.start(500);
    populateSampleRateComboBox();
}

Controls::~Controls()
{
    delete ui;
}

void Controls::populateSampleRateComboBox()
{
    //Populate combobox
    const google::protobuf::EnumDescriptor* sampleRateEnum = DataFormat::SampleRate_descriptor();
    for (int enumNr = 0; enumNr < sampleRateEnum->value_count(); enumNr++) {
        ui->cmb_sampleRates->insertItem(enumNr, QString::fromStdString(sampleRateEnum->value(enumNr)->name()));
    }
    ui->cmb_sampleRates->setCurrentIndex(DataFormat::SampleRate::DataFormat_SampleRate__48000);
}


void Controls::on_btn_resetDevice_pressed() {
    m_daqDriver.reset();
}


void Controls::on_cmb_sampleRates_activated(int index) {

}

void Controls::on_cmb_sampleRates_currentIndexChanged(int index) {
    DataFormat dataFormat = m_daqDriver.formatting().dataFormat();
    dataFormat.set_samplerate(DataFormat::SampleRate(index));
    m_daqDriver.formatting().setDataFormat(dataFormat);
}

void Controls::onTimesynced(int64_t difference)
{
    ui->lbl_differenceMS->setText(QString::number(difference) + " ms");
}

void Controls::uiTimerTimeout()
{
    bool present = m_daqDriver.voyagerConnected();
    ui->btn_connect->setEnabled(present);
    if(present) {
        ui->lbl_present->setText("Yes");
        ui->lbl_present->setStyleSheet("color: green;");
    } else {
        ui->lbl_present->setText("No");
        ui->lbl_present->setStyleSheet("color: red;");
    }
    bool connected = m_daqDriver.isConnected();
    ui->grp_Sync->setEnabled(connected);
    ui->grp_StreamControl->setEnabled(connected);
    ui->groupBox->setEnabled(connected);
    ui->btn_resetDevice->setEnabled(connected);
    ui->btn_connect->setChecked(connected);
    ui->btn_Aux1IEPE->setChecked(m_daqDriver.iepe().getIEPE(IEPE::Aux1));
    ui->btn_Aux2IEPE->setChecked(m_daqDriver.iepe().getIEPE(IEPE::Aux2));
    ui->chk_StreamEnabled->setChecked(m_daqDriver.streaming().isStreaming());
    ui->lbl_buffer_count->setText(QString::number(m_daqDriver.streaming().pendingBufferCount()));
    //Update deviceinformation
    ui->lbl_deviceName->setText(QString::fromStdString(m_daqDriver.deviceInfo().remoteDeviceInfo().devicename()));
    ui->lbl_modelName->setText(QString::fromStdString(m_daqDriver.deviceInfo().remoteDeviceInfo().modelname()));
    ui->lbl_driverName->setText(QString::fromStdString(m_daqDriver.deviceInfo().remoteDeviceInfo().drivername()));
    ui->lbl_driverVersion->setText(QString::fromStdString(m_daqDriver.deviceInfo().remoteDeviceInfo().driverversion()));
    ui->lbl_softwareVersion->setText(QString::fromStdString(m_daqDriver.deviceInfo().remoteDeviceInfo().softwareversion()));
    ui->lbl_deviceSerial->setText(QString::fromStdString(m_daqDriver.deviceInfo().remoteDeviceInfo().deviceserial()));
}

void Controls::processTimerTimeout()
{

}

void Controls::on_btn_Sync_pressed()
{
    m_daqDriver.time().sync();
}

void Controls::on_btn_connect_toggled(bool checked)
{
    if(checked) ui->btn_connect->setChecked(m_daqDriver.connect());
    else m_daqDriver.disconnect();

}

DAQDriver& Controls::daqDriver()
{
    return m_daqDriver;
}

void Controls::on_chk_StreamEnabled_clicked(bool checked)
{
    if(checked) m_daqDriver.streaming().start();
    else m_daqDriver.streaming().stop();
}

void Controls::on_btn_Aux1IEPE_clicked(bool checked) {
    m_daqDriver.iepe().setIEPE(IEPE::Aux1, checked);
}

void Controls::on_btn_Aux2IEPE_clicked(bool checked) {
    m_daqDriver.iepe().setIEPE(IEPE::Aux2, checked);
}

void Controls::on_btn_setinputRange_clicked()
{
    m_daqDriver.inputRange().setInputRange(ui->cmb_input_Channel->currentIndex(),
                                           static_cast<InputRange::Voltage>(ui->cmb_inputrange->currentIndex()));
}

void Controls::on_cmb_input_Channel_currentIndexChanged(int index) {
    ui->cmb_inputrange->setCurrentIndex(m_daqDriver.inputRange().getInputRange(index));
}

void Controls::on_btn_refreshDeviceInfo_pressed() {
    m_daqDriver.deviceInfo().refresh();
}
