#include "myaudio.h"

#include <QDebug>

#define SAMPLE_RATE 44100
#define CHANNELS 1
#define SAMPLE_SIZE 16
#define SAMPLE_BYTE SAMPLE_SIZE/8
#define SAMPLE_TYPE SignedInt
#define BUFFER_SIZE 0x10000

MyAudio::MyAudio()
{
    format.setSampleRate(SAMPLE_RATE);
    format.setChannelCount(CHANNELS);
    format.setSampleSize(SAMPLE_SIZE);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SAMPLE_TYPE);

    //print out the output device setup parameters
    QAudioDeviceInfo deviceOut(QAudioDeviceInfo::defaultOutputDevice());
    qDebug() << "Selected Output device ="          << deviceOut.deviceName();
    if (!deviceOut.isFormatSupported(format)) {
        qWarning() << "Raw audio format not supported by output device";
        return;
    }

    //print out the input device setup parameters
    QAudioDeviceInfo deviceIn(QAudioDeviceInfo::defaultInputDevice());
    qDebug() << "Selected input device ="           << deviceIn.deviceName();
    if (!deviceIn.isFormatSupported(format)) {
        qWarning() << "Raw audio format not supported by by output device";
        return;
    }

    //configure device
    audioOut = new QAudioOutput(deviceOut, format);
    audioIn  = new QAudioInput(deviceIn, format);


    pbuff = new char[BUFFER_SIZE];
    RXbuff = 0;                     // set RX buffer pointer
    TXbuff = 0;                     // set TX buffer pointer

    audioOut->setVolume(1.0);       // volume 0 to 1.0

    qDebug() << "File open" << open(QIODevice::ReadWrite);
    qDebug() << "is device Sequential=" << isSequential();
    audioIn->start(this);           // start reading device
    audioOut->start(this);          // start writing to device
}

// rewrite a virtual function of QIODevice Class
// send to output (Speaker)
qint64 MyAudio::readData(char *data, qint64 len)
{
    qint64 total = 0;
    if (TXbuff + len < RXbuff) total = len;
    else total = RXbuff - TXbuff;

    if (RXbuff%BUFFER_SIZE + total <= BUFFER_SIZE) {
        memcpy(&data[0], &pbuff[RXbuff%BUFFER_SIZE], total);
    }
    else {
        memcpy(&data[0], &pbuff[RXbuff%BUFFER_SIZE], BUFFER_SIZE - RXbuff%BUFFER_SIZE);
        memcpy(&data[BUFFER_SIZE - RXbuff%BUFFER_SIZE], &pbuff[0], RXbuff%BUFFER_SIZE + total - BUFFER_SIZE);
    }

    return total;  //the reset interval
}


// audio input (from Microphone)
qint64 MyAudio::writeData(const char *data, qint64 len)
{
    if (RXbuff%BUFFER_SIZE + len <= BUFFER_SIZE) {
        memcpy(&pbuff[RXbuff%BUFFER_SIZE], &data[0], len);
    }
    else {
        memcpy(&pbuff[RXbuff%BUFFER_SIZE], &data[0], BUFFER_SIZE - RXbuff%BUFFER_SIZE);
        memcpy(&pbuff[0], &data[BUFFER_SIZE - RXbuff%BUFFER_SIZE], RXbuff%BUFFER_SIZE + len - BUFFER_SIZE);
    }
    RXbuff += len;
    return len;
}

qint64 MyAudio::bytesAvailable() const{return 0;}
