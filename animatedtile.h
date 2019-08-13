#ifndef ANIMATEDTILE_H
#define ANIMATEDTILE_H
#include "tile.h"
#include <vector>
#include <SFML/System/Clock.hpp>
class CAnimatedTile : public CTile
{
public:
    CAnimatedTile(const CAnimatedTile&) = delete;

    CAnimatedTile(const std::vector<TextureRegion*>& frames, const std::vector<TUInt32> intervals);

    const TextureRegion& getTextureRegion() const override;

    TUInt64 getCurrentFrameIndex() const;

    static void updateAnimationBaseTime();
private:
    std::vector<TextureRegion*> mFrames;
    std::vector<TUInt32> mIntervals;
    TInt mLoopDuration;

    static TInt sBaseTime;
    static sf::Clock sClock;
};

#endif // ANIMATEDTILE_H
