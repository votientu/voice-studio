#include <QCoreApplication>

#include "myaudio.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    auto* m= new MyAudio();
    return a.exec();
}

