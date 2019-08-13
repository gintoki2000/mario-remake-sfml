#include "gameobject.h"
#include "fontmanager.h"
#include <SFML/Graphics/RenderTarget.hpp>
sf::RectangleShape CGameObject::sDebugRect = sf::RectangleShape();
sf::Text CGameObject::sDebugText = sf::Text();
CGameObject::CGameObject():
    mActiveDis(EAlwaysActive),
    mDebugColor(sf::Color(255, 255, 255, 255)),
    mGravityScale(0.f),
    mIsRigid(true)
{
}

TBoolean CGameObject::init(CProperties *properties)
{
    setX(properties->get<CFloat>("x")->value());
    setY(properties->get<CFloat>("y")->value());
    setWidth(properties->get<CFloat>("width")->value());
    setHeight(properties->get<CFloat>("height")->value());
    return true;
}

void CGameObject::drawDebug(sf::RenderTarget &renderTarget, sf::RenderStates renderStates)
{
    sDebugRect.setSize(getSize());
    sDebugRect.setPosition(getPosition());
    sDebugRect.setOutlineThickness(1.f);
    sDebugRect.setOutlineColor(getDebugColor());

    sf::Color c(getDebugColor().r, getDebugColor().g, getDebugColor().b, 255 / 4);
    sDebugRect.setFillColor(c);
    renderTarget.draw(sDebugRect);

    static sf::Font* font = FontManager::get(EUbuntuRegularFont);
    sDebugText.setFont(*font);
    sDebugText.setFillColor(sf::Color::White);
    sDebugText.setString(getName());
    sDebugText.setPosition(getPosition());
    sDebugText.setCharacterSize(14);
    const sf::FloatRect& tBounds = sDebugText.getLocalBounds();
    sDebugText.setStyle(sf::Text::Bold);
    sDebugText.move(0.f - tBounds.width / 2.f + getWidth() /2.f, -tBounds.height - 15);
    renderTarget.draw(sDebugText);
}



void CGameObject::setBounds(const sf::FloatRect &bounds)
{
    setPosition(bounds.left, bounds.top);
    setSize(bounds.width, bounds.height);
}

sf::FloatRect CGameObject::getBounds() const
{
    return sf::FloatRect(getPosition(), getSize());
}

const TInt& CGameObject::getActiveDis() const
{
    return mActiveDis;
}

void CGameObject::setActiveDis(TInt activeDis)
{
    mActiveDis = activeDis;
}

void CGameObject::setDebugColor(const sf::Color &debugColor)
{
    mDebugColor = debugColor;
}

const sf::Color& CGameObject::getDebugColor() const
{
    return mDebugColor;
}

void CGameObject::setSize(const sf::Vector2f &size)
{
    move(mSize - size);
    mSize = size;
}

void CGameObject::setSize(TFloat width, TFloat height)
{
    mSize.x = width;
    mSize.y = height;
}

void CGameObject::setWidth(TFloat width)
{
    mSize.x = width;
}

void CGameObject::setHeight(TFloat height)
{
    mSize.y = height;
}

void CGameObject::setX(TFloat x)
{
    setPosition(x, getY());
}

void CGameObject::setY(TFloat y)
{
    setPosition(getX(), y);
}

const TFloat& CGameObject::getWidth() const
{
    return getSize().x;
}

const TFloat& CGameObject::getHeight() const
{
    return getSize().y;
}

const TFloat& CGameObject::getX() const
{
    return getPosition().x;
}

const TFloat& CGameObject::getY() const
{
    return  getPosition().y;
}

const sf::Vector2f &CGameObject::getSize() const
{
    return mSize;
}
