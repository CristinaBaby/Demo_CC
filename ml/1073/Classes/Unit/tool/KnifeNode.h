
#ifndef __KnifeNode__
#define __KnifeNode__


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "DragNode.h"

USING_NS_CC;
using namespace cocos2d::ui;

class KnifeNode: public Node
{
public:
    
    CREATE_FUNC(KnifeNode);
    virtual bool init();
    
    
    void cut(Vec2 center,float raduis);
    
    void startWork();
    void stopWork();
protected:
    KnifeNode();
    ~KnifeNode();
    
protected:
    ClippingNode* m_pClippingNode;
    Sprite* m_pBladeBack;
    Sprite* m_pBlade;
    Sprite* m_pHandle;
};


#endif