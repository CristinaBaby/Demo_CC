
#include "DecorateManager.h"
#include "Global.h"

DecorateManager::DecorateManager()
{
    m_pFoodUneatableBackLayer = nullptr;
    m_pFoodUneatableFrontLayer = nullptr;
    m_pDrinkUneatableBackLayer = nullptr;
    m_pDrinkUneatableFrontLayer = nullptr;
    m_pFoodLayer = nullptr;
    m_pDrinkLayer = nullptr;
    m_pDrinkFoodLayer = nullptr;
    m_pAllFrontLayer = nullptr;
    m_pAllUneatFrontLayer = nullptr;
    m_pDrinkRT = nullptr;
    m_pDrinkSprite = nullptr;
    m_pDrinkMaskSprite = nullptr;
    m_pDrinkDecRT = nullptr;
    m_pStraw = nullptr;
    
    m_pEatableLayer = nullptr;
    m_pEatableFrontLayer = nullptr;
    m_nDrinkPercent = 100;
    m_bFinishedEat = false;
    
    m_pMultiDrinkLayer = nullptr;
    m_pMultiSauceLayer = nullptr;
}

DecorateManager::~DecorateManager()
{
    if (m_pDrinkMaskSprite) {
        m_pDrinkMaskSprite->release();
    }
    if (m_pDrinkSprite) {
        m_pDrinkSprite->release();
    }
}

void DecorateManager::initWithParent(Node* parent,int type)
{
    m_pParent = parent;
    parent->addChild(this);
    if (type==eDecorateTypeFood) {
        _initFoodLayer();
    }else if (type==eDecorateTypeDrink) {
        _initDrinkLayer();
    }else{
        _initFoodLayer();
        _initDrinkLayer();
    }
    m_pAllFrontLayer = _initLayer(m_pAllFrontLayer);
    m_pAllUneatFrontLayer = _initLayer(m_pAllUneatFrontLayer);
    m_pMultiDrinkLayer = _initLayer(m_pMultiDrinkLayer);
    m_pMultiSauceLayer = _initLayer(m_pMultiSauceLayer);
}

