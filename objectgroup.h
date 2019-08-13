#ifndef OBJECTGROUP_H
#define OBJECTGROUP_H
#include "node.h"

class CObjectGroup : public CNode
{
public:
    CObjectGroup();
    ~CObjectGroup();
    void update(TFloat deltaTime) override;

    //void draw(sf::RenderTarget& renderTarget, sf::RenderStates) override;
};

#endif // OBJECTGROUP_H
