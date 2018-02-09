
#include "PizzaEatScene.h"

PizzaEatScene::PizzaEatScene()
{
    m_pDecoLayer = nullptr;
}

PizzaEatScene::~PizzaEatScene()
{
    
}
bool PizzaEatScene::init()
{
    if ( !ShareBaseScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/bg.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    Sprite* pTable = Sprite::create(localRootPath("0/bk1.png"));
    CMVisibleRect::setPositionAdapted(pTable, 320, 300);
    this->addChildToBGLayer(pTable);
    
    
    _showPlate();
    _showPizza();
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_eat_pizza.mp3");
    }), NULL));
    
    m_pEatLayer = EatLayer::create();
    this->addChildToUILayer(m_pEatLayer);
    m_pEatLayer->onEatBeginCallback = CC_CALLBACK_2(PizzaEatScene::onEatBeinCallback, this);
    m_pEatLayer->onEatEndCallback = CC_CALLBACK_1(PizzaEatScene::onEatEndCallback, this);
    
    m_pGameUI->hideBack();
    return true;
}

void PizzaEatScene::onEnter()
{
    ShareBaseScene::onEnter();
}

void PizzaEatScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void PizzaEatScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
}

void PizzaEatScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void PizzaEatScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
}

void PizzaEatScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

void PizzaEatScene::onEatBeinCallback(Vec2 location,bool drinkable)
{
}

void PizzaEatScene::onEatEndCallback(Vec2 location)
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
void PizzaEatScene::_initData()
{
    setExPath("content/make/pizza/2/");
}

void PizzaEatScene::_reset()
{    
    m_pDecManager->reset();
    _showPizza(false);
}

void PizzaEatScene::_showPlate()
{
    m_pPlate = Sprite::create(localPath("plate.png"));
    this->addChildToContentLayer(m_pPlate);
    CMVisibleRect::setPositionAdapted(m_pPlate, 320-visibleSize.width, 350);
    m_pPlate->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
}

void PizzaEatScene::_showPizza(bool animation)
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
    
    Sprite* pPizza = Sprite::create(FileUtils::getInstance()->getWritablePath()+"pizza_dec.png");
    Size size = m_pPlate->getContentSize();
    if (pPizza) {
        size = pPizza->getContentSize()*1.5;
    }
    
    MyRenderTexture* rt = MyRenderTexture::create(size.width,size.height);
    rt->setPosition(Vec2(0, 0));
    rt->beginWithClear(0, 0, 0, 0);
    if (pPizza) {
        pPizza->setPosition(Vec2(size.width/2, size.height/2));
        pPizza->setScale(1.5);
        pPizza->visit();
    }
    rt->end();
    Director::getInstance()->getRenderer()->render();
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(rt, EVENT_COME_TO_FOREGROUND);
    
    pEatLayer->setRenderTexture(rt);
    pEatLayer->setShapeTexture(Sprite::create("content/eatShape3.png")->getTexture());
//    pEatLayer->setPosition(Vec2(-size.width/2, -size.height/2));
//    pEatLayer->setShapeTexture(Sprite::create("content/eatShape2.png")->getTexture());
//    this->addChildToContentLayer(pEatLayer);
//    CMVisibleRect::setPositionAdapted(pEatLayer, 320-size.width/2-visibleSize.width, 320-size.height/2);
//    pEatLayer->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    
    CMVisibleRect::setPositionAdapted(pEatLayer, 320-size.width/2, 340-size.height/2);
    m_pDecoLayer->addChild(pEatLayer);
    m_pDecManager->setEatableLayer(pEatLayer);
    
}