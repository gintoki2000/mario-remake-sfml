#include "animatedtile.h"
#include <exception>
#include <iostream>
CAnimatedTile::CAnimatedTile(const std::vector<TextureRegion *> &frames, const std::vector<TUInt> intervals):
    mFrames(frames),
    mIntervals(intervals),
    mLoopDuration(0)
{
    for(const auto& interval : intervals)
    {
        mLoopDuration+=interval;
    }

}

const TextureRegion &CAnimatedTile::getTextureRegion() const
{
	TUInt32 index = getCurrentFrameIndex();
    return *mFrames[index];
}

TUInt64 CAnimatedTile::getCurrentFrameIndex() const
{
    TUInt32 currentTime = sBaseTime % mLoopDuration;
    for(TUInt64 i = 0; i < mIntervals.size(); i++)
    {
        TUInt32 animationInterval = mIntervals[i];
        if(currentTime <= animationInterval) return i;
        currentTime -= animationInterval;
    }
    throw std::runtime_error("could not determine index(CAnimatedTile::getCurrentFrameIndex())");
}

void CAnimatedTile::updateAnimationBaseTime()
{
    CAnimatedTile::sBaseTime = CAnimatedTile::sClock.getElapsedTime().asMilliseconds();
}

sf::Clock CAnimatedTile::sClock = sf::Clock();
TInt CAnimatedTile::sBaseTime = 0;