void DecorateManager::setDrinkMaskSprite(Sprite* pMask)
{
    if (m_pDrinkMaskSprite) {
        m_pDrinkMaskSprite->release();
    }
    m_pDrinkMaskSprite = pMask;
    pMask->retain();
    pMask->setBlendFunc((BlendFunc){GL_ZERO,GL_SRC_ALPHA});
}
void DecorateManager::addDecoration(cocos2d::Node *pUnit, int layerFlag,const std::string singleName,Vec2 position,int zOrder)
{
    pUnit->setPosition(position);
    if (std::strcmp(singleName.c_str(), "")!=0){
        pUnit->setName(singleName);
    }
    switch (layerFlag) {
        case eDecorationLayerFoodBack:
        {
            if (std::strcmp(singleName.c_str(), "")!=0) {
                Node* pChild = m_pFoodUneatableBackLayer->getChildByName(singleName);
                if (pChild) {
                    pChild->removeFromParent();
                }
            }
            m_pFoodUneatableBackLayer->addChild(pUnit, zOrder);
        }
            break;
        case eDecorationLayerFood:
        {
            if (std::strcmp(singleName.c_str(), "")!=0) {
                Node* pChild = m_pFoodLayer->getChildByName(singleName);
                if (pChild) {
                    pChild->removeFromParent();
                }
            }
            m_pFoodLayer->addChild(pUnit, zOrder);
        }
            break;
        case eDecorationLayerFoodFront:
        {
            if (std::strcmp(singleName.c_str(), "")!=0) {
                Node* pChild = m_pFoodUneatableFrontLayer->getChildByName(singleName);
                if (pChild) {
                    pChild->removeFromParent();
                }
            }
            m_pFoodUneatableFrontLayer->addChild(pUnit, zOrder);
        }
            break;
        case eDecorationLayerDrinkBack:
        {
            if (std::strcmp(singleName.c_str(), "")!=0) {
                Node* pChild = m_pDrinkUneatableBackLayer->getChildByName(singleName);
                if (pChild) {
                    pChild->removeFromParent();
                }
            }
            m_pDrinkUneatableBackLayer->addChild(pUnit, zOrder);
        }
            break;
        case eDecorationLayerDrink:
        {
            if (std::strcmp(singleName.c_str(), "")!=0) {
                Node* pChild = m_pDrinkLayer->getChildByName(singleName);
                if (pChild) {
                    pChild->removeFromParent();
                }
            }
            if (m_pDrinkSprite) {
                m_pDrinkSprite->release();
            }
            if (!m_pDrinkRT) {
                m_pDrinkRT = RenderTexture::create(visibleSize.width, visibleSize.height);
                m_pDrinkRT->retain();
                __NotificationCenter::getInstance()->removeObserver(m_pDrinkRT, EVENT_COME_TO_BACKGROUND);
                __NotificationCenter::getInstance()->removeObserver(m_pDrinkRT, EVENT_COME_TO_FOREGROUND);
            }
            if (!m_pDrinkDecRT) {
                m_pDrinkDecRT = RenderTexture::create(visibleSize.width, visibleSize.height);
                m_pDrinkDecRT->retain();
                m_pDrinkLayer->addChild(m_pDrinkDecRT);
                __NotificationCenter::getInstance()->removeObserver(m_pDrinkDecRT, EVENT_COME_TO_BACKGROUND);
                __NotificationCenter::getInstance()->removeObserver(m_pDrinkDecRT, EVENT_COME_TO_FOREGROUND);
            }
            
            m_pDrinkSprite = (Sprite*)pUnit;
            m_pDrinkSprite->setAnchorPoint(Vec2(0.5,0));
            pUnit->setPosition(Vec2(position.x, position.y-0.5*m_pDrinkSprite->getContentSize().height));
            m_pDrinkSprite->retain();
            m_pDrinkSprite->setBlendFunc((BlendFunc){GL_ONE,GL_ZERO});
            m_DrinkSize = m_pDrinkSprite->boundingBox().size;
            
            m_pDrinkDecRT->setPosition(visibleSize.width/2, visibleSize.height/2);
            m_pDrinkMaskSprite->setPosition(Vec2(m_pDrinkSprite->getPositionX(), m_pDrinkSprite->getPositionY()+0.5*m_pDrinkSprite->getContentSize().height));
            
            m_pDrinkDecRT->begin();
            m_pDrinkSprite->visit();
            m_pDrinkMaskSprite->visit();
            m_pDrinkDecRT->end();
            Director::getInstance()->getRenderer()->render();
        }
            break;
        case eDecorationLayerDrinkFood:
        {
            if (std::strcmp(singleName.c_str(), "")!=0) {
                Node* pChild = m_pDrinkFoodLayer->getChildByName(singleName);
                if (pChild) {
                    pChild->removeFromParent();
                }
            }
            m_pDrinkFoodLayer->addChild(pUnit, zOrder);
        }
            break;
        case eDecorationLayerDrinkFront:
        {
            if (std::strcmp(singleName.c_str(), "")!=0) {
                Node* pChild = m_pDrinkUneatableFrontLayer->getChildByName(singleName);
                if (pChild) {
                    pChild->removeFromParent();
                }
            }
            m_pDrinkUneatableFrontLayer->addChild(pUnit, zOrder);
        }
            break;
        case eDecorationLayerAllUneatFront:
        {
            if (std::strcmp(singleName.c_str(), "")!=0) {
                Node* pChild = m_pAllUneatFrontLayer->getChildByName(singleName);
                if (pChild) {
                    pChild->removeFromParent();
                }
            }
            m_pAllUneatFrontLayer->addChild(pUnit, zOrder);
        }
            break;
        case eDecorationLayerAllFront:
        {
            if (std::strcmp(singleName.c_str(), "")!=0) {
                Node* pChild = m_pAllFrontLayer->getChildByName(singleName);
                if (pChild) {
                    pChild->removeFromParent();
                }
            }
            m_pAllFrontLayer->addChild(pUnit, zOrder);
        }
            break;
            
        case eDecorationLayerMultDrink:
        {
            if (std::strcmp(singleName.c_str(), "")!=0) {
                Node* pChild = m_pMultiDrinkLayer->getChildByName(singleName);
                if (pChild) {
                    pChild->removeFromParent();
                }
            }
            m_pMultiDrinkLayer->addChild(pUnit, zOrder);
            m_DrinkVector.pushBack(pUnit);
        }
            break;
        case eDecorationLayerMultSauce:
        {
            if (std::strcmp(singleName.c_str(), "")!=0) {
                Node* pChild = m_pMultiSauceLayer->getChildByName(singleName);
                if (pChild) {
                    pChild->removeFromParent();
                }
            }
            m_pMultiSauceLayer->addChild(pUnit, zOrder);
            m_SauceVector.pushBack(pUnit);
        }
            break;
        default:
            break;
    }
}
void DecorateManager::reset()
{
//    if (m_pParent) {
//        m_pParent->removeAllChildren();
//    }
    this->removeAllChildren();
    if (m_pDrinkMaskSprite) {
        m_pDrinkMaskSprite->release();
    }
    
    if (m_pDrinkDecRT) {
        m_pDrinkDecRT->release();
    }
    
    if (m_pDrinkRT) {
        m_pDrinkRT->release();
    }
    
    if (m_pDrinkSprite) {
        m_pDrinkSprite->release();
    }
    
    if (this->getParent()) {
        this->removeFromParent();
    }
    m_pDrinkMaskSprite = nullptr;
    m_pDrinkDecRT = nullptr;
    m_pDrinkRT = nullptr;
    m_pDrinkSprite = nullptr;
    
    m_pFoodUneatableBackLayer = nullptr;
    m_pFoodUneatableFrontLayer = nullptr;
    m_pDrinkUneatableBackLayer = nullptr;
    m_pDrinkUneatableFrontLayer = nullptr;
    m_pAllUneatFrontLayer = nullptr;
    m_pFoodLayer = nullptr;
    m_pDrinkLayer = nullptr;
    m_pDrinkFoodLayer = nullptr;
    m_pAllFrontLayer = nullptr;
    m_pDrinkRT = nullptr;
    m_pDrinkSprite = nullptr;
    m_pDrinkMaskSprite = nullptr;
    m_pDrinkDecRT = nullptr;
    m_pStraw = nullptr;
    
    m_pEatableLayer = nullptr;
    m_pEatableFrontLayer = nullptr;
//    m_pUneatableLayer = nullptr;
    
    m_pMultiDrinkLayer = nullptr;
    m_pMultiSauceLayer = nullptr;
    m_nDrinkPercent = 100;
    m_bFinishedEat = false;
    
    m_SauceVector.clear();
    m_DrinkVector.clear();
}

