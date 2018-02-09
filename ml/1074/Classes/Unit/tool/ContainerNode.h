
#ifndef __ContainerNode__
#define __ContainerNode__


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "DragNode.h"

USING_NS_CC;
using namespace cocos2d::ui;

class ContainerNode: public Node
{
public:
    
    static ContainerNode* create(Node* pParent);
    virtual bool init(Node* pParent);
    
    void addBody(std::string body);
    void addFront(std::string front,std::string name = "front",Vec2 pos = Vec2::ZERO);
    void addShadow(std::string shadow,Vec2 pos = Vec2::ZERO);
    void showShadow(bool show = true);
    
    void addFood(std::string food,std::string mask = "",Vec2 pos = Vec2::ZERO);
    void addFoodShadow(std::string shadow,Vec2 pos = Vec2::ZERO);
    void pourFood(bool left = true,float dt = 2);
    
    Sprite* getFood(){
        return m_pFood;
    }
    void reset(){
        for(auto pNode:m_allChildren) {
            pNode->removeFromParent();
        }
        m_pFood = nullptr;
        m_pClippingNode = nullptr;
        m_allChildren.clear();
    }
protected:
    ContainerNode();
    ~ContainerNode();
    
protected:
    Node* m_pParent;
    ClippingNode* m_pClippingNode;
    Sprite* m_pFood;
    Vector<Node*> m_allChildren;
};


#endif