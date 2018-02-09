
#ifndef SieveNode_h
#define SieveNode_h

#include "cocos2d.h"

USING_NS_CC;


class SieveNode : public Node
{
public:
    SieveNode();
    ~SieveNode();
    
    CREATE_FUNC(SieveNode);
    
    virtual bool init();
    virtual void onEnter();
    
    void setSieve(std::string bodyPath,std::string frontPath);
    
    void setPowder(std::string path,float dt = 0.0f);
    void powderFadeOut(float dt = 1);
    void separatePowder();
    void stopSeparate();
    
    void setPowderRect(Rect rect){
        m_powderRect = rect;
    }
    Rect getWorldRect(){
        Rect rect = m_pBody->getBoundingBox();
        rect.origin = this->convertToWorldSpace(rect.origin);
        return rect;
    }
protected:
    Sprite* m_pBody;
    Sprite* m_pBodyFront;
    Sprite* m_pPowder;
    Vec2 m_origngPos;
    Rect m_powderRect;
    bool m_bIsWorking;
};
#endif /* SieveNode_h */