void DecorateManager::reDecorate()
{
    if (m_pFoodLayer) {
        m_pDrinkMaskSprite->release();
    }
    m_pDrinkMaskSprite = nullptr;
    
    if (m_pDrinkDecRT) {
        m_pDrinkDecRT->release();
    }
    m_pDrinkDecRT = nullptr;
    
    if (m_pDrinkRT) {
        m_pDrinkRT->release();
    }
    m_pDrinkRT = nullptr;
    
    if (m_pDrinkSprite) {
        m_pDrinkSprite->release();
    }
    m_pDrinkSprite = nullptr;
    
    if (m_pFoodUneatableBackLayer) {
        m_pFoodUneatableBackLayer->removeAllChildren();
    }
    if (m_pFoodUneatableFrontLayer) {
        m_pFoodUneatableFrontLayer->removeAllChildren();
    }
    if (m_pDrinkUneatableBackLayer) {
        m_pDrinkUneatableBackLayer->removeAllChildren();
    }
    
    if (m_pDrinkUneatableFrontLayer) {
        m_pDrinkUneatableFrontLayer->removeAllChildren();
    }
    if (m_pFoodLayer) {
        m_pFoodLayer->removeAllChildren();
    }
    if (m_pDrinkFoodLayer) {
        m_pDrinkFoodLayer->removeAllChildren();
    }
    if (m_pDrinkLayer) {
        m_pDrinkLayer->removeAllChildren();
    }
    if (m_pAllFrontLayer) {
        m_pAllFrontLayer->removeAllChildren();
    }
    if (m_pAllUneatFrontLayer) {
        m_pAllUneatFrontLayer->removeAllChildren();
    }
}
void DecorateManager::doDrink()
{
    if (!m_pDrinkDecRT) {
        return;
    }
    float delayTime = 1;
    _updateDrink();
//    resume();
//    schedule(schedule_selector(DecorateManager::_updateDrink), delayTime);
    Director::getInstance()->getRunningScene()->schedule(schedule_selector(DecorateManager::_updateDrink), delayTime);
}

void DecorateManager::stopDrink()
{
    log("====DecorateManager::stopDrink()====");
//    unschedule(schedule_selector(DecorateManager::_updateDrink));
    Director::getInstance()->getRunningScene()->unschedule(schedule_selector(DecorateManager::_updateDrink));
}

void DecorateManager::doEat(Vec2 location)
{
    bool bUpdate = false;
    if (m_pEatableLayer) {
        m_pEatableLayer->doEat(location);
         bUpdate = true;
    }
    if (m_pEatableFrontLayer) {
        m_pEatableFrontLayer->doEat(location);
        bUpdate = true;
    }
    for (int i = 0; i<m_SauceShareVector.size(); i++) {
        Node* util = (DrinkUtil*)m_SauceShareVector.at(i);
        if (util) {
            MakeEatLayer* eatLayer = (MakeEatLayer*)util->getChildByName("eatLayer");
            if (eatLayer) {
                eatLayer->doEat(location);
            }
        }
    }
    if (bUpdate) {
        _updateFinishedPercent();
    }
}

void DecorateManager::saveEatLayer()
{
    {
//        uneatable back
        RenderTexture* pUneatableBackRender =  RenderTexture::create(visibleSize.width, visibleSize.height);
        pUneatableBackRender->setPosition(visibleSize*0.5);
        
        pUneatableBackRender->begin();
        if (m_pFoodUneatableBackLayer) {
            m_pFoodUneatableBackLayer->visit();
        }
        if (m_pDrinkUneatableBackLayer) {
            m_pDrinkUneatableBackLayer->visit();
        }
        pUneatableBackRender->end();
        __NotificationCenter::getInstance()->removeObserver(pUneatableBackRender, EVENT_COME_TO_BACKGROUND);
        __NotificationCenter::getInstance()->removeObserver(pUneatableBackRender, EVENT_COME_TO_FOREGROUND);
    }
    
    {
        RenderTexture* pEatableBackRender =  RenderTexture::create(visibleSize.width, visibleSize.height);
        pEatableBackRender->setPosition(visibleSize*0.5);
        
        pEatableBackRender->begin();
        if (m_pFoodLayer) {
            m_pFoodLayer->visit();
        }
        pEatableBackRender->end();
        __NotificationCenter::getInstance()->removeObserver(pEatableBackRender, EVENT_COME_TO_BACKGROUND);
        __NotificationCenter::getInstance()->removeObserver(pEatableBackRender, EVENT_COME_TO_FOREGROUND);
    }
}


Layer* DecorateManager::getUneatableLayer()
{
    Layer* pLayer = Layer::create();
    RenderTexture* rt = RenderTexture::create(visibleSize.width, visibleSize.height);
    rt->setPosition(visibleSize*0.5);
    rt->begin();
    if (m_pFoodUneatableBackLayer) {
        m_pFoodUneatableBackLayer->visit();
    }
    if (m_pDrinkUneatableBackLayer) {
        m_pDrinkUneatableBackLayer->visit();
    }
    rt->end();
    Director::getInstance()->getRenderer()->render();
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_FOREGROUND);
    
    Image* image = rt->newImage();
    Texture2D* lTexture = new Texture2D();
    lTexture->initWithImage(image);
    Sprite* pSprite = Sprite::createWithTexture(lTexture);
    lTexture->autorelease();
    image->autorelease();
    pSprite->setPosition(visibleSize*0.5);
    log("%f,%f",pSprite->getContentSize().width,pSprite->getContentSize().height);
    pLayer->addChild(pSprite);
    
    return pLayer;
}


Layer* DecorateManager::getFoodFrontLayer()
{
    Layer* pLayer = Layer::create();
    RenderTexture* rt = RenderTexture::create(visibleSize.width, visibleSize.height);
//    rt->setPosition(visibleSize*0.5);
    rt->begin();
    if (m_pFoodUneatableFrontLayer) {
        Node* pNode = m_pFoodUneatableFrontLayer->getChildByName("package");
        if (pNode) {
            pNode->setVisible(false);
        }
        m_pFoodUneatableFrontLayer->visit();
        if (pNode) {
            pNode->setVisible(true);
        }
    }
    rt->end();
    Director::getInstance()->getRenderer()->render();
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_FOREGROUND);
//    pLayer->addChild(rt);
    Image* image = rt->newImage();
    Texture2D* lTexture = new Texture2D();
    lTexture->initWithImage(image);
    Sprite* pSprite = Sprite::createWithTexture(lTexture);
    lTexture->autorelease();
    image->autorelease();
    pSprite->setPosition(visibleSize*0.5);
    pLayer->addChild(pSprite);
    return pLayer;
}

