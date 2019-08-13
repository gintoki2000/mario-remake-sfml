#ifndef BRICKBLOCK_H
#define BRICKBLOCK_H
#include "block.h"

class CBrickBlock : public CBlock
{
public:

    static CBrickBlock* create(const sf::Vector2f& position, CTile* tile);

    void update(TFloat deltaTime) override;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const override;

    void kicked(CPlayer* player) override;

private:
    CBrickBlock();
    TBoolean init(const sf::Vector2f& position, CTile* tile) override;
    std::string mSpawnItem;
    TFloat mKickedSpeed;
    TFloat mKickedValue;
};

#endif // BRICKBLOCK_H
