#ifndef PORTAL_H
#define PORTAL_H
#include "gameobject.h"
#include "property.h"
class CPortal : public CGameObject
{
public:
    enum class TType {EEnter,EExit};
    static CPortal* create(CProperties* properties);

    ~CPortal();

    void update(TFloat);

    void draw(sf::RenderTarget&, sf::RenderStates);

    TInt getDirection() const;

    const TString& getWorld() const;

    TType getType() const;

    const TString& getSpawningPortal() const;

private:
    CPortal();
    TBoolean init(CProperties* properties);

    TInt mDirection;

    TString mWorld;

    TType mType;

    TString mSpawningPortal;

};

#endif // PORTAL_H