Layer* DecorateManager::getDrinkFrontLayer()
{
    Layer* pLayer = Layer::create();
    RenderTexture* rt = RenderTexture::create(visibleSize.width, visibleSize.height);
    rt->setPosition(visibleSize*0.5);
    rt->beginWithClear(0, 0, 0, 0);
    if (m_pDrinkUneatableFrontLayer) {
        m_pDrinkUneatableFrontLayer->visit();
    }
    rt->end();
    Director::getInstance()->getRenderer()->render();
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_FOREGROUND);
    pLayer->addChild(rt);
    
//    Image* image = rt->newImage();
//    Texture2D* lTexture = new Texture2D();
//    lTexture->initWithImage(image);
//    Sprite* pSprite = Sprite::createWithTexture(lTexture);
//    lTexture->autorelease();
//    image->autorelease();
//    pSprite->setPosition(visibleSize*0.5);
//    pLayer->addChild(pSprite);
    return pLayer;
}
MakeEatLayer* DecorateManager::getEatableLayer()
{
    MakeEatLayer* pLayer = MakeEatLayer::create();
    MyRenderTexture* rt = MyRenderTexture::create(visibleSize.width, visibleSize.height*1.4);
    rt->setPosition(Vec2(visibleSize.width*0.5,visibleSize.height*0.7));
    rt->beginWithClear(0, 0, 0, 0);
    if (m_pFoodLayer) {
        m_pFoodLayer->visit();
    }
    if (m_pDrinkFoodLayer) {
        m_pDrinkFoodLayer->visit();
    }
    rt->end();
    
    MyRenderTexture* rt2 = MyRenderTexture::create(visibleSize.width, visibleSize.height*1.4);
    rt2->setPosition(Vec2(visibleSize.width*0.5,visibleSize.height*0.7));
    
    Sprite* pSpriteTexture = Sprite::createWithTexture(rt->getSprite()->getTexture());
    Sprite* pSprite = Sprite::createWithTexture(rt->getSprite()->getTexture());
//    Vec2 position = pSprite->getPosition();
    pSprite->setPosition(Vec2(visibleSize.width/2, visibleSize.height*0.7));
    pSprite->setFlippedY(-1);
    
    pSpriteTexture->setPosition(Vec2(visibleSize.width/2, visibleSize.height*0.7));
    pSpriteTexture->setFlippedY(-1);
    
    pSprite->setBlendFunc((ccBlendFunc){GL_ZERO,GL_SRC_ALPHA});
    LayerColor* pColor = LayerColor::create(Color4B(180, 180, 180, 150));
//    pColor->setBlendFunc((ccBlendFunc){GL_ONE,GL_ZERO});
    pSpriteTexture->setBlendFunc((ccBlendFunc){GL_ONE,GL_ZERO});
    
    rt2->beginWithClear(0, 0, 0, 0);
    pSpriteTexture->visit();
    pColor->visit();
    pSprite->visit();
    
    rt2->end();
    
    Director::getInstance()->getRenderer()->render();
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_FOREGROUND);
    pLayer->setRenderTexture(rt);
    
    
    pLayer->setRenderTextureInner(rt2);
    
    return pLayer;
}

