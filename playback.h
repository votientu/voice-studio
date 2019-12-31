#ifndef PLAYBACK_H
#define PLAYBACK_H

class Playback
{
public:
    Playback();
    virtual ~Playback() {}

    virtual void start() {}
    virtual void stop() {}
    virtual void setVolume(float level) {}
};

#endif // PLAYBACK_H
