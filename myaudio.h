#ifndef MYAUDIO_H
#define MYAUDIO_H

#include <QIODevice>
#include <QAudioInput>
#include <QAudioOutput>

class MyAudio : public QIODevice
{
    Q_OBJECT

public:
    QAudioOutput *audioOut;
    QAudioInput  *audioIn;

    MyAudio();
    ~MyAudio(){}
    void fillBuffer();
    QAudioFormat format;
    char *pbuff;
    quint64 RXbuff, TXbuff;

protected:
    qint64 readData(char *data, qint64 len);
    qint64 bytesAvailable() const;
    qint64 writeData(const char *data, qint64 len);
};

#endif // MYAUDIO_H