MakeEatLayer* DecorateManager::getEatableFrontLayer()
{
    MakeEatLayer* pLayer = MakeEatLayer::create();
    MyRenderTexture* rt = MyRenderTexture::create(visibleSize.width, visibleSize.height*1.4);
    rt->setPosition(Vec2(visibleSize.width/2, visibleSize.height*0.7));
    rt->beginWithClear(0, 0, 0, 0);
    if (m_pAllFrontLayer) {
        m_pAllFrontLayer->visit();
    }
    rt->end();
    
    MyRenderTexture* rt2 = MyRenderTexture::create(visibleSize.width, visibleSize.height*1.4);
    rt2->setPosition(Vec2(visibleSize.width/2, visibleSize.height*0.7));
    
    Sprite* pSpriteTexture = Sprite::createWithTexture(rt->getSprite()->getTexture());
    Sprite* pSprite = Sprite::createWithTexture(rt->getSprite()->getTexture());
    //    Vec2 position = pSprite->getPosition();
    pSprite->setPosition(Vec2(visibleSize.width/2, visibleSize.height*0.7));
    pSprite->setFlippedY(-1);
    
    pSpriteTexture->setPosition(Vec2(visibleSize.width/2, visibleSize.height*0.7));
    pSpriteTexture->setFlippedY(-1);
    
    pSprite->setBlendFunc((ccBlendFunc){GL_ZERO,GL_SRC_ALPHA});
    LayerColor* pColor = LayerColor::create(Color4B(180, 180, 180, 150));
    //    pColor->setBlendFunc((ccBlendFunc){GL_ONE,GL_ZERO});
    pSpriteTexture->setBlendFunc((ccBlendFunc){GL_ONE,GL_ZERO});
    
    rt2->beginWithClear(0, 0, 0, 0);
    pSpriteTexture->visit();
    pColor->visit();
    pSprite->visit();
    
    rt2->end();
    
    Director::getInstance()->getRenderer()->render();
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_FOREGROUND);
    pLayer->setRenderTexture(rt);
    
    __NotificationCenter::getInstance()->removeObserver(rt2, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(rt2, EVENT_COME_TO_FOREGROUND);
    
    pLayer->setRenderTextureInner(rt2);
    
    return pLayer;
}

Layer* DecorateManager::getUneatableFrontLayer()
{
    Layer* pLayer = Layer::create();
    RenderTexture* rt = RenderTexture::create(visibleSize.width, visibleSize.height*1.4);
    rt->setPosition(Vec2(visibleSize.width/2, visibleSize.height*0.7));
    rt->begin();
    if (m_pAllUneatFrontLayer) {
        m_pAllUneatFrontLayer->visit();
    }
    rt->end();
    Director::getInstance()->getRenderer()->render();
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_FOREGROUND);

    Image* image = rt->newImage();
    Texture2D* lTexture = new Texture2D();
    lTexture->initWithImage(image);
    Sprite* pSprite = Sprite::createWithTexture(lTexture);
//    Sprite* pSprite = Sprite::createWithTexture(rt->getSprite()->getTexture());
    lTexture->autorelease();
    image->autorelease();
    pSprite->setPosition(visibleSize*0.5);
    log("%f,%f",pSprite->getContentSize().width,pSprite->getContentSize().height);
    pLayer->addChild(pSprite);
    
    return pLayer;

}
RenderTexture* DecorateManager::getDrinkRenderTexture()
{
    return m_pDrinkRT;
}

void DecorateManager::_updateDrink(float dt)
{
    if (DecorateManager::getInstance()->m_nDrinkPercent<0 || DecorateManager::getInstance()->m_nDrinkPercent>100) {
        log("====DecorateManager::_updateDrink() stopDrink====%d",DecorateManager::getInstance()->m_nDrinkPercent);
//        unschedule(schedule_selector(DecorateManager::_updateDrink));
        Director::getInstance()->getRunningScene()->unschedule(schedule_selector(DecorateManager::_updateDrink));
        return;
    }
    DecorateManager::getInstance()->m_nDrinkPercent -=10;
    _showDrink(DecorateManager::getInstance()->m_nDrinkPercent);
    _updateFinishedPercent();
    
}

void DecorateManager::_showDrink(int percent)
{
//    if (percent>100 || percent<0) {
//        return;
//    }
    log("====update %d",percent);
    if (!DecorateManager::getInstance()->m_pDrinkRT) {
        DecorateManager::getInstance()->m_pDrinkRT = RenderTexture::create(visibleSize.width, visibleSize.height);
        DecorateManager::getInstance()->m_pDrinkRT->retain();
        __NotificationCenter::getInstance()->removeObserver(DecorateManager::getInstance()->m_pDrinkRT, EVENT_COME_TO_BACKGROUND);
        __NotificationCenter::getInstance()->removeObserver(DecorateManager::getInstance()->m_pDrinkRT, EVENT_COME_TO_FOREGROUND);
//        m_pDrinkLayer->addChild(m_pDrinkRT);
    }
    DecorateManager::getInstance()->m_pDrinkRT->setPosition(visibleSize.width/2, visibleSize.height/2);
    
    DecorateManager::getInstance()->m_pDrinkSprite->setTextureRect(Rect(0, 0, DecorateManager::getInstance()->m_DrinkSize.width, DecorateManager::getInstance()->m_DrinkSize.height*percent/100));
//    m_pDrinkMaskSprite->setPosition(Vec2(m_pDrinkSprite->getPositionX(), m_pDrinkSprite->getPositionY()+0.5*m_pDrinkSprite->getContentSize().height));
    
    DecorateManager::getInstance()->m_pDrinkRT->beginWithClear(0, 0, 0, 0);
    DecorateManager::getInstance()->m_pDrinkSprite->visit();
    DecorateManager::getInstance()->m_pDrinkMaskSprite->visit();
    DecorateManager::getInstance()->m_pDrinkRT->end();
    Director::getInstance()->getRenderer()->render();
}

