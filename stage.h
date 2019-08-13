#ifndef STAGE_H
#define STAGE_H
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include "camera.h"
#include "base.h"
class CNode;
class CStage : public CObject
{
public:
    CStage(const CStage&) = delete;
    CStage& operator=(const CStage&) = delete;
    CStage();
    virtual ~CStage();

    virtual void draw(sf::RenderTarget& renderTarget) const;

    virtual void drawDebug(sf::RenderTarget& renderTarget) const;

    virtual void update(TFloat deltaTime);

    void setRoot(CNode* node);

    CNode* getRoot() const;

    Camera& getCamera();

    const Camera& getCamera() const;
private:
    CNode* mRoot;
    Camera mCamera;
};

#endif // STAGE_H
