#include "animation.h"
#include <algorithm>
#include <iostream>
#include <cassert>

TAnimation::TAnimation():
    mPlayMode(TPlayMode::ENormal),
    mFrameDuration(1),
    mLastFrameIndex(0),
    mLastTimeState(0)
{

}

TAnimation::TAnimation(TInt frameDuration, const std::vector<sf::Sprite> &keyFrames, TAnimation::TPlayMode playMode):
    mKeyFrames(keyFrames),
    mPlayMode(playMode),
    mFrameDuration(frameDuration),    
    mLastFrameIndex(0),
    mLastTimeState(0)
{
}





void TAnimation::create(sf::Texture *source, const std::vector<sf::IntRect> &rects)
{
    mKeyFrames.clear();
    mFrameDuration = 1;
    mPlayMode = TPlayMode::ENormal;
    mLastTimeState = 0;
    mLastFrameIndex = 0;
    for(const auto& rect : rects)
    {
        mKeyFrames.emplace_back(*source, rect);
    }

}

void TAnimation::create(sf::Texture *source, const sf::IntRect &rect)
{
    mKeyFrames.clear();
    mFrameDuration = 1;
    mPlayMode = TPlayMode::ENormal;
    mLastTimeState = 0;
    mLastFrameIndex = 0;
    mKeyFrames.emplace_back(*source, rect);
}

void TAnimation::create(sf::Texture *source, TInt startX, TInt startY, TInt tileWidth, TInt tileHeight, TInt cols, TInt rows)
{
    mKeyFrames.clear();
    mFrameDuration = 1;
    mPlayMode = TPlayMode::ENormal;
    mLastTimeState = 0;
    mLastFrameIndex = 0;
    for(TInt x = 0; x < cols; x++)
        for(TInt y = 0; y < rows; y++)
            mKeyFrames.emplace_back(*source, sf::IntRect(startX + x * tileWidth, startY + y * tileHeight, tileWidth, tileHeight));
}

const std::vector<sf::Sprite> &TAnimation::getKeyFrames() const
{
    return mKeyFrames;
}

void TAnimation::setKeyFrames(const std::vector<sf::Sprite> &keyFrames)
{
    mKeyFrames = keyFrames;
}

TInt TAnimation::getFrameDuration() const
{
    return mFrameDuration;
}

void TAnimation::setFrameDuration(TInt frameDuration)
{
    if(frameDuration < 0) throw std::runtime_error("argument \'frameduration\' can not be a negative number in function! TAnimation::setFrameDuration(TInt frameDuration)");
    mFrameDuration = frameDuration;
}

TInt TAnimation::getCurrentKeyFrameIndex(TInt state) const
{
    const TInt length = mKeyFrames.size();
    if(state < 0) throw  std::runtime_error("argument \'state\' can not be a negative nummber! in function TAnimation::getCurrentKeyFrameIndex(TInt state)");
    if(length == 0) throw std::runtime_error("a animation empty so can not return index of current frame! in function TAnimation::getCurrentKeyFrameIndex(TInt state)");
    if(length == 1) return 0;
    TInt frameNumber = state / mFrameDuration;
    switch (mPlayMode)
    {
    case(TPlayMode::ENormal):
        frameNumber =  std::min<TInt>(length - 1, frameNumber);
        break;
    case(TPlayMode::EReversed):
        frameNumber = std::max<TInt>(length - frameNumber - 1, 0);
        break;
    case(TPlayMode::ELoop):
        frameNumber = frameNumber % length;
        break;
    case(TPlayMode::ELoopReversed):
        frameNumber = frameNumber % length;
        frameNumber = length - frameNumber - 1;
        break;
    case(TPlayMode::ELoopPingpong):
        frameNumber = frameNumber % ((length * 2) - 2);
        if(frameNumber >= length)
            frameNumber = length - 2 - (frameNumber - length);
        break;
    }
    return frameNumber;
}

const sf::Sprite &TAnimation::getCurrentKeyFrame(TInt state) const
{
    TInt currentKeyFrameIndex = getCurrentKeyFrameIndex(state);
    return mKeyFrames[currentKeyFrameIndex];
}

TAnimation::TPlayMode TAnimation::getPlayMode() const
{
    return mPlayMode;
}

void TAnimation::setPlayMode(const TAnimation::TPlayMode &playMode)
{
    mPlayMode = playMode;
}

TInt TAnimation::getLastFrameIndex() const
{
    return mLastFrameIndex;
}

TInt TAnimation::getLastTimeState() const
{
    return mLastTimeState;
}
