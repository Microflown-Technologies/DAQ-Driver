#ifndef WAVEFORM_H
#define WAVEFORM_H

//Qt framework
#include <QDebug>
#include <QQueue>
#include <QWidget>

//STD framework
#include <stdint.h>

//Protobuf messages
#include "DataBuffer.pb.h"

//Qt charts
#include <QtCharts/QChartGlobal>
QT_CHARTS_BEGIN_NAMESPACE
class QChart;
class QValueAxis;
class QLineSeries;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE


namespace Ui {
class WaveForm;
}

struct Sample {
    int32_t P;
    int32_t Ux;
    int32_t Uy;
    int32_t Uz;
    int32_t Aux1;
    int32_t Aux2;
};

class WaveForm : public QWidget
{
    Q_OBJECT

public:
    explicit WaveForm(QWidget *parent = nullptr);
    ~WaveForm();

    void initializeChart();

public slots:
    void processBuffer(const DataBuffer &buffer);

    std::vector<Sample> bufferToSamples(const DataBuffer &buffer);

private:
    Ui::WaveForm *ui;
    QChart *m_chart;
    QValueAxis *m_axisX, *m_axisY;
    QLineSeries *m_series;
    QQueue<DataBuffer> m_bufferQueue;
};

#endif // WAVEFORM_H
