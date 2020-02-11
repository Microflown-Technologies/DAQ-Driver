#include "SystemAudioDriver.h"

SystemAudioDriver::SystemAudioDriver(QObject *parent) : QObject(parent)
{
    QAudioFormat customFormat;
    customFormat.setChannelCount(6);
    customFormat.setCodec("audio/pcm");
    customFormat.setSampleRate(48000);
    customFormat.setByteOrder(QAudioFormat::LittleEndian);
    customFormat.setSampleSize(32);
    customFormat.setSampleType(QAudioFormat::SignedInt);
    m_audioOutput = new QAudioOutput(customFormat);
    m_audioOutput->setBufferSize(48000 / 12.5); //Buffer 10 milliseconds of audio
    m_audioOutput->setNotifyInterval(100); //Process data every millisecond
    m_audioOutput->setVolume(1);
    audioIODevice = m_audioOutput->start(); //Set audiodevice to new value
}

void SystemAudioDriver::newDataBuffer(const DataBuffer &buffer)
{
    audioIODevice->write(buffer.data().c_str());
}
