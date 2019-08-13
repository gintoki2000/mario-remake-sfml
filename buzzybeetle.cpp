#include "buzzybeetle.h"

CBuzzyBeetle *CBuzzyBeetle::create(CProperties* property)
{
    CBuzzyBeetle* self = new (std::nothrow) CBuzzyBeetle();
    if(self && self->init(property))
    {
        return self;
    }
    delete self;
    return nullptr;

}

CBuzzyBeetle::~CBuzzyBeetle()
{

}

void CBuzzyBeetle::update(TFloat deltaTime)
{

}

void CBuzzyBeetle::draw(sf::RenderTarget &render, sf::RenderStates states)
{

}

void CBuzzyBeetle::kickedByPlayer(CPlayer *player)
{

}

void CBuzzyBeetle::kickedFromUnderground()
{

}

void CBuzzyBeetle::killedByFireBall(CPlayerFireBall *fireBall)
{

}

void CBuzzyBeetle::killedByOther(CEnemy *other)
{

}

void CBuzzyBeetle::touchPlayer(CPlayer *player)
{

}

CBuzzyBeetle::CBuzzyBeetle()
{

}

TBoolean CBuzzyBeetle::init(CProperties* property)
{

}

void CBuzzyBeetle::setState(CBuzzyBeetle::TState state)
{
    if(mState == state) return;
    switch(state)
    {
        case ENormal:

        break;
    }
}
