#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "node.h"
#include "utils.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "property.h"
//mario game objects
enum TActivateDistance
{
    EAlwaysActive = -1,
    EFiveMeter = 32 * 5,
    ETenMeter = 32 * 10,
    EFifitTeenMeter = 32 * 15,
    ETwentyMeter = 32 * 20,
    ETwentyFiveMeter = 32 * 25
};
class CGameObject : public CNode
{
public:



    virtual void drawDebug(sf::RenderTarget& renderTarget, sf::RenderStates renderStates);

    void setSize(const sf::Vector2f& size);

    void setSize(TFloat width, TFloat height);

    void setWidth(TFloat width);

    void setHeight(TFloat height);

    void setX(TFloat x);

    void setY(TFloat y);

    const TFloat& getWidth() const;

    const TFloat& getHeight() const;

    const TFloat& getX() const;

    const TFloat& getY() const ;

    const sf::Vector2f& getSize() const;

    void setBounds(const sf::FloatRect& bounds);

    sf::FloatRect getBounds() const;

    const TInt& getActiveDis() const;

    void setActiveDis(TInt activeDis);

    void setDebugColor(const sf::Color& debugColor);

    const sf::Color& getDebugColor() const;

    //void setSpeed(const sf::Vector2f& speed);

    //void setSpeedX(const TFloat& speedX);

    //void setSpeedY(const TFloat& speedY);

    //void addImpluse(const sf::Vector2f& impluse);

    //const sf::Vector2f& getSpeed() const;

    //const TFloat& getSpeedX() const;

    //const TFloat& getSpeedY() const;


protected:
    CGameObject();
    virtual TBoolean init(CProperties* properties);
private:

    sf::Vector2f mSize;

    sf::Vector2f mSpeed;

    TInt mActiveDis;

    TFloat mGravityScale;

    sf::Color mDebugColor;

    TBoolean mIsRigid;


    static TFloat KGravityAcceleration;
    static sf::RectangleShape sDebugRect;
    static sf::Text sDebugText;
};



#endif // GAMEOBJECT_H
