#ifndef BUZZYBEETLE_H
#define BUZZYBEETLE_H
#include "enemy.h"
#include "texturemanager.h"
#include "soundmanager.h"
#include "animation.h"
class CBuzzyBeetle : public CEnemy
{
private:
    enum TState
    {
        ENone,
        ENormal,
        EHidingInShell,
        ECrazyShell,
        EDead
    };
public:
    static CBuzzyBeetle* create(CProperties* property);

    ~CBuzzyBeetle();

    void update(TFloat deltaTime) override;

    void draw(sf::RenderTarget& render, sf::RenderStates states) override;

    void kickedByPlayer(CPlayer* player) override;

    void kickedFromUnderground() override;

    void killedByFireBall(CPlayerFireBall* fireBall) override;

    void killedByOther(CEnemy* other) override;

    void touchPlayer(CPlayer* player) override;
private:
    CBuzzyBeetle();

    virtual bool init(CProperties* property) override;

    void setState(TState state);

    TState mState;

    TAnimation mAnim;

    TDirection mDir;

    const static TFloat KWalkingVel;
    const static TFloat KRunCrazyVel;

};

#endif // BUZZYBEETLE_H
