#include "framerate.h"
#include <iostream>
FrameRate::FrameRate():
        mFrames(0),
        mTime(0.0f),
        mCurrentFrameRate(0)
{

}

void FrameRate::tick(TFloat deltaTime)
{
    mFrames++;
    mTime+=deltaTime;
    if(mTime >= 1.f)
    {
        mCurrentFrameRate = mFrames;
        mTime = mTime - 1.f;
        mFrames = 0;
    }
}

TInt FrameRate::getCurrentFrameRate() const
{
    return mCurrentFrameRate;
}
