
#ifndef __MakeEatLayer__
#define __MakeEatLayer__

#include <iostream>
#include "cocos2d.h"
#include "CCImageColorSpace.h"
#include "MyRenderTexture.h"

USING_NS_CC;

class MakeEatLayer:public Layer {
    
    
public:
    
    CREATE_FUNC(MakeEatLayer);
    virtual bool init();
    virtual void onEnter();
    
    void setRenderTexture(RenderTexture* rt){
        if (m_pRenderTexture) {
            m_pRenderTexture->release();
        }
        m_pRenderTexture = (MyRenderTexture*)rt;
        m_pRenderTexture->retain();
        m_pRenderTexture->setPosition(Vec2(m_pRenderTexture->getSprite()->getContentSize().width*0.5, m_pRenderTexture->getSprite()->getContentSize().height*0.5));
        addChild(m_pRenderTexture);
        __NotificationCenter::getInstance()->removeObserver(m_pRenderTexture, EVENT_COME_TO_BACKGROUND);
        __NotificationCenter::getInstance()->removeObserver(m_pRenderTexture, EVENT_COME_TO_FOREGROUND);};
    
    void setRenderTextureInner(RenderTexture* rt){
        if (m_pRenderTextureInner) {
            m_pRenderTextureInner->release();
        }
        m_pRenderTextureInner = (MyRenderTexture*)rt;
        m_pRenderTextureInner->retain();
        m_pRenderTextureInner->setPosition(Vec2(m_pRenderTextureInner->getSprite()->getContentSize().width*0.5, m_pRenderTextureInner->getSprite()->getContentSize().height*0.5));
        addChild(m_pRenderTextureInner);
        m_pRenderTextureInner->setLocalZOrder(-1);
        __NotificationCenter::getInstance()->removeObserver(m_pRenderTextureInner, EVENT_COME_TO_BACKGROUND);
        __NotificationCenter::getInstance()->removeObserver(m_pRenderTextureInner, EVENT_COME_TO_FOREGROUND);};
    
    void setShapeTexture(Texture2D* texture){m_pShapeSprite->setTexture(texture);};
    
    void setDelegate(Ref* obj){m_pDelegate = obj;}
    void setSelector(SEL_CallFunc sel){m_pFunc = sel;}
    
    void setAlphaPercent(){m_nAlphaPercent = 90;}
    int getPercent(){return m_nCurPercent;}
    void doEat(Vec2 point);
protected:
    MakeEatLayer():m_pRenderTexture(NULL),m_pRenderTextureInner(NULL),m_pShapeSprite(NULL),m_pDelegate(NULL),m_pFunc(NULL){
        m_pShapeSprite = Sprite::create("content/eatShape.png");
        m_pShapeSprite->retain();
    };
    ~MakeEatLayer(){
        m_pShapeSprite->release();
        if (m_pRenderTexture) {
            m_pRenderTexture->release();
        }
        if (m_pRenderTextureInner) {
            m_pRenderTextureInner->release();
        }
    }
    MyRenderTexture* m_pRenderTexture;
    MyRenderTexture* m_pRenderTextureInner;
    Sprite* m_pShapeSprite;
    
    Ref* m_pDelegate;
    SEL_CallFunc m_pFunc;
    
    int m_nAlphaPercent;
    int m_nCurPercent;
    int m_nEatCount;
};

#endif /* defined(__MakeEatLayer__) */
