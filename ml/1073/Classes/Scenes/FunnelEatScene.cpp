
#include "FunnelEatScene.h"

FunnelEatScene::FunnelEatScene()
{
    m_pDecoLayer = nullptr;
}

FunnelEatScene::~FunnelEatScene()
{
    
}
bool FunnelEatScene::init()
{
    if ( !ShareBaseScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/bg_dec.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
        
    _showPlate();
    _showFunnelCake();
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_funnel_eat_page.mp3");
    }), NULL));
    
    m_pEatLayer = EatLayer::create();
    this->addChildToUILayer(m_pEatLayer);
    m_pEatLayer->onEatBeginCallback = CC_CALLBACK_2(FunnelEatScene::onEatBeinCallback, this);
    m_pEatLayer->onEatEndCallback = CC_CALLBACK_1(FunnelEatScene::onEatEndCallback, this);
    
//    m_pGameUI->hideBack();
    return true;
}

void FunnelEatScene::onEnter()
{
    ShareBaseScene::onEnter();
}

void FunnelEatScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void FunnelEatScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
}

void FunnelEatScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void FunnelEatScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
}

void FunnelEatScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

void FunnelEatScene::onEatBeinCallback(Vec2 location,bool drinkable)
{
}

void FunnelEatScene::onEatEndCallback(Vec2 location)
{
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
    m_pDecManager->doEat(location);
    m_pDecManager->stopDrink();
    AudioHelp::getInstance()->stopDrinkEffect();
    if (m_pDecManager->finishedEat() && !m_pGameUI->eatAgainIsShow()) {
        
        m_pGameUI->showEatAgainLayout();
        m_pGameUI->showNextLayout2();
        
        AudioHelp::getInstance()->playEffect("cheer_star.mp3");
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
        this->addChildToUILayer(pParticle);
        pParticle->setLocalZOrder(10);
        pParticle->setPosition(visibleSize*0.5);
    }
}


#pragma mark - initData
void FunnelEatScene::_initData()
{
    setExPath("content/make/funnel/dec/");
}

void FunnelEatScene::_reset()
{    
    m_pDecManager->reset();
    _showFunnelCake(false);
    m_pDecManager->m_nEatPercent = 78;
}

void FunnelEatScene::_showPlate()
{
    m_pPlate = Sprite::create(localPath("plate.png"));
    this->addChildToContentLayer(m_pPlate);
    CMVisibleRect::setPositionAdapted(m_pPlate, 320-visibleSize.width, 520-210+80);
    m_pPlate->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
}

void FunnelEatScene::_showFunnelCake(bool animation)
{
    if (!m_pDecoLayer) {
        m_pDecoLayer = Layer::create();
        this->addChildToContentLayer(m_pDecoLayer);
    }else{
        m_pDecoLayer->removeAllChildren();
    }
    m_pDecManager = DecorateManager::getInstance();
    m_pDecManager->showFinishedFood(m_pDecoLayer);
    m_pDecManager->m_nEatPercent = 78;
    if(animation){
        m_pDecoLayer->setPosition(Vec2(-visibleSize.width, 0));
        m_pDecoLayer->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    }
    
    MakeEatLayer* pEatLayer = MakeEatLayer::create();
    
//    Vec2 pos = Vec2(320, 400+80);
    int funnelIndex = GameDataManager::getInstance()->m_nFunnelIndex;
    Sprite* pCake = Sprite::create(localPath("cake"+std::to_string(funnelIndex)+".png"));
    
    Sprite* pSugar = Sprite::create(FileUtils::getInstance()->getWritablePath()+"funnel_dec.png");
    Size size = m_pPlate->getContentSize();
    if (pSugar) {
        size = pSugar->getContentSize();
    }
    
    pCake->setPosition(Vec2(size.width/2, size.height/2));
    MyRenderTexture* rt = MyRenderTexture::create(size.width,size.height);
    rt->setPosition(Vec2(0, 0));
    rt->beginWithClear(0, 0, 0, 0);
    pCake->visit();
    if (pSugar) {
        pSugar->setPosition(Vec2(size.width/2, size.height/2));
        pSugar->visit();
    }
    rt->end();
    Director::getInstance()->getRenderer()->render();
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_FOREGROUND);
    
    pEatLayer->setRenderTexture(rt);
    
    CMVisibleRect::setPositionAdapted(rt, 320, 520-200+80);
    
    m_pDecoLayer->addChild(pEatLayer);
    m_pDecManager->setEatableLayer(pEatLayer);
    m_pDecManager->setShapeTexture("content/eatShape1.png");
    
    m_pTip = Sprite::create("content/common/tte.png");
    this->addChildToUILayer(m_pTip);
    CMVisibleRect::setPosition(m_pTip, 320+visibleSize.width, 100,kBorderNone,kBorderTop);
    m_pTip->runAction(Sequence::create(DelayTime::create(0.5),
                                       MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                       DelayTime::create(2),
                                       CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_tap_enjoy.mp3");
    }), NULL));
}