#ifndef ANIMATION_H
#define ANIMATION_H
#include <SFML/Graphics/Sprite.hpp>
#include <vector>
#include <cstdint>
#include "base.h"
class TAnimation
{
public:
    enum class TPlayMode
    {
        ENormal, EReversed, ELoop, ELoopReversed, ELoopPingpong
    };

    TAnimation();

    TAnimation(TInt frameDuration, const std::vector<sf::Sprite>& keyFrames, TPlayMode playMode = TPlayMode::ENormal);

    void create(sf::Texture* source, const std::vector<sf::IntRect>& rects);

    void create(sf::Texture* source, const sf::IntRect& rect);

    void create(sf::Texture* source, TInt startX, TInt startY, TInt tileWidth, TInt tileHeight, TInt cols, TInt rows);

    const std::vector<sf::Sprite>& getKeyFrames() const;

    void setKeyFrames(const std::vector<sf::Sprite> &getKeyFrames);

    TInt getFrameDuration() const;

    void setFrameDuration(TInt frameDuration);

    TInt getCurrentKeyFrameIndex(TInt state) const;

    const sf::Sprite& getCurrentKeyFrame(TInt state) const;

    TPlayMode getPlayMode() const;

    void setPlayMode(const TPlayMode &playMode);

    TInt getLastFrameIndex() const;

    TInt getLastTimeState() const;

private:
    std::vector<sf::Sprite> mKeyFrames;
    TPlayMode mPlayMode;
    TInt mFrameDuration;
    TInt mLastFrameIndex;
    TInt mLastTimeState;

};

#endif // ANIMATION_H
