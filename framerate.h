

#ifndef FRAMERATE_H
#define FRAMERATE_H
#include "base.h"

class FrameRate
{
public:
    FrameRate();

    void tick(TFloat deltaTime);

    TInt getCurrentFrameRate() const;
private:
    TInt mFrames;
    TFloat mTime;
    TInt mCurrentFrameRate;
};

#endif // FRAMERATE_H
