#include "stage.h"
#include "node.h"
#include <stdexcept>
CStage::CStage():
    mRoot(new CNode())
{
    getRoot()->setStage(this);
}

CStage::~CStage()
{
    delete mRoot;
}

void CStage::draw(sf::RenderTarget &renderTarget) const
{
    if(getRoot()->isVisible())
    {
        const sf::View& oldView = renderTarget.getView();
        renderTarget.setView(getCamera());
        getRoot()->draw(renderTarget, sf::RenderStates::Default);
        renderTarget.setView(oldView);
    }
}

void CStage::drawDebug(sf::RenderTarget &renderTarget) const
{
    getRoot()->drawDebug(renderTarget, sf::RenderStates::Default);
}

void CStage::update(TFloat deltaTime)
{
    if(getRoot()->isActive())
    {
        getRoot()->update(deltaTime);
    }
}

void CStage::setRoot(CNode *node)
{
    if(!node) throw std::invalid_argument("\'node\' argument can not be nullptr! in function CStage::setRoot(CNode *node)");
    if(node->hasParent())
    {
        node->getParent()->deattachChild(node);
    }
    delete mRoot;
    mRoot = node;
}

CNode *CStage::getRoot() const
{
    return mRoot;
}

Camera &CStage::getCamera()
{
    return mCamera;
}

const Camera &CStage::getCamera() const
{
    return mCamera;
}
