
#include "MakeEatLayer.h"
#include "Heads.h"

bool MakeEatLayer::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    m_nAlphaPercent = 100;
    m_nCurPercent = 0;
    m_nEatCount = 0;
    return true;
}

void MakeEatLayer::onEnter()
{
    CCLayer::onEnter();
    
}

void MakeEatLayer::doEat(Vec2 point)
{
    if (m_pRenderTexture&&m_pShapeSprite) {
        m_pShapeSprite->setBlendFunc((ccBlendFunc){GL_ZERO,GL_ONE_MINUS_SRC_ALPHA});
//        point = m_pRenderTexture->convertToNodeSpace(point);
//        point = ccpAdd(point, Vec2(m_pRenderTexture->getSprite()->getContentSize().width*0.5, m_pRenderTexture->getSprite()->getContentSize().height*0.5));
        Vec2 pos = m_pRenderTexture->getParent()->convertToNodeSpace(point);
        log("====%f,%f,%f,%f",m_pRenderTexture->getContentSize().width,m_pRenderTexture->getContentSize().height,pos.x,pos.y);
        m_pShapeSprite->setPosition(pos);
        m_pRenderTexture->begin();
        m_pShapeSprite->visit();
        m_pRenderTexture->end();
        Director::getInstance()->getRenderer()->render();
        
        __NotificationCenter::getInstance()->removeObserver(m_pRenderTexture, EVENT_COME_TO_BACKGROUND);
        __NotificationCenter::getInstance()->removeObserver(m_pRenderTexture, EVENT_COME_TO_FOREGROUND);
        
        m_nEatCount++;
        if (m_nEatCount%5==0) {
            m_nCurPercent = m_pRenderTexture->getPercentageTransparent();
        }
        if (m_pDelegate&& m_pFunc) {
            (m_pDelegate->*m_pFunc)();
        }
    }
    
    if (m_pRenderTextureInner&&m_pShapeSprite) {
        m_pShapeSprite->setBlendFunc((ccBlendFunc){GL_ZERO,GL_ONE_MINUS_SRC_ALPHA});
//        point = m_pRenderTextureInner->convertToNodeSpace(point);
//        point = ccpAdd(point, Vec2(m_pRenderTexture->getSprite()->getContentSize().width*0.5, m_pRenderTexture->getSprite()->getContentSize().height*0.5));
        
        Vec2 pos = m_pRenderTextureInner->getParent()->convertToNodeSpace(point);
        log("====%f,%f,%f,%f",m_pRenderTextureInner->getContentSize().width,m_pRenderTextureInner->getContentSize().height,pos.x,pos.y);
        m_pShapeSprite->setPosition(pos);
        m_pShapeSprite->setScale(0.8);
        m_pRenderTextureInner->begin();
        m_pShapeSprite->visit();
        m_pRenderTextureInner->end();
        m_pShapeSprite->setScale(1);
        Director::getInstance()->getRenderer()->render();
        
        __NotificationCenter::getInstance()->removeObserver(m_pRenderTextureInner, EVENT_COME_TO_BACKGROUND);
        __NotificationCenter::getInstance()->removeObserver(m_pRenderTextureInner, EVENT_COME_TO_FOREGROUND);
        
    }
}
