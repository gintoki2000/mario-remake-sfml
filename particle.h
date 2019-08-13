#ifndef PARTICLE_H
#define PARTICLE_H
#include "node.h"
#include "timer.h"
class CParticle : public CNode
{
protected:
    CParticle();
    virtual TBoolean init(int time);
public:
    virtual void update(TFloat deltaTime) override;

    TInt getFramesRemaining() const;

    TInt getFramesEslapsed() const;

private:
    TTimer mTimer;

};

#endif // PARTICLE_H
