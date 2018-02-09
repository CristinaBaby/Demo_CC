
#include "BurgerRollBeafScene.h"
#include "SceneManager.h"

BurgerRollBeafScene::BurgerRollBeafScene()
{
    m_nMoveCount = 0;
    m_nFinishCount = 0;
    m_nRollCount = 0;
}

BurgerRollBeafScene::~BurgerRollBeafScene()
{
    
}
bool BurgerRollBeafScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("bg/bg_make1.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    _showBowl();
    _showBoard();
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        m_pGuideLayer->showGuideMove(m_pBeaf->getPosition(), m_pBoard->getPosition()+Vec2(100, 50));
        AudioHelp::getInstance()->playEffect("vo_drag_meat.mp3");
    }), NULL));
    
    m_pTouchLayer = TouchLayer::create();
    this->addChildToUILayer(m_pTouchLayer);
    m_pTouchLayer->onTouchBegan = CC_CALLBACK_2(BurgerRollBeafScene::TouchBegan, this);
    m_pTouchLayer->onTouchMoved = CC_CALLBACK_2(BurgerRollBeafScene::TouchMove, this);
    m_pTouchLayer->onTouchEnded = CC_CALLBACK_2(BurgerRollBeafScene::TouchEnded, this);
    m_pTouchLayer->setTouchLayerEnable(false);
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    
    m_pGameUI->showNormalLayout();
    return true;
}

void BurgerRollBeafScene::onEnter()
{
    ExtensionScene::onEnter();
}

void BurgerRollBeafScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void BurgerRollBeafScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if (name == "beaf"){
        if (pDragNode->index == 0){
            pDragNode->setVisible(true);
            pDragNode->index = 1;
            m_pMixture->setTexture(localPath("bowl_beef"+std::to_string(m_nFinishCount+2)+".png"));
        }
    }
}

void BurgerRollBeafScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void BurgerRollBeafScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    std::string name = pDragNode->getName();
    if (name == "beaf") {
        if (pDragNode->index == 1) {
            Rect rect = m_pBoard->getBoundingBox();
            if (rect.containsPoint(pDragNode->getPosition())) {
                pDragNode->setTouchEnabled(false);
                pDragNode->index = 2;
                pDragNode->setPosition(m_pBoard->getPosition()+Vec2(100, 20));
                m_pBeafGrid->setPosition(pDragNode->getPosition());
                
                pDragNode->setVisible(false);
                m_pBeafGrid->setVisible(true);
                
                m_nMoveCount = 0;
                
                m_pGuideLayer->showGuideMove(pDragNode->getPosition(), pDragNode->getPosition()+Vec2(0, -100));
                m_pTouchLayer->setTouchLayerEnable();
            }
        }else if(pDragNode->index == 2){
            Sprite* pBeafFinish = Sprite::create(localPath("beef_patty7.png"));
            this->addChildToContentLayer(pBeafFinish);
            pBeafFinish->setScale(0.7);
            pBeafFinish->setPosition(m_pBoard->getPosition()+Vec2(-140, 60-m_nFinishCount*60));
            
            m_pBeaf->setVisible(false);
            m_pBeaf->setPosition(m_pBowl->getPosition());
            m_pBeaf->getDragSprite()->setTexture(localPath("beef_patty1.png"));
            m_pRollBeaf->setTexture(localPath("beef_patty1.png"));
            
            _finish();
        }
    }
}

void BurgerRollBeafScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

void BurgerRollBeafScene::TouchBegan(Touch *pTouch, Event *pEvent)
{
    if (m_nRollCount>3) {
        return;
    }
    Rect rect = m_pBeaf->getDragSprite()->getBoundingBox();
    rect.origin = m_pBeaf->convertToWorldSpace(rect.origin);
    Vec2 pos = pTouch->getLocation();
    if(rect.containsPoint(pos)){
        m_pBeafGrid->runAction(RepeatForever::create(Sequence::create(Waves::create(1, Size(20, 20), 2, 5, true, true),
                                                                      DelayTime::create(0.1), NULL)));
    }
}

void BurgerRollBeafScene::TouchMove(Touch *pTouch, Event *pEvent)
{
    if (m_nRollCount>3) {
        return;
    }
    Rect rect = m_pBeaf->getDragSprite()->getBoundingBox();
    rect.origin = m_pBeaf->convertToWorldSpace(rect.origin);
    Vec2 pos = pTouch->getLocation();
    if(rect.containsPoint(pos)){
        m_nMoveCount++;
        static int count = 80;
        if (m_nMoveCount%count==0) {
            int step = m_nMoveCount/count;
            if (step<=3) {
                m_nRollCount = step;
                m_pRollBeaf->setTexture(localPath("beef_patty"+std::to_string(step+1)+".png"));
                if (step==3) {
                    
                    m_pGuideLayer->showGuideTap(m_pBeaf->getPosition());
                }
            }
        }
    }
}

void BurgerRollBeafScene::TouchEnded(Touch *pTouch, Event *pEvent)
{
    m_pGuideLayer->removeGuide();
    m_pBeafGrid->stopAllActions();
    if (m_nRollCount>3 && m_nRollCount<7) {
        m_pGuideLayer->showGuideTap(m_pBeaf->getPosition());
        m_pRollBeaf->setTexture(localPath("beef_patty"+std::to_string(m_nRollCount+1)+".png"));
        if (m_nRollCount>=6) {
            _finishRoll();
        }
        m_nRollCount++;
    }
    if(m_nRollCount==3){
        m_nRollCount++;
    }
}

#pragma mark - initData
void BurgerRollBeafScene::_initData()
{
    setExPath("content/make/burger/3/");
}

void BurgerRollBeafScene::_showBowl()
{
    m_pBowl = Sprite::create(localPath("bowl.png"));
    CMVisibleRect::setPosition(m_pBowl, 320+visibleSize.width, 810);
    this->addChildToContentLayer(m_pBowl);
    m_pBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
//    m_pBowl->setScale(0.6);
    
    m_pMixture = Sprite::create(localPath("bowl_beef1.png"));
    this->addChildToContentLayer(m_pMixture);
    m_pMixture->setLocalZOrder(5);
    CMVisibleRect::setPosition(m_pMixture, 320+visibleSize.width, 810);
    m_pMixture->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
//    m_pMixture->setScale(0.6);
    
    m_pBeaf = _createDrageNode(localPath("beef_patty1.png"));
    this->addChildToContentLayer(m_pBeaf);
    CMVisibleRect::setPosition(m_pBeaf, 320+visibleSize.width, 810);
    m_pBeaf->setTouchEnabled(false);
    m_pBeaf->setName("beaf");
    m_pBeaf->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                        CallFunc::create([=](){
        m_pBeaf->setTouchEnabled(true);
    }), NULL));
}

void BurgerRollBeafScene::_showBoard()
{
    m_pBoard = Sprite::create(localPath("plate.png"));
    this->addChildToContentLayer(m_pBoard);
    CMVisibleRect::setPosition(m_pBoard, 320+visibleSize.width, 300);
    m_pBoard->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
    m_pBeafGrid = NodeGrid::create();
    this->addChildToContentLayer(m_pBeafGrid);
    m_pBeafGrid->setVisible(false);
    
    m_pRollBeaf = Sprite::create(localPath("beef_patty1.png"));
    m_pBeafGrid->addChild(m_pRollBeaf);
}

void BurgerRollBeafScene::_finishRoll()
{
    m_pTouchLayer->setTouchLayerEnable(false);
    m_pBeafGrid->stopAllActions();
    m_pBeaf->getDragSprite()->setTexture(localPath("beef_patty7.png"));
    m_pBeafGrid->setVisible(false);
    m_pBeaf->setVisible(true);
    m_pBeaf->setTouchEnabled(true);
    
    m_pGuideLayer->showGuideMove(m_pBeaf->getPosition(), m_pBoard->getPosition()+Vec2(-60, 60-m_nFinishCount*60));
}
void BurgerRollBeafScene::_finish()
{
    m_nFinishCount++;
    if (m_nFinishCount==3) {
        m_pBeaf->setTouchEnabled(false);
        AudioHelp::getInstance()->playEffect("done.mp3");
        AudioHelp::getInstance()->_playEffectNiceWork();
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
        this->addChildToUILayer(pParticle);
        pParticle->setLocalZOrder(10);
        pParticle->setPosition(visibleSize*0.5);
        
        this->runAction(Sequence::create(DelayTime::create(2),
                                         CallFunc::create([=](){
            SceneManager::replaceTheScene<BurgerFryBeafScene>();
        }), NULL));
    }else{
        m_nRollCount = 0;
        m_pBeaf->index = 0;
        m_pBeaf->setTouchEnabled(true);
        m_pGuideLayer->showGuideMove(m_pBeaf->getPosition(), m_pBoard->getPosition()+Vec2(100, 20));
    }
    
}