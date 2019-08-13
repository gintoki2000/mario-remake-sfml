#include "particle.h"

CParticle::CParticle()
{

}

TBoolean CParticle::init(TInt time)
{
    mTimer = TTimer(time);
    mTimer.start();
    return true;
}

void CParticle::update(TFloat deltaTime)
{
    if(!mTimer.tick()) remove();
}

TInt CParticle::getFramesRemaining() const
{
    return mTimer.getFramesRemaining();
}

TInt CParticle::getFramesEslapsed() const
{
    return mTimer.getFramesEslapsed();
}
