#ifndef PLAYBACKLINUX_H
#define PLAYBACKLINUX_H

#include "playback.h"

class PlaybackLinux : public Playback
{
public:
    PlaybackLinux();
    ~PlaybackLinux() {}

    void start();
    void stop();

private:
    auto isStarted();
};

#endif // PLAYBACKLINUX_H
