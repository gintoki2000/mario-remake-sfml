#include "node.h"
#include <iostream>
#include "stage.h"
#include <climits>
CNode::CNode():
    mParent(nullptr),
    mStage(nullptr),
    mIsVisible(true),
    mIsActive(true)
{

}

CNode::~CNode()
{
    for(const auto& child : mChildren)
    {
        delete child;
    }
    mChildren.clear();

    deleteRemovedChildren();
}

TBoolean CNode::init()
{
    return true;
}

void CNode::update(TFloat deltaTime)
{
    if(isActive())
    {
        for(const auto& child : captureChildren())
        {
            if(child->isActive())
            {
                child->update(deltaTime);
            }
        }
    }
    deleteRemovedChildren();
}

void CNode::draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates)
{
    if(isVisible())
    {
        renderStates.transform *= getTransform();
        for(const auto& child : mChildren)
        {
            if(child->isVisible())
            {
                child->draw(renderTarget, renderStates);
            }
        }
    }
}

void CNode::drawDebug(sf::RenderTarget &renderTarget, sf::RenderStates renderStates)
{
    renderStates.transform*=getTransform();
    for(CNode* child : mChildren)
    {
        child->drawDebug(renderTarget, renderStates);
    }
}

void CNode::attachChild(CNode *node)
{
    if(!node) return;
    if(node->getParent() == this) return;
    if(node->hasParent())
    {
        CNode* np = node->getParent();
        np->deattachChild(node);
    }

    mChildren.push_back(node);
    node->setStage(getStage());
    node->setParent(this);
}

TBoolean CNode::deattachChild(CNode *node)
{
    auto found = std::find(mChildren.begin(), mChildren.end(), node);
    if(found == mChildren.end()) return false;
    (*found)->setParent(nullptr);
    (*found)->setStage(nullptr);
    mChildren.erase(found);
    return true;
}

TBoolean CNode::removeChild(CNode *node)
{
    auto found = std::find(mChildren.begin(), mChildren.end(), node);
    if(found == mChildren.end()) return false;
    mRemovedChildren.push_back(*found);
    mChildren.erase(found);
    return true;
}

void CNode::remove()
{
    if(hasParent())
        getParent()->removeChild(this);
}

std::vector<CNode *> CNode::captureChildren() const
{
    return mChildren;
}

const std::vector<CNode *>& CNode::getChildren() const
{
    return mChildren;
}

void CNode::setParent(CNode *node)
{
    mParent = node;
}

CNode *CNode::getParent() const
{
    return mParent;
}

TBoolean CNode::hasParent() const
{
    return mParent != nullptr;
}

void CNode::setStage(CStage *stage)
{
    mStage = stage;
}

CStage *CNode::getStage() const
{
    return mStage;
}

void CNode::show()
{
    setVisible(true);
}

void CNode::hide()
{
    setVisible(false);
}

void CNode::setVisible(TBoolean visible)
{
    mIsVisible = visible;
}

TBoolean CNode::isVisible() const
{
    return mIsVisible;
}

void CNode::activate()
{
    setActive(true);
}

void CNode::deactivate()
{
    setActive(false);
}

void CNode::setActive(TBoolean active)
{
    mIsActive = active;
}

TBoolean CNode::isActive() const
{
    return mIsActive;
}

void CNode::clear()
{
    for(const auto& child : mChildren)
    {
        mRemovedChildren.push_back(child);
    }
    mChildren.clear();
}

void CNode::setName(const std::string &name)
{
    mName = name;
}

const std::string &CNode::getName() const
{
    return mName;
}

TInt CNode::getZIndex() const
{
    if(hasParent())
    {
        const auto& childrenOfParent = getParent()->getChildren();
        auto position = std::find(childrenOfParent.begin(), childrenOfParent.end(), this);
        return static_cast<TInt>(position - childrenOfParent.begin());
    }
    return -1;
}

TBoolean CNode::setZIndex(TInt newZIndex)
{
    if(newZIndex < 0) return false;
    if(!hasParent()) return false;
    auto& children = getParent()->mChildren;
    newZIndex = std::min<TInt>(newZIndex, children.size() - 1);
    auto currentPosition = std::find(children.begin(), children.end(), this);
    if((currentPosition - children.begin()) == newZIndex) return false;
    children.erase(currentPosition);
    children.insert(children.begin() + newZIndex, this);
    return true;
}

void CNode::toFront()
{
    setZIndex(0);
}

void CNode::toBack()
{
    setZIndex(INT32_MAX);
}

CNode *CNode::findNodeByName(const std::string &name)
{
    for(const auto& child : mChildren)
    {
        if(child->getName() == name)
            return child;
    }

    for (const auto& child : mChildren)
    {
        auto found = child->findNodeByName(name);
        if(found != nullptr) return found;
    }

    return nullptr;
}

sf::Vector2f CNode::localToParentCoords(const sf::Vector2f &localCoords)
{
    return getTransform().transformPoint(localCoords);
}

sf::Vector2f CNode::parentToLocalCoords(const sf::Vector2f &parentCoords)
{
    return getInverseTransform().transformPoint(parentCoords);
}

sf::Vector2f CNode::localToAscendantCoords(CNode *ascendant, const sf::Vector2f &localCoords)
{
   CNode* node = this;
   sf::Vector2f ascendantCoords = localCoords;
   do{
       ascendantCoords = node->localToParentCoords(ascendantCoords);
       node = node->getParent();
       if(node == ascendant) break;
   }while(node != nullptr);
   return ascendantCoords;
}

sf::Vector2f CNode::localToStageCoords(const sf::Vector2f& localCoords)
{
    return localToAscendantCoords(nullptr, localCoords);
}

sf::Vector2f CNode::stageToLocalCoords(const sf::Vector2f &stageCoords)
{
    return parentToLocalCoords(hasParent() ? getParent()->stageToLocalCoords(stageCoords) : stageCoords);
}

sf::Vector2f CNode::localToNodeCoords(CNode *node, const sf::Vector2f &localCoords)
{
    return node->stageToLocalCoords(localToStageCoords(localCoords));
}

void CNode::deleteRemovedChildren()
{
    for(const auto& removedChild : mRemovedChildren)
    {
        delete removedChild;
    }
    mRemovedChildren.clear();
}












