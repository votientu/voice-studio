#ifndef MYAUDIO_H
#define MYAUDIO_H

#include <QIODevice>
#include <QAudioInput>
#include <QAudioOutput>

class MyAudio : public QIODevice
{
    // Q_OBJECT

public:
    QAudioOutput *audioOut;
    QAudioInput  *audioIn;

    MyAudio();
    ~MyAudio(){}
    void fillBuffer();
    QAudioFormat formatIn,formatOut;
    QByteArray buff;
    char *pbuff;
    quint64 RXbuff;
    quint64 buffPtr;
protected:
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
    qint64 bytesAvailable() const;
};

#endif // MYAUDIO_H
