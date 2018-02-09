
#include "BurgerWearGloveScene.h"
#include "SceneManager.h"

BurgerWearGloveScene::BurgerWearGloveScene()
{
    m_nMoveCount = 0;
    m_nStep = 0;
}

BurgerWearGloveScene::~BurgerWearGloveScene()
{
    
}
bool BurgerWearGloveScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/bg_make2.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    m_pHand = Sprite::create(localPath("hand.png"));
    this->addChildToContentLayer(m_pHand);
    CMVisibleRect::setPositionAdapted(m_pHand, 500+visibleSize.width, 180);
    m_pHand->runAction(Sequence::create(DelayTime::create(1),
                                        MoveBy::create(1, Vec2(-visibleSize.width, 0)), NULL));
    m_pHand->setLocalZOrder(5);
    
    m_pGlove = _createDrageNode(localPath("gloves_small.png"));
    this->addChildToContentLayer(m_pGlove);
    CMVisibleRect::setPositionAdapted(m_pGlove, 200, 750+visibleSize.height);
    m_pGlove->setTouchEnabled(false);
    m_pGlove->runAction(Sequence::create(DelayTime::create(1),
                                         MoveBy::create(1, Vec2(0, -visibleSize.height)),
                                         CallFunc::create([=](){
        m_pGlove->setTouchEnabled(true);
        m_pGuideLayer->showGuideMove(m_pGlove->getPosition(), m_pHand->getPosition());
    }), NULL));
    
    m_pGloveBack = Sprite::create(localPath("hand_gloves_big_down.png"));
    this->addChildToContentLayer(m_pGloveBack);
    m_pGloveBack->setVisible(false);
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    
    m_pTouchLayer = TouchLayer::create();
    this->addChildToUILayer(m_pTouchLayer);
    m_pTouchLayer->onTouchBegan = CC_CALLBACK_2(BurgerWearGloveScene::TouchBegan, this);
    m_pTouchLayer->onTouchMoved = CC_CALLBACK_2(BurgerWearGloveScene::TouchMove, this);
    m_pTouchLayer->onTouchEnded = CC_CALLBACK_2(BurgerWearGloveScene::TouchEnded, this);
    m_pTouchLayer->setTouchLayerEnable(false);
    
    m_pGameUI->showNormalLayout();
    return true;
}

void BurgerWearGloveScene::onEnter()
{
    ExtensionScene::onEnter();
}

void BurgerWearGloveScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void BurgerWearGloveScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    if (pDragNode->index==0) {
        pDragNode->index = 1;
        pDragNode->setDragSprite(Sprite::create(localPath("hand_gloves_big_on.png")));
    }
}

void BurgerWearGloveScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    if (pDragNode->index==1) {
        Rect rect = m_pHand->getBoundingBox();
        rect.origin = m_pHand->getParent()->convertToWorldSpace(rect.origin);
        Rect rect2 = pDragNode->getDragSprite()->getBoundingBox();
        rect2.origin = pDragNode->convertToWorldSpace(rect2.origin);
        if (rect.intersectsRect(rect2)) {
            m_nMoveCount++;
            if (m_nMoveCount%10!=0) {
                return;
            }
            pDragNode->setTouchEnabled(false);
            static Vec2 sOffset[6] = {
                Vec2(-222,243),
                Vec2(-200,213),
            };
            float handScale = 0.7;
            float gloveScale = 1.3;
            if (m_nStep==0) {
                m_pGuideLayer->removeGuide();
                
                m_pHand->runAction(ScaleTo::create(0.3, handScale));
                pDragNode->setPosition(m_pHand->getPosition()+sOffset[0]);
                m_pGloveBack->setPosition(pDragNode->getPosition());
                pDragNode->runAction(ScaleTo::create(0.3, gloveScale));
            }
            m_pHand->runAction(Sequence::create(DelayTime::create(0.3),
                                                ScaleTo::create(1, handScale), NULL));
            auto action = Sequence::create(DelayTime::create(0.3),
                                           Spawn::create(MoveTo::create(1, m_pHand->getPosition()+sOffset[m_nStep+1]),
                                                         ScaleTo::create(1, gloveScale), NULL),
                                           CallFuncN::create([=](Node*){
                m_pGuideLayer->showGuideMove(m_pHand->getPosition()+Vec2(-100, 200), m_pHand->getPosition());
                m_pTouchLayer->setTouchLayerEnable(true);
            }), NULL);
            
            pDragNode->runAction(action);
            m_pGloveBack->runAction(action->clone());
            this->runAction(Sequence::create(DelayTime::create(2),
                                             CallFunc::create([=](){
                m_nStep++;
            }), NULL));
        }
    }
    
}

void BurgerWearGloveScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    m_pGuideLayer->removeGuide();
}

void BurgerWearGloveScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

void BurgerWearGloveScene::TouchBegan(Touch *pTouch, Event *pEvent)
{
    
}
void BurgerWearGloveScene::TouchMove(Touch *pTouch, Event *pEvent)
{
    if (!m_pTouchLayer->getToucnEable()) {
        return;
    }
    Rect rect = m_pGloveBack->getBoundingBox();
    rect.origin = m_pGloveBack->getParent()->convertToWorldSpace(rect.origin);
    Vec2 pos = pTouch->getLocation();
    if (rect.containsPoint(pos)) {
        m_nMoveCount++;
//        if (m_nMoveCount%1!=0) {
//            return;
//        }
        m_pTouchLayer->setTouchLayerEnable(false);
        static Vec2 sOffset[6] = {
            Vec2(-222,243),
            Vec2(-200,213),
            Vec2(-131,123),
            Vec2(-68,55),
            Vec2(-15,21),
            Vec2::ZERO,
        };
        float handScale = 0.7;
        float gloveScale = 1.3;
        if (m_nStep==1){
            handScale = 0.76;
        }else if (m_nStep==2){
            handScale = 0.8;
            gloveScale = 1.22;
        }else if (m_nStep==3){
            handScale = 0.98;
            gloveScale = 1.17;
        }else if (m_nStep==4){
            handScale = 1;
            gloveScale = 1;
        }
        if (m_nStep==0) {
            m_pGuideLayer->removeGuide();
            
            m_pHand->runAction(ScaleTo::create(0.3, handScale));
            m_pGlove->setPosition(m_pHand->getPosition()+Vec2(-222, 243));
            m_pGloveBack->setPosition(m_pGlove->getPosition());
            m_pGlove->runAction(ScaleTo::create(0.3, gloveScale));
        }
        static int totalCount = 5;
        
        m_pHand->runAction(Sequence::create(DelayTime::create(0.3),
                                            ScaleTo::create(1, handScale), NULL));
        auto action = Sequence::create(DelayTime::create(0.3),
                                       Spawn::create(MoveTo::create(1, m_pHand->getPosition()+sOffset[m_nStep+1]),
                                                     ScaleTo::create(1, gloveScale), NULL),
                                       CallFuncN::create([=](Node*){
        }), NULL);
        
        m_pGlove->runAction(action);
        m_pGloveBack->runAction(action->clone());
        this->runAction(Sequence::create(DelayTime::create(2),
                                         CallFunc::create([=](){
            m_nStep++;
            if (m_nStep==totalCount) {
                
                m_pGuideLayer->removeGuide();
                _finish();
            }else{
                
                m_pGuideLayer->showGuideMove(m_pHand->getPosition()+Vec2(-100, 200), m_pHand->getPosition());
                m_pTouchLayer->setTouchLayerEnable(true);
            }
        }), NULL));
    }
}
void BurgerWearGloveScene::TouchEnded(Touch *pTouch, Event *pEvent)
{
}
#pragma mark - initData
void BurgerWearGloveScene::_initData()
{
    setExPath("content/make/burger/2/");
}

void BurgerWearGloveScene::_finish()
{
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<BurgerMixScene>();
    }), NULL));
}

