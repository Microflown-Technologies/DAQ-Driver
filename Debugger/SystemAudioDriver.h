#ifndef SYSTEMAUDIODRIVER_H
#define SYSTEMAUDIODRIVER_H

//Qt framework
#include <QDebug>
#include <QObject>
#include <QAudioOutput>

//Internal headers
#include "DataBuffer.pb.h"

class SystemAudioDriver : public QObject
{
    Q_OBJECT
public:
    explicit SystemAudioDriver(QObject *parent = nullptr);

signals:

public slots:
    void newDataBuffer(const DataBuffer &buffer);

private:
    QAudioOutput *m_audioOutput;
    QIODevice *audioIODevice; ///< Is the IODevice that represent the audio output that is being written to

};

#endif // SYSTEMAUDIODRIVER_H
