//
//  KnifeSprite.h
//  LunchFoodMaker
//
//  Created by huxixiang on 15/9/23.
//
//

#ifndef __LunchFoodMaker__KnifeSprite__
#define __LunchFoodMaker__KnifeSprite__

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class KnifeSpriteDelegate {
public:
    virtual void touchDown(){};
    virtual void touchMove(){};
    virtual void cut(){};
    virtual void touchEnd(){};
};

class KnifeSprite : public Sprite
{
public:
    KnifeSprite();
    ~KnifeSprite();
    
    static KnifeSprite* create(const char* pImage);
    virtual bool init(const char* pImage);
    
    void setDelegate(KnifeSpriteDelegate* delegate);
private:
    bool onKnifeTouchBegan(Touch *touch , Event *unusedEvent);
    void onKnifeTouchMove(Touch *touch , Event *unusedEvent);
    void onKnifeTouchEnd(Touch *touch , Event *unusedEvent);
    void onKnifeTouchCancel(Touch *touch , Event *unusedEvent);
    
    virtual void onEnter();
    virtual void onExit();
    
public:
    Vec2 getEndVec();
    void setEndVec(Vec2 vec, bool isOffset = true);
    void setLine(Vec2 vec);
    Vec2 getShadow(Vec2 from,Vec2 to);
    void setOriginalPoint(Vec2 vec, bool isOffset = true);
    Vec2 getOriginalPoint();
    
    void setLength(float length);
    void setSprtieRotate(float value);
    void setSpriteEnabled(bool isEnabled);
    void clearData();
    void setPosRate(float x = 1.0f, float y = 1.0f);
    void setFlag(bool flag);
    void setPosYMaxAndMin(float max, float min);
    void setResLength(float length);
private:
    KnifeSpriteDelegate* m_pDelegate;
    
    Vec2 m_oOriginal;
    
    Vec2 m_oStartPos;
    
    Vec2 toLine, endVec;
    
    bool m_bIsLock;
    
    bool m_bFlag;
    
    float m_fLength;
    
    float m_fRotateValue;
    
    float m_fRateX;
    float m_fRateY;
    
    float m_fMaxY;
    float m_fMinY;
    
    float m_fResLength;
    EventListenerTouchOneByOne *_touchListener;
};

#endif /* defined(__LunchFoodMaker__KnifeSprite__) */
