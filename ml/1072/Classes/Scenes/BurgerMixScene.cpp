
#include "BurgerMixScene.h"
#include "SceneManager.h"

BurgerMixScene::BurgerMixScene()
{
    m_nMoveCount = 0;
    m_nCount = 0;

}

BurgerMixScene::~BurgerMixScene()
{
    
}
bool BurgerMixScene::init()
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
    
    _showBowl();
    _showHand();
    
    m_pTouchLayer = TouchLayer::create();
    this->addChildToUILayer(m_pTouchLayer);
    m_pTouchLayer->onTouchBegan = CC_CALLBACK_2(BurgerMixScene::TouchBegan, this);
    m_pTouchLayer->onTouchMoved = CC_CALLBACK_2(BurgerMixScene::TouchMove, this);
    m_pTouchLayer->onTouchEnded = CC_CALLBACK_2(BurgerMixScene::TouchEnded, this);
    AudioHelp::getInstance()->playEffect("vo_hand_mix.mp3");
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    m_pGameUI->showNormalLayout();
    return true;
}

void BurgerMixScene::onEnter()
{
    ExtensionScene::onEnter();
}

void BurgerMixScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void BurgerMixScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
}

void BurgerMixScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void BurgerMixScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
}

void BurgerMixScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

void BurgerMixScene::TouchBegan(Touch *pTouch, Event *pEvent)
{
    
}

void BurgerMixScene::TouchMove(Touch *pTouch, Event *pEvent)
{
    if(!m_pTouchLayer->getToucnEable()){
        return;
    }
    Rect rect = m_pBowl->getBoundingBox();
    rect.origin = m_pBowl->getParent()->convertToWorldSpace(rect.origin);
    Vec2 pos = pTouch->getLocation();
    if(rect.containsPoint(pos)){
        m_nMoveCount++;
        if (!m_pBeafGrid->getActionByTag(100)) {
            auto action = RepeatForever::create(Sequence::create(Waves::create(1, Size(10, 10), 2, 2, true, true),
                                                                 DelayTime::create(0.1), NULL));
            action->setTag(100);
            m_pBeafGrid->runAction(action);
        }
//        if (m_nMoveCount%20 == 0){
//            int tag = m_pHand->getTag();
//            m_pHand->setTexture(localPath("hand_make"+std::to_string(tag%4+1)+".png"));
//            m_pHand->setTag(tag+1);
//        }
        if (!m_pHand->getActionByTag(100)) {
            auto action = RepeatForever::create(AnimationHelp::createAnimate(localPath("hand_make"), 1, 4,true,true,0.5));
            action->setTag(100);
            m_pHand->runAction(action);
        }
        if (m_nMoveCount%40 == 0) {
//            m_pHand->runAction(AnimationHelp::createAnimate(localPath("hand_make"), 4, 1,true,true,0.3));
            m_nCount++;
            if (m_nCount/4==5) {
                m_pTouchLayer->setTouchLayerEnable(false);
                m_pHand->runAction(Sequence::create(DelayTime::create(1),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                    _finish();
                }), NULL));
            }
            if (m_nCount%4==0 && m_nCount/4<=5) {
                m_pMixture->setTexture(localPath("bowl_"+std::to_string(m_nCount/4+1)+".png"));
            }
        }
        
    }
}

void BurgerMixScene::TouchEnded(Touch *pTouch, Event *pEvent)
{
    m_pHand->stopActionByTag(100);
    if(!m_pTouchLayer->getToucnEable()){
        return;
    }
    m_pBeafGrid->stopAllActions();
}
#pragma mark - initData
void BurgerMixScene::_initData()
{
    setExPath("content/make/burger/2/");
}


void BurgerMixScene::_showBowl()
{
    m_pBowl = Sprite::create(localPath("bowl.png"));
    CMVisibleRect::setPosition(m_pBowl, 320+visibleSize.width, 430);
    this->addChildToContentLayer(m_pBowl);
    m_pBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
    m_pBeafGrid = NodeGrid::create();
    this->addChildToContentLayer(m_pBeafGrid);
    m_pBeafGrid->setLocalZOrder(5);
    CMVisibleRect::setPosition(m_pBeafGrid, 330+visibleSize.width, 420);
    m_pBeafGrid->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
//    m_pMixture = Sprite::create(FileUtils::getInstance()->getWritablePath()+"buger_mixture");
    m_pMixture = Sprite::create(localPath("bowl_1.png"));
    m_pBeafGrid->addChild(m_pMixture);
}

void BurgerMixScene::_showHand()
{
    m_pHand = Sprite::create(localPath("hand_make1.png"));
    this->addChildToContentLayer(m_pHand);
    m_pHand->setTag(0);
    CMVisibleRect::setPositionAdapted(m_pHand, 460+visibleSize.width, 280);
    m_pHand->setLocalZOrder(10);
    m_pHand->runAction(Sequence::create(DelayTime::create(1),
                                        MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                        CallFunc::create([=](){
        m_pTouchLayer->setTouchLayerEnable();
    }), NULL));
    
}

void BurgerMixScene::_finish()
{
    m_pBowl->runAction(MoveTo::create(0.5, visibleSize*0.5));
    m_pBeafGrid->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, 50)),
                                           CallFunc::create([=](){
        
        AudioHelp::getInstance()->playEffect("done.mp3");
        AudioHelp::getInstance()->_playEffectNiceWork();
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
        this->addChildToUILayer(pParticle);
        pParticle->setLocalZOrder(10);
        pParticle->setPosition(visibleSize*0.5);
        
        this->runAction(Sequence::create(DelayTime::create(2),
                                         CallFunc::create([=](){
            SceneManager::replaceTheScene<BurgerRollBeafScene>();
        }), NULL));
    }), NULL));
}