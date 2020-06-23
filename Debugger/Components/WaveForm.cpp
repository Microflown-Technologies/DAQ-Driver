#include "WaveForm.h"
#include "ui_WaveForm.h"

#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

WaveForm::WaveForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaveForm)
{
    ui->setupUi(this);
    initializeChart();
}

WaveForm::~WaveForm()
{
    delete ui;
}

void WaveForm::initializeChart()
{
    //Create chart
    m_chart = new QChart();
    //Create series
    m_series = new QLineSeries();
    //Create axis
    m_axisX = new QValueAxis();
    m_axisY = new QValueAxis();
    //Assign axis to chart
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    //Configure XAxis
    m_axisX->setRange(0, 2000);
    m_axisY->setRange(-qPow(2, 24),
                      qPow(2, 24));
    //Configure lineseries
    m_chart->addSeries(m_series);
    m_series->attachAxis(m_axisX);
    m_series->attachAxis(m_axisY);
    //Assign chart to GUI
    ui->waveFormGraph->setChart(m_chart);
}

void WaveForm::processBuffer(const DataBuffer &buffer)
{
//    m_bufferQueue << buffer;
//    if(m_bufferQueue.size() > 100) {
//        QList<QPointF> newPoints;
//        while(m_bufferQueue.size()) {
//            auto samples = bufferToSamples(m_bufferQueue.dequeue());
//            uint32_t startIndex = m_series->count();
//            newPoints.reserve(newPoints.size() + samples.size());
//            for(int sampleNr = 0; sampleNr < samples.size(); sampleNr+=100) {
//                newPoints << QPointF(++startIndex, samples[sampleNr].P);
//            }
//        }
//        m_series->append(newPoints);
//        qDebug() << "Update graph";
//    }

}

std::vector<Sample> WaveForm::bufferToSamples(const DataBuffer &buffer)
{
    size_t sampleCount = buffer.data().size() / sizeof(Sample);
    std::vector<Sample> samples(sampleCount);
    std::memcpy(samples.data(), buffer.data().data(), buffer.data().size());
    return samples;
}
