#ifndef BLOCK_H
#define BLOCK_H
#include <SFML/Graphics/Drawable.hpp>
#include "property.h"

class CTile;
class CTiledMap;
class CPlayer;
class CBlock : public sf::Drawable, public CObject
{
public:

    CBlock(const CBlock&) = delete;

    CBlock& operator=(const CBlock&) = delete;


    static CBlock* create(const sf::Vector2f& position, CTile* tile);

    virtual ~CBlock();

    void setParent(CTiledMap* map);

    CTiledMap* getParent() const;

    void setTile(CTile* tile);

    CTile* getTile() const;

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const override;

    virtual void update(TFloat deltaTime) { }

    virtual void kicked(CPlayer* player) { }

    void setRigid(TBoolean isRigid);

    TBoolean isRigid() const;

    const sf::Vector2f& getPosition() const;

    virtual sf::FloatRect getBounds() const;

    TBoolean isDestroyed() const;

    void destroy();

    void kickEnemyAbove();
protected:
    CBlock();

    virtual TBoolean init(const sf::Vector2f& position, CTile* tile);
private:

    CTile* mTile;


    CTiledMap* mParent;

    sf::Vector2f mPosition;

    TBoolean mIsRigid;

    TBoolean mIsDestroyed;

};

#endif // BLOCK_H
