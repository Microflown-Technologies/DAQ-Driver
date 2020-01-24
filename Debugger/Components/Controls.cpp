#include "Controls.h"
#include "ui_Controls.h"

Controls::Controls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Controls)
{
    ui->setupUi(this);
    connect(&m_processTimer, &QTimer::timeout, this, &Controls::processTimerTimeout);
    m_processTimer.start(1);
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

void Controls::on_btn_GrabControl_pressed() {
    m_daqDriver.deviceControl().takeControl();
    ui->btn_GrabControl->setEnabled(!m_daqDriver.deviceControl().hasControl());
    ui->btn_ReleaseControl->setEnabled(m_daqDriver.deviceControl().hasControl());
}

void Controls::on_btn_ReleaseControl_pressed() {
    m_daqDriver.deviceControl().releaseControl();
    ui->btn_GrabControl->setEnabled(!m_daqDriver.deviceControl().hasControl());
    ui->btn_ReleaseControl->setEnabled(m_daqDriver.deviceControl().hasControl());
}

void Controls::on_btn_resetDevice_pressed() {
    m_daqDriver.deviceControl().reset();
    ui->btn_GrabControl->setEnabled(!m_daqDriver.deviceControl().hasControl());
    ui->btn_ReleaseControl->setEnabled(m_daqDriver.deviceControl().hasControl());
}

void Controls::on_chk_StreamEnabled_toggled(bool checked) {
    if(checked) m_daqDriver.streaming().start();
    else m_daqDriver.streaming().stop();
}

void Controls::on_cmb_sampleRates_activated(int index) {

}

void Controls::on_cmb_sampleRates_currentIndexChanged(int index) {
    DataFormat dataFormat = m_daqDriver.formatting().dataFormat();
    dataFormat.set_samplerate(DataFormat::SampleRate(index));
    m_daqDriver.formatting().setDataFormat(dataFormat);
}

void Controls::processTimerTimeout()
{
    m_daqDriver.process();
}

void Controls::on_btn_Sync_pressed()
{
    m_daqDriver.time().sync();
}
