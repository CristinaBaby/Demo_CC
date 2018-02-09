
#include "DrinkJuiceScene.h"
#include "SceneManager.h"

DrinkJuiceScene::DrinkJuiceScene()
{
    
}

DrinkJuiceScene::~DrinkJuiceScene()
{
    
}
bool DrinkJuiceScene::init()
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

void DrinkJuiceScene::onEnter()
{
    ShareScene::onEnter();
}

void DrinkJuiceScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}


#pragma mark - initData
void DrinkJuiceScene::_initData()
{
    setExPath("content/make/juice/");
}

void DrinkJuiceScene::_reset()
{
    m_pDrink->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                         CallFunc::create([=](){
        m_pDecoLayer->removeAllChildren();
        
        int flavorIndex = GameDataManager::getInstance()->m_nFlavorIndex;
        int strawIndex = GameDataManager::getInstance()->m_nStickIndex;
        m_pDrink = Node::create();
        this->addChildToContentLayer(m_pDrink);
        CMVisibleRect::setPositionAdapted(m_pDrink, 320, 420);
        
        KettleNode* pContainer = KettleNode::create(m_pDrink);
        m_pDrink->setUserData(pContainer);
        pContainer->addBody(localPath("cup0_h.png"));
        pContainer->addFront(localPath("cup0.png"));
        pContainer->addWater(localPath("cup_juice"+std::to_string(flavorIndex)+".png"),localPath("cup_mask.png"));
        
        ClippingNode* pClippingStraw = ClippingNode::create(Sprite::create("content/category/straw/straw_mask2.png"));
        pClippingStraw->setAlphaThreshold(0);
        pClippingStraw->setInverted(true);
        pClippingStraw->setName("straw_clipping");
        m_pDrink->addChild(pClippingStraw,8);
        
        Vec2 startPos = m_pDrink->getPosition();
        Sprite* pStraw = Sprite::create("content/category/straw/"+std::to_string(strawIndex+1)+".png");
        pClippingStraw->addChild(pStraw);
        pStraw->setAnchorPoint(Vec2::ZERO);
        pStraw->setPosition(pClippingStraw->convertToNodeSpace(startPos+Vec2(-50, -130)));
        pStraw->setName("straw");
    }), NULL));
    
}
void DrinkJuiceScene::_showEatScene()
{
    int flavorIndex = GameDataManager::getInstance()->m_nFlavorIndex;
    int strawIndex = GameDataManager::getInstance()->m_nStickIndex;
    m_pDrink = Node::create();
    this->addChildToContentLayer(m_pDrink);
    CMVisibleRect::setPositionAdapted(m_pDrink, 320, 420);
    
    KettleNode* pContainer = KettleNode::create(m_pDrink);
    m_pDrink->setUserData(pContainer);
    pContainer->addBody(localPath("cup0_h.png"));
    pContainer->addFront(localPath("cup0.png"));
    pContainer->addWater(localPath("cup_juice"+std::to_string(flavorIndex)+".png"),localPath("cup_mask.png"));
    
    ClippingNode* pClippingStraw = ClippingNode::create(Sprite::create("content/category/straw/straw_mask2.png"));
    pClippingStraw->setAlphaThreshold(0);
    pClippingStraw->setInverted(true);
    pClippingStraw->setName("straw_clipping");
    m_pDrink->addChild(pClippingStraw,8);
    
    Vec2 startPos = m_pDrink->getPosition();
    Sprite* pStraw = Sprite::create("content/category/straw/"+std::to_string(strawIndex+1)+".png");
    pClippingStraw->addChild(pStraw);
    pStraw->setAnchorPoint(Vec2::ZERO);
    pStraw->setPosition(pClippingStraw->convertToNodeSpace(startPos+Vec2(-50, -130)));
    pStraw->setName("straw");
    {
        m_bCanEat = true;
        m_pTip = Sprite::create("content/common/ttd.png");
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
void DrinkJuiceScene::onEatBeinCallback(Vec2 location,bool drinkable)
{
    if (!m_bCanEat) {
        return;
    }
    KettleNode* pCup = (KettleNode*)m_pDrink->getUserData();
    if(pCup){
        pCup->startDrink();
    }
}

void DrinkJuiceScene::onEatEndCallback(Vec2 location)
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
    
    if(m_pDrink){
        KettleNode* pCup = (KettleNode*)m_pDrink->getUserData();
        if(pCup){
            pCup->stopDrink();
        }
        
    }
    KettleNode* pCup = (KettleNode*)m_pDrink->getUserData();
    if(pCup){
        if (pCup->getWater()->getScaleY()<=0.1 && !m_pGameUI->eatAgainIsShow()) {
            m_pGameUI->showEatAgainLayout(true);
            m_pGameUI->showNextLayout2();
            
            AudioHelp::getInstance()->playEffect("magic_effect.mp3");
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
            this->addChildToUILayer(pParticle);
            pParticle->setLocalZOrder(10);
            pParticle->setPosition(visibleSize*0.5);
        };
    }
}