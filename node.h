#ifndef NODE_H
#define NODE_H
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <vector>
#include <string>
#include "base.h"
class CStage;
class CNode : public sf::Transformable, public CObject
{
public:
    CNode(const CNode&) = delete;
    CNode& operator=(const CNode&) = delete;

    CNode();

    virtual ~CNode();

    virtual void update(TFloat deltaTime);

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates);

    virtual void drawDebug(sf::RenderTarget& renderTarget, sf::RenderStates renderStates);

    void attachChild(CNode* node);

    //deattach child
    TBoolean deattachChild(CNode* node);


    TBoolean removeChild(CNode* node);

    void remove();

    std::vector<CNode*> captureChildren() const;

    const std::vector<CNode*>& getChildren() const;


    void setParent(CNode* node);

    CNode* getParent() const;

    TBoolean hasParent() const;

    void setStage(CStage* stage);

    CStage* getStage() const;

    void show();

    void hide();

    void setVisible(TBoolean visible);

    TBoolean isVisible() const;

    void activate();

    void deactivate();

    void setActive(TBoolean active);

    TBoolean isActive() const;

    void clear();

    void setName(const std::string& name);

    const std::string& getName() const;

    TInt getZIndex() const;

    TBoolean setZIndex(TInt newZIndex);

    void toFront();

    void toBack();

    template<typename T>
    TBoolean isTypeOf()
    {
        return dynamic_cast<T*>(this) != nullptr;
    }

    template<typename T>
    CNode* findNodeByType()
    {
        for(const auto& child : mChildren)
        {
            if(child->isTypeOf<T>()) return child;
        }

        for(const auto& child : mChildren)
        {
            auto found = child->findNodeByType<T>();
            if(found != nullptr) return found;
        }

        return nullptr;
    }

    template<typename T>
    void getNodesByType(std::vector<CNode*> tmp)
    {
        for(const auto& child : mChildren)
        {
            if(child->isTypeOf<T>()) tmp.push_back(child);
            child->findNodeByType<T>(tmp);
        }
    }

    CNode* findNodeByName(const std::string& name);

    sf::Vector2f localToParentCoords(const sf::Vector2f& localCoords);

    sf::Vector2f parentToLocalCoords(const sf::Vector2f& parentCoords);

    sf::Vector2f localToAscendantCoords(CNode* ascendant, const sf::Vector2f& localCoords);

    sf::Vector2f localToStageCoords(const sf::Vector2f& localCoords);

    sf::Vector2f stageToLocalCoords(const sf::Vector2f& stageCoords);

    sf::Vector2f localToNodeCoords(CNode* node, const sf::Vector2f& localCoords);

protected:
    virtual TBoolean init();
    void deleteRemovedChildren();
private:

    CNode* mParent;
    CStage* mStage;
    std::vector<CNode*> mChildren;
    std::vector<CNode*> mRemovedChildren;
    TBoolean mIsVisible;
    TBoolean mIsActive;
    std::string mName;


};

#endif // NODE_H
