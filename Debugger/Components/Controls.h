#ifndef CONTROLS_H
#define CONTROLS_H

//Qt framework
#include <QTimer>
#include <QWidget>

//Driver
#include "DAQDriver.h"
#include <google/protobuf/reflection.h>
#include "DataFormat.pb.h"

namespace Ui {
class Controls;
}

class Controls : public QWidget
{
    Q_OBJECT

public:
    explicit Controls(QWidget *parent = nullptr);
    ~Controls();

    DAQDriver &daqDriver();

private:
    void populateSampleRateComboBox();

private slots:

    void on_btn_resetDevice_pressed();

    void on_cmb_sampleRates_activated(int index);

    void on_cmb_sampleRates_currentIndexChanged(int index);

    void onTimesynced(int64_t difference);
    void uiTimerTimeout();
    void processTimerTimeout();
    void on_btn_Sync_pressed();

    void on_btn_connect_toggled(bool checked);

    void on_chk_StreamEnabled_clicked(bool checked);

    void on_btn_Aux1IEPE_clicked(bool checked);

    void on_btn_Aux2IEPE_clicked(bool checked);

    void on_btn_setinputRange_clicked();

    void on_cmb_input_Channel_currentIndexChanged(int index);

    void on_btn_refreshDeviceInfo_pressed();

private:
    DAQDriver m_daqDriver;
    QTimer m_processTimer;
    QTimer m_uiTimer;
    Ui::Controls *ui;
};

#endif // CONTROLS_H
