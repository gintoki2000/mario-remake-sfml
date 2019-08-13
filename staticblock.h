#ifndef STATICBLOCK_H
#define STATICBLOCK_H
#include "block.h"

class CStaticBlock : public CBlock
{
public:
    static CStaticBlock* create(const sf::Vector2f& position, CTile* tile);
private:
    CStaticBlock();
    TBoolean init(const sf::Vector2f& position, CTile* tile) override;
};

#endif // STATICBLOCK_H
