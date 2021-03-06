
#ifndef __KettleNode__
#define __KettleNode__


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ContainerNode.h"

USING_NS_CC;
using namespace cocos2d::ui;

class KettleNode: public ContainerNode
{
public:
    
    static KettleNode* create(Node* pParent);
    virtual bool init(Node* pParent);
    
    void addWater(std::string water,std::string mask,Vec2 pos = Vec2::ZERO);
    void addPourWater(std::string water);
    
    void pourWater(float dt = 2);
    void waveWater(int wave); //0 repeatforever  1以后就是wave个数
    
protected:
    KettleNode();
    ~KettleNode();
    
protected:
    NodeGrid* m_pGridNode;
    Sprite* m_pWater;
    std::string m_pathWater;
    std::string m_pathPourWater;
};


#endif