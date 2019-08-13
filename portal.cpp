#include "portal.h"

CPortal *CPortal::create(CProperties *properties)
{
    CPortal* self = new (std::nothrow) CPortal;
    if(self && self->init(properties))
    {
        return self;
    }
    delete self;
    return nullptr;
}

CPortal::~CPortal()
{

}

void CPortal::update(TFloat)
{

}

void CPortal::draw(sf::RenderTarget &, sf::RenderStates)
{

}

CPortal::CPortal()
{

}

TBoolean CPortal::init(CProperties *properties)
{
    CGameObject::init(properties);
    const CString* direcion = properties->get<CString>("Direction");
    const CString* type = properties->get<CString>("Type");
    const CString* world = properties->get<CString>("World");
    const CString* spawningPortal = properties->get<CString>("SpawningPortal");
    if(!direcion || !type || !world)
    {
        std::cout<<"could not init protal object!"<<std::endl;
        return false;
    }
    if(spawningPortal)
    {
        mSpawningPortal = spawningPortal->value();
        std::cout<<mSpawningPortal<<std::endl;
    }
    mWorld = world->value();
    mType = type->value() == "enter" ? TType::EEnter : TType::EExit;
    if(direcion->value() == "up")
    {
        mDirection = EUp;
    }
    else if(direcion->value() == "down")
    {
        mDirection = EDown;
    }
    else if(direcion->value() == "left")
    {
        mDirection = ELeft;
    }
    else if(direcion->value() == "right")
    {
        mDirection = ERight;
    }
    else {
        std::cout<<"Direction="<<direcion->value()<<"... what??"<<std::endl;
        return false;
    }
    return true;
}

const TString& CPortal::getSpawningPortal() const
{
    return mSpawningPortal;
}

CPortal::TType CPortal::getType() const
{
    return mType;
}

const TString& CPortal::getWorld() const
{
    return mWorld;
}

TInt CPortal::getDirection() const
{
    return mDirection;
}
