#ifndef TIMER_H
#define TIMER_H
#include "base.h"

class TTimer
{
public:

    TTimer();

    TTimer(TInt numberOfFrames);

    void start();

    void pause();

    void finish();

    TBoolean tick();

    TBoolean isActive() const;

    TBoolean isPaused() const;

    TBoolean isComplete() const;

    TInt getFramesRemaining() const;

    TInt getFramesEslapsed() const;

    void setFramesRemaining(TInt numberOfFrames);
    TInt getTotalFrames() const;

private:
    TInt mTotalFrames;
    TInt mFramesRemaining;
    TBoolean mIsActive;
    TBoolean mIsPaused;
};

#endif // TIMER_H
