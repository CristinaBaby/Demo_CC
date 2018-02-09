//
//  STSlider.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-25.
//
//

#include "STSlider.h"

USING_NS_CC;

STSlider* STSlider::create(Sprite * backgroundSprite, Sprite* pogressSprite, Sprite* thumbSprite)
{
    return create(backgroundSprite, pogressSprite, thumbSprite, NULL, NULL);
}

STSlider* STSlider::create(Sprite * backgroundSprite, Sprite* pogressSprite, Sprite* thumbSprite,
                 Ref* target, SEL_CallFuncN selector)
{
    STSlider *pRet = new STSlider();
    pRet->initWithSprites(backgroundSprite, pogressSprite, thumbSprite);
    pRet->setTarget(target, selector);
    pRet->autorelease();
    return pRet;
}

void STSlider::setTarget(Ref *rec, SEL_CallFuncN selector)
{
    m_pListener = rec;
    m_pfnSelector = selector;
}

// 0   144
void STSlider::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    ControlSlider::onTouchEnded(pTouch, pEvent);
    
    Vec2 location = locationFromTouch(pTouch);
    
    int value = (int)valueForLocation(location);
// log("value = %d" , value);
    if (value < 11) {
        setValue(13);
        if (m_pListener && m_pfnSelector)
        {
            (m_pListener->*m_pfnSelector)(this);
        }
        return;
    }else if(value > 68){
        setValue(65);
        if (m_pListener && m_pfnSelector)
        {
            (m_pListener->*m_pfnSelector)(this);
        }
        return;
    }
    
    int a[] = { (value - 0) ,(value -13),(value - 26),(value - 39),(value - 52),(value - 65),(value - 78)};
    for(int i =0 ;i < 7-1 ; i++){

        if(abs(a[i]) < abs(a[i+1])){
    
            int temp = a[i+1];
            a[i+1] = a[i];
            a[i] = temp;
    
        }
    }
    
    setValue(value - a[6]);
    
    if (m_pListener && m_pfnSelector)
    {
        (m_pListener->*m_pfnSelector)(this);
    }
}
float lastNumber;
void STSlider::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    Vec2 location = locationFromTouch(pTouch);
      int value = (int)valueForLocation(location);
    setValue(value);
    
    int mmm = value/13;
    int nnn = value%13;
    if (nnn>6) {
        mmm++;
    }
    if (lastNumber!=mmm && value > 19  && value<66 ) {
        if (m_pListener && m_pfnSelector)
        {
            (m_pListener->*m_pfnSelector)(this);
        }
    }
    
    
//     log("value = %d" , value);
    if(value > 5 && value % 13 == 0 && value<66){
       
        setValue(value);
        if (m_pListener && m_pfnSelector)
        {
            (m_pListener->*m_pfnSelector)(this);
        }
    }
    
    if (value < 11) {
        setValue(13);
        if (m_pListener && m_pfnSelector)
        {
            (m_pListener->*m_pfnSelector)(this);
        }
        return;
    }else if(value > 68){
        setValue(65);
        if (m_pListener && m_pfnSelector)
        {
            (m_pListener->*m_pfnSelector)(this);
        }
        return;
    }
    
    
    
}
//是否为某自然数的平方
int STSlider::square(int n){
    
    int i=1;
    n=n-i;
    while(n>0){
        i+=2;
        n-=i;
    }
    if(n==0)
        return 1;
    else
        return 0;
}

STSlider::STSlider() : m_pListener(NULL), m_pfnSelector(NULL)
{
}

STSlider::~STSlider()
{
}

