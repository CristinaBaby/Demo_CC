
#include "EatEggScene.h"
#include "SceneManager.h"

EatEggScene::EatEggScene()
{
    
}

EatEggScene::~EatEggScene()
{
    
}
bool EatEggScene::init()
{
    if ( !ShareScene::init() )
    {
        return false;
    }
    _initData();
    
    m_pGameUI->hideBack();
    //    bg
    
//    AudioHelp::getInstance()->playEffect("vo_cant_wait_ry.mp3");
    return true;
}

void EatEggScene::onEnter()
{
    ShareScene::onEnter();
}

void EatEggScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}


#pragma mark - initData
void EatEggScene::_initData()
{
    setExPath("content/make/egg/make2/");
}

void EatEggScene::_showEatScene()
{
    m_bCanEat = true;
    m_pPlate = Sprite::create(localPath("plate.png"));
    m_pDecoLayer->addChild(m_pPlate);
    CMVisibleRect::setPositionAdapted(m_pPlate, 320, 400);
    
    {
        Node* pDec = Node::create();
        int shape = GameDataManager::getInstance()->m_nShapeIndex;
        if(shape <4){
            Sprite* pFood = Sprite::create(localPath(gEggShapeName[shape]+".png"));
            pFood->setScale(0.7);
            
            Sprite* pFood2 = Sprite::create(localPath(gEggShapeName[shape]+".png"));
            pFood2->setScale(0.7);
            
            pFood->setPosition(Vec2(-100, 0));
            pFood2->setPosition(Vec2(100, 0));
            
            pDec->addChild(pFood);
            pDec->addChild(pFood2);
        }else{
            Sprite* pFood = Sprite::create(localPath(gEggShapeName[shape]+".png"));
            
            Sprite* pBacon = Sprite::create("content/make/bacon/bacon5_1.png");
            pBacon->setRotation(-15);
            
            Sprite* pBacon2 = Sprite::create("content/make/bacon/bacon5_2.png");
            pBacon2->setRotation(15);
            
            pFood->setPosition(Vec2(0, 50));
            pBacon->setPosition(Vec2(0, -60));
            pBacon2->setPosition(Vec2(0, -60));
            
            pDec->addChild(pFood);
            pDec->addChild(pBacon);
            pDec->addChild(pBacon2);
        }
        
        Size size = m_pPlate->getContentSize();
        
        pDec->setPosition(Vec2(size.width/2, size.height/2));
        
        MyRenderTexture* rt = MyRenderTexture::create(size.width,size.height,Texture2D::PixelFormat::RGBA8888,
                                                      GL_DEPTH24_STENCIL8_OES);
        
        rt->beginWithClear(0, 0, 0, 0);
        pDec->visit();
        rt->end();
        Director::getInstance()->getRenderer()->render();
        __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_BACKGROUND);
        __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_FOREGROUND);
        
        m_pEatLayerDown = MakeEatLayer::create();
        m_pEatLayerDown->setRenderTexture(rt);
        
        CMVisibleRect::setPositionAdapted(rt, 320, 400);
        m_pDecoLayer->addChild(m_pEatLayerDown);
    }
    
    {
        m_bCanEat = true;
        m_pTip = Sprite::create("content/common/tte.png");
        this->addChildToUILayer(m_pTip);
        CMVisibleRect::setPosition(m_pTip, 320+visibleSize.width, 100,kBorderNone,kBorderTop);
        m_pTip->runAction(Sequence::create(DelayTime::create(0.5),
                                           MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                           DelayTime::create(2),
                                           CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("vo_cant_wait.mp3");
        }), NULL));
    }
}

void EatEggScene::_reset()
{
    m_pPlate->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                         CallFunc::create([=](){
        CMVisibleRect::setPositionAdapted(m_pPlate, 320, 400);
    }), NULL));
    m_pEatLayerDown->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                CallFunc::create([=](){
        m_pEatLayerDown->removeFromParent();
        
        Node* pDec = Node::create();
        int shape = GameDataManager::getInstance()->m_nShapeIndex;
        if(shape <4){
            Sprite* pFood = Sprite::create(localPath(gEggShapeName[shape]+".png"));
            pFood->setScale(0.7);
            
            Sprite* pFood2 = Sprite::create(localPath(gEggShapeName[shape]+".png"));
            pFood2->setScale(0.7);
            
            pFood->setPosition(Vec2(-100, 0));
            pFood2->setPosition(Vec2(100, 0));
            
            pDec->addChild(pFood);
            pDec->addChild(pFood2);
        }else{
            Sprite* pFood = Sprite::create(localPath(gEggShapeName[shape]+".png"));
            
            Sprite* pBacon = Sprite::create("content/make/bacon/bacon5_1.png");
            pBacon->setRotation(-15);
            
            Sprite* pBacon2 = Sprite::create("content/make/bacon/bacon5_2.png");
            pBacon2->setRotation(15);
            
            pFood->setPosition(Vec2(0, 50));
            pBacon->setPosition(Vec2(0, -60));
            pBacon2->setPosition(Vec2(0, -60));
            
            pDec->addChild(pFood);
            pDec->addChild(pBacon);
            pDec->addChild(pBacon2);
        }
        
        Size size = m_pPlate->getContentSize();
        
        pDec->setPosition(Vec2(size.width/2, size.height/2));
        
        MyRenderTexture* rt = MyRenderTexture::create(size.width,size.height,Texture2D::PixelFormat::RGBA8888,
                                                      GL_DEPTH24_STENCIL8_OES);
        
        rt->beginWithClear(0, 0, 0, 0);
        pDec->visit();
        rt->end();
        Director::getInstance()->getRenderer()->render();
        __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_BACKGROUND);
        __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_FOREGROUND);
        
        m_pEatLayerDown = MakeEatLayer::create();
        m_pEatLayerDown->setRenderTexture(rt);
        
        CMVisibleRect::setPositionAdapted(rt, 320, 400);
        m_pDecoLayer->addChild(m_pEatLayerDown);
    }), NULL));
    
}

void EatEggScene::onEatEndCallback(Vec2 location)
{
    if (!m_bCanEat) {
        return;
    }
    if (m_pTip) {
        m_pTip->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                           CallFuncN::create([=](Node* node)
                                                             {
                                                                 node->removeFromParent();
                                                             }),
                                           NULL));
        m_pTip = nullptr;
    }
        AudioHelp::getInstance()->playEffect("basic/eat.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/biting.plist");
    this->addChildToContentLayer(pParticle);
    pParticle->setPosition(location);
    
    m_pEatLayerDown->doEat(location);
    
    if (m_pEatLayerDown->getPercent()>=80 && !m_pGameUI->eatAgainIsShow()) {
        m_pGameUI->showEatAgainLayout();
        m_pGameUI->showNextLayout2();
        
        AudioHelp::getInstance()->playEffect("magic_effect.mp3");
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
        this->addChildToUILayer(pParticle);
        pParticle->setLocalZOrder(10);
        pParticle->setPosition(visibleSize*0.5);
    }
}

