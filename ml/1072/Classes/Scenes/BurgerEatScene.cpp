
#include "BurgerEatScene.h"

BurgerEatScene::BurgerEatScene()
{
    m_pDecoLayer = nullptr;
}

BurgerEatScene::~BurgerEatScene()
{
    
}
bool BurgerEatScene::init()
{
    if ( !ShareBaseScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/bg_decoration1.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
        
    _showPlate();
    _showHotdog();
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_eat_burger.mp3");
    }), NULL));
    
    m_pEatLayer = EatLayer::create();
    this->addChildToUILayer(m_pEatLayer);
    m_pEatLayer->onEatBeginCallback = CC_CALLBACK_2(BurgerEatScene::onEatBeinCallback, this);
    m_pEatLayer->onEatEndCallback = CC_CALLBACK_1(BurgerEatScene::onEatEndCallback, this);
    
//    m_pGameUI->hideBack();
    return true;
}

void BurgerEatScene::onEnter()
{
    ShareBaseScene::onEnter();
}

void BurgerEatScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void BurgerEatScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
}

void BurgerEatScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void BurgerEatScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
}

void BurgerEatScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

void BurgerEatScene::onEatBeinCallback(Vec2 location,bool drinkable)
{
}

void BurgerEatScene::onEatEndCallback(Vec2 location)
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
void BurgerEatScene::_initData()
{
    setExPath("content/make/burger/5/");
}

void BurgerEatScene::_reset()
{    
    m_pDecManager->reset();
    _showHotdog(false);
}

void BurgerEatScene::_showPlate()
{
    m_pPlate = Sprite::create(localPath("plate.png"));
    this->addChildToContentLayer(m_pPlate);
    CMVisibleRect::setPositionAdapted(m_pPlate, 320-visibleSize.width, 520-200);
    m_pPlate->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
}

void BurgerEatScene::_showHotdog(bool animation)
{
    if (!m_pDecoLayer) {
        m_pDecoLayer = Layer::create();
        this->addChildToContentLayer(m_pDecoLayer);
    }else{
        m_pDecoLayer->removeAllChildren();
    }
    
    m_pDecManager = DecorateManager::getInstance();
    m_pDecManager->showFinishedFood(m_pDecoLayer);
    
    if(animation){
        m_pDecoLayer->setPosition(Vec2(-visibleSize.width, 0));
        m_pDecoLayer->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    }
    
    MakeEatLayer* pEatLayer = MakeEatLayer::create();
    
    Sprite* pHotdog = Sprite::create(FileUtils::getInstance()->getWritablePath()+"burger_dec.png");
    Size size = m_pPlate->getContentSize();
    if (pHotdog) {
        size = pHotdog->getContentSize();
    }
    
    MyRenderTexture* rt = MyRenderTexture::create(size.width,size.height);
    rt->setPosition(Vec2(0, 0));
    rt->beginWithClear(0, 0, 0, 0);
    pHotdog->setPosition(Vec2(0, -200));
    if (pHotdog) {
        pHotdog->setPosition(Vec2(size.width/2, size.height/2));
//        pHotdog->setScale(2);
        pHotdog->visit();
    }
    rt->end();
    Director::getInstance()->getRenderer()->render();
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_FOREGROUND);
    
    pEatLayer->setRenderTexture(rt);
    
//    CMVisibleRect::setPositionAdapted(pEatLayer, 320, 480);
    m_pDecoLayer->addChild(pEatLayer);
    m_pDecManager->setEatableLayer(pEatLayer);
    
}