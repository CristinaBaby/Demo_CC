
#ifndef __EatLayer__
#define __EatLayer__


#include "cocos2d.h"

USING_NS_CC;

class EatLayer : public Layer
{
public:
    EatLayer();
    ~EatLayer();
    
    CREATE_FUNC(EatLayer);
    
    virtual bool init();
    
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);
    
    void setDrinkable(bool enable){m_bCanDrink = enable;}
    void setDrinkRect(Rect rect){m_DrinkRect = rect;}
    
    std::function<void(Vec2 location,bool isDrink)>onEatBeginCallback;
    std::function<void(Vec2 location)>onEatEndCallback;
protected:
    bool m_bCanDrink;
    Rect m_DrinkRect;
};

#endif