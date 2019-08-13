#ifndef QUESTIONBLOCK_H
#define QUESTIONBLOCK_H
#include "soundmanager.h"
#include "block.h"
class CQuestionBlock : public CBlock
{
public:
    CQuestionBlock(const CQuestionBlock&) = delete;

    static CQuestionBlock* create(const sf::Vector2f& position, CTile* tile);

    void update(TFloat deltaTime) override;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const override;

    void kicked(CPlayer* player) override;

    void setSpawnItemType(const std::string &spawnItem);

    const std::string& getSpawnItemType() const;
private:
    CQuestionBlock();
    TBoolean init(const sf::Vector2f& position, CTile* tile) override;
    TBoolean mIsSpawned;
    std::string mSpawnItem;
    TFloat mKickedSpeed;
    TFloat mKickedValue;
};

#endif // QUESTIONBLOCK_H
