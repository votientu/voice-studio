#include <QDebug>
#include <QIODevice>
#include <QAudioInput>
#include <QAudioOutput>
#include <QCoreApplication>

class myAudio :public QIODevice
{
  // Q_OBJECT

public:
     QAudioOutput *audioOut;
     QAudioInput  *audioIn;

     myAudio();
    ~myAudio(){}
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

#define SAMPLE_RATE 22050
#define CHANNELS 1
#define SAMPLE_SIZE 16
#define SAMPLE_TYPE SignedInt

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    myAudio *m= new myAudio();
    return a.exec();
}
myAudio::myAudio()
    {
    formatIn.setSampleRate(SAMPLE_RATE);
    formatIn.setChannelCount(CHANNELS);
    formatIn.setSampleSize(SAMPLE_SIZE);
    formatIn.setCodec("audio/pcm");
    formatIn.setByteOrder(QAudioFormat::LittleEndian);
    formatIn.setSampleType(QAudioFormat::SAMPLE_TYPE);

    formatOut.setSampleRate(SAMPLE_RATE);
    formatOut.setChannelCount(CHANNELS);
    formatOut.setSampleSize(SAMPLE_SIZE);
    formatOut.setCodec("audio/pcm");
    formatOut.setByteOrder(QAudioFormat::LittleEndian);
    formatOut.setSampleType(QAudioFormat::SAMPLE_TYPE);

//print out the output device setup parameters
     QAudioDeviceInfo          deviceOut(QAudioDeviceInfo::availableDevices(QAudio::AudioOutput).at(0));     //select output device 0
     qDebug()<<"Selected Output device ="<<deviceOut.deviceName();

//print out the input device setup parameters
     QAudioDeviceInfo     deviceIn(QAudioDeviceInfo::availableDevices(QAudio::AudioInput).at(0));     //select output device 0
     qDebug()<<"Selected input device ="<<deviceIn.deviceName();

//configure device
     audioOut = new QAudioOutput(deviceOut,formatOut,0);
     audioIn  = new QAudioInput (deviceIn, formatIn,0);

//print out the device specifications
     foreach(const QAudioDeviceInfo &deviceInfo,     QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
          {
          qDebug() << "\nSuported Input devices";
          qDebug() << "\nDevice name: "             << deviceInfo.deviceName();
          qDebug() << "Supported channel count: "   << deviceInfo.supportedChannelCounts();
          qDebug() << "Supported Codec: "           << deviceInfo.supportedCodecs();
          qDebug() << "Supported byte order: "      << deviceInfo.supportedByteOrders();
          qDebug() << "Supported Sample Rate: "     << deviceInfo.supportedSampleRates();
          qDebug() << "Supported Sample Size: "     << deviceInfo.supportedSampleSizes();
          qDebug() << "Supported Sample Type: "     << deviceInfo.supportedSampleTypes();
          qDebug() << "Preferred Device settings:"  << deviceInfo.preferredFormat();
          }
     foreach(const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
         {
         qDebug() << "\nSuported output devices";
         qDebug() << "Device name: "             << deviceInfo.deviceName();
         qDebug() << "Supported channel count: "   << deviceInfo.supportedChannelCounts();
         qDebug() << "Supported Codec: "           << deviceInfo.supportedCodecs();
         qDebug() << "Supported byte order: "      << deviceInfo.supportedByteOrders();
         qDebug() << "Supported Sample Rate: "     << deviceInfo.supportedSampleRates();
         qDebug() << "Supported Sample Size: "     << deviceInfo.supportedSampleSizes();
         qDebug() << "Supported Sample Type: "     << deviceInfo.supportedSampleTypes();
         qDebug() << "Preferred Device settings:"  << deviceInfo.preferredFormat();
         }

      buff.resize(0x10000);   //create a rx buffer

      pbuff=buff.data();       //get the buff address;
      RXbuff=0;                //set RX buffer pointer

      qDebug()<<"File open"<<open(QIODevice::ReadWrite);
      qDebug()<<"is device Sequential="<<isSequential();
      audioIn->start(this); //start reading device

      audioOut->setVolume(1.0);  //volume 0 to 1.0
      audioOut->start(this);    //start writing to device
}

//QIODevice Class (Protected Functions)This function is called by QIODevice.
//send to output(Speaker)
qint64 myAudio::readData(char *data, qint64 len)
{
static quint64 TXbuff=0;
qint64 total = 0;
while (len > total  && RXbuff>TXbuff)//write and synchonise buffers
       {
         //write data to speaker
        memcpy(&data[total],&pbuff[TXbuff%0x10000],2);    //copy 2 Bytes
        TXbuff+=2; //point to next buffer 16 bit location
        total+=2;
       }
return total;  //the reset interval
}


//audio input (from Microphone)
qint64 myAudio::writeData(const char *data, qint64 len)
{
int total=0;
while (len > total)
       {
        memcpy(&pbuff[RXbuff%0x10000],&data[total], 2); //write 2Bytes into circular buffer(64K)
        RXbuff+=2; //next 16bit buffer location
        total+=2;  //next data location
      }
return (total); //return total number of bytes received
}

qint64 myAudio::bytesAvailable() const{return 0;}