void DecorateManager::_updateFinishedPercent()
{
    int percent = 0;
    int curPercent = 0;
    int percent1 = 0;
    int curPercent1 = 0;
    int percent2 = 0;
    int curPercent2 = 0;

    if (DecorateManager::getInstance()->m_pEatableLayer) {
        percent+=100;
        curPercent += DecorateManager::getInstance()->m_pEatableLayer->getPercent();
    }
    if (DecorateManager::getInstance()->m_pEatableFrontLayer) {
        percent1+=100;
        curPercent1 += DecorateManager::getInstance()->m_pEatableFrontLayer->getPercent();
    }
    
    if (m_pDrinkDecRT) {
        percent2+=100;
        if (m_nDrinkPercent<=100) {
            curPercent2 += (100-m_nDrinkPercent)+10;
        }
    }
    
    bool bDrinkFinish = true;
    for (int i = 0; i<m_DrinkShareVector.size(); i++) {
        DrinkUtil* util = (DrinkUtil*)m_DrinkShareVector.at(i);
        if (util) {
            if (util->getWaterPercent()>30) {
                bDrinkFinish = false;
                break;
            }
        }
    }
    
    bool bSauceFinish = true;
    for (int i = 0; i<m_SauceShareVector.size(); i++) {
        Node* util = (DrinkUtil*)m_SauceShareVector.at(i);
        if (util) {
            MakeEatLayer* eatLayer = (MakeEatLayer*)util->getChildByName("eatLayer");
            if (eatLayer) {
                if (eatLayer->getPercent()<98) {
                    bSauceFinish = false;
                }
            }
        }
    }
    if ((curPercent*100/percent>=98 || percent==0) && (curPercent1*100/percent1>=98 || percent1==0)&& (curPercent2>80 || curPercent2 ==0) && bDrinkFinish && bSauceFinish) {
        m_bFinishedEat = true;
    }
}
void DecorateManager::_initFoodLayer()
{
    if (!m_pFoodUneatableBackLayer) {
        m_pFoodUneatableBackLayer = _initLayer(m_pFoodUneatableBackLayer);
    }
    if (!m_pFoodLayer) {
        m_pFoodLayer = _initLayer(m_pFoodLayer);
    }
    if (!m_pFoodUneatableFrontLayer) {
        m_pFoodUneatableFrontLayer = _initLayer(m_pFoodUneatableFrontLayer);
    }
}

void DecorateManager::_initDrinkLayer()
{
    m_pDrinkUneatableBackLayer = _initLayer(m_pDrinkUneatableBackLayer);
    m_pDrinkFoodLayer = _initLayer(m_pDrinkFoodLayer);
    m_pDrinkLayer = _initLayer(m_pDrinkLayer);
    m_pDrinkUneatableFrontLayer = _initLayer(m_pDrinkUneatableFrontLayer);
    
//    if (!m_pDrinkDecRT) {
//        m_pDrinkDecRT = RenderTexture::create(visibleSize.width, visibleSize.height);
//        m_pDrinkLayer->addChild(m_pDrinkDecRT);
//        m_pDrinkDecRT->retain();
//    }
}

Layer * DecorateManager::_initLayer(cocos2d::Layer *pLayer)
{
    if (!pLayer) {
        pLayer = Layer::create();
        this->addChild(pLayer);
    }else{
        pLayer->removeAllChildren();
        if (pLayer->getParent()) {
            pLayer->removeFromParent();
        }
        this->addChild(pLayer);
    }
    return pLayer;
}