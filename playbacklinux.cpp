#include "playbacklinux.h"

#include <QProcess>

PlaybackLinux::PlaybackLinux()
{

}

void PlaybackLinux::start()
{
    QProcess::startDetached("pactl load-module module-loopback latency_msec=50");
}

void PlaybackLinux::stop()
{
    QProcess::startDetached("pactl unload-module module-loopback");
}

auto PlaybackLinux::isStarted()
{
    QProcess processPactlList;
    processPactlList.start("pactl list short modules");
    processPactlList.waitForFinished();
    QString loadedModules(processPactlList.readAllStandardOutput());
    if (loadedModules.contains("module-loopback"))
        return true;
    else
        return false;
}
