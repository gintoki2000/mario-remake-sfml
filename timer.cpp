#include "timer.h"
#include <algorithm>

TTimer::TTimer():
    mTotalFrames(0),
    mFramesRemaining(-1),
    mIsActive(false),
    mIsPaused(false)
{

}

TTimer::TTimer(TInt numberOfFrames):
    mTotalFrames(numberOfFrames),
    mFramesRemaining(-1),
    mIsActive(false),
    mIsPaused(false)
{

}

void TTimer::start()
{
    mFramesRemaining = mTotalFrames;
    mIsPaused = false;
    mIsActive = true;
}

void TTimer::pause()
{
    mIsPaused = true;
}

void TTimer::finish()
{
    mFramesRemaining = 0;
    mIsPaused = false;
    mIsActive = false;
}

TBoolean TTimer::tick()
{
    if(mIsActive && !mIsPaused)
    {
        --mFramesRemaining;
        if(mFramesRemaining <= 0)
        {
            mIsActive = false;
            mFramesRemaining = 0;
            return true;
        }
        return mIsActive;
    }
    return false;
}

TBoolean TTimer::isActive() const
{
    return mIsActive;
}

TBoolean TTimer::isPaused() const
{
    return mIsPaused;
}

TBoolean TTimer::isComplete() const
{
    return mFramesRemaining == 0;
}

TInt TTimer::getFramesRemaining() const
{
    return mFramesRemaining;
}

TInt TTimer::getFramesEslapsed() const
{
    return mTotalFrames - mFramesRemaining;
}

void TTimer::setFramesRemaining(TInt numberOfFrames)
{
    mFramesRemaining = std::max<TInt>(std::min<TInt>(numberOfFrames, mTotalFrames), 0);
}

TInt TTimer::getTotalFrames() const
{
    return mTotalFrames;
}


