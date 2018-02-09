
#ifndef IAPItem_h
#define IAPItem_h

#include "cocos2d.h"

USING_NS_CC;

class IAPItem : public Node
{
public:
    IAPItem();
    ~IAPItem();
    
    CREATE_FUNC(IAPItem);
    
    virtual bool init();
    
    virtual bool ccTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void ccTouchMoved(Touch *pTouch, Event *pEvent);
    virtual void ccTouchEnded(Touch *pTouch, Event *pEvent);
    virtual void ccTouchCancelled(Touch *pTouch, Event *pEvent);
    
    Rect visibleWorldRect;
    void initWithName(const std::string contentPath,const std::string buyPath);
    std::function<void(int)>onItemClicked;
    void unlock();
protected:
    Sprite* m_pContentSrite;
    Sprite* m_pBuy;
};
#endif /* IAPItem_h */
