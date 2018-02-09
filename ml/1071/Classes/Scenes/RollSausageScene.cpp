

#include "RollSausageScene.h"
#include "SceneManager.h"

RollSausageScene::RollSausageScene()
{
    m_pPlateRight = nullptr;
    m_nAddCount = 0;
    m_nMoveCount = 0;
    m_bReady = false;
}

RollSausageScene::~RollSausageScene()
{
    
}
bool RollSausageScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    Sprite* pBg = Sprite::create(localPath("bg/bg2.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    
    _showPlate();
    _showPlateRight();
    _showPlateDown();
    
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        m_pGuideLayer->showGuideMove(m_pPlateRight->getPosition(), m_pPlateDown->getPosition());
    }),   NULL));
    
    m_pTouchLayer = TouchLayer::create();
    this->addChildToUILayer(m_pTouchLayer);
    m_pTouchLayer->setTouchLayerEnable(false);
    m_pTouchLayer->onTouchBegan = CC_CALLBACK_2(RollSausageScene::TouchBegan, this);
    m_pTouchLayer->onTouchMoved = CC_CALLBACK_2(RollSausageScene::TouchMove, this);
    m_pTouchLayer->onTouchEnded = CC_CALLBACK_2(RollSausageScene::TouchEnded, this);

    m_pGameUI->showNormalLayout();
    return true;
}

void RollSausageScene::onEnter()
{
    ExtensionScene::onEnter();
}

void RollSausageScene::onExit()
{
    ExtensionScene::onExit();
}

void RollSausageScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void RollSausageScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    pDragNode->setLocalZOrder(20);
    pDragNode->stopAllActions();
    pDragNode->setScale(1);
    if (pDragNode->index==0) {
        
    }else{
        ProgressBarNew* pBar = (ProgressBarNew*)pDragNode->getUserData();
        if(pBar){
            pBar->pauseProgress();
        }
        
        pDragNode->stopAllActions();
    }
}
void RollSausageScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    
}
void RollSausageScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    if (!m_pPlateDown) {
        pDragNode->back(pDragNode->getTag());
        pDragNode->runAction(ScaleTo::create(0.5, 0.5));
        return;
    }
    std::string name = pDragNode->getName();
    Rect doughRect = m_pPlateDown->getBoundingBox();
    doughRect.origin = m_pPlateDown->getParent()->convertToWorldSpace(doughRect.origin);
    
    Rect rect = pDragNode->getDragSprite()->getBoundingBox();
    rect.origin = pDragNode->convertToWorldSpace(rect.origin);
    int index = m_PlateDownVector.size();
    if (name == "pancake"){
        if (rect.intersectsRect(doughRect) && index==0){
            AudioHelp::getInstance()->playEffect("fall.mp3");
            pDragNode->setTouchEnabled(false);
            pDragNode->setVisible(false);
            pDragNode->setPosition(m_pPlateDown->getPosition());
            m_PlateDownVector.push_back(pDragNode);
//            m_PancakeVector.erase(m_PancakeVector.begin()+pDragNode->getTag());
            auto iter = std::find(m_SausageVector.begin(), m_SausageVector.end(), pDragNode);
            if (iter != m_SausageVector.end())
            {
                m_SausageVector.erase(iter);
            }
            
            m_pHotdog = Sprite::create(localPath("make4/hotdog1.png"));
            this->addChildToContentLayer(m_pHotdog);
            m_pHotdog->setPosition(m_pPlateDown->getPosition());
            if(m_nAddCount == 0){
                m_pGuideLayer->showGuideMove(m_pPlateLeft->getPosition(), m_pPlateDown->getPosition());
            }
        }else{
            pDragNode->back(pDragNode->getTag());
            pDragNode->runAction(ScaleTo::create(0.5, 0.7));
        }
        
    }else if (name == "sausage") {
        if (rect.intersectsRect(doughRect) && index==1){
            AudioHelp::getInstance()->playEffect("fall.mp3");
            pDragNode->setTouchEnabled(false);
            pDragNode->setPosition(m_pPlateDown->getPosition());
            pDragNode->setVisible(false);
            
            m_PlateDownVector.push_back(pDragNode);
            
            auto iter = std::find(m_SausageVector.begin(), m_SausageVector.end(), pDragNode);
            if (iter != m_SausageVector.end())
            {
                m_SausageVector.erase(iter);
            }
//            m_SausageVector.erase(m_SausageVector.begin()+pDragNode->getTag());
            
            m_pHotdog->setTexture(localPath("make4/hotdog2.png"));
            m_bReady = false;
            m_nMoveCount = 0;
            for_each(m_PancakeVector.begin(), m_PancakeVector.end(), [=](DragNode* pPancake){
                pPancake->setTouchEnabled(false);
            });
            for_each(m_SausageVector.begin(), m_SausageVector.end(), [=](DragNode* pSausage){
                pSausage->setTouchEnabled(false);
            });
            m_pTouchLayer->setTouchLayerEnable();
            m_bCanRoll = true;
            if (m_nAddCount==0) {
                AudioHelp::getInstance()->playEffect("vo_roll_hotdog.mp3");
                m_pGuideLayer->showGuideMove(m_pHotdog->getPosition()+Vec2(0, -100),m_pHotdog->getPosition()+Vec2(0, 100));
            }
        }else{
            pDragNode->back(pDragNode->getTag());
//            pDragNode->runAction(ScaleTo::create(0.5, 0.7));
        }
        
    }
}
void RollSausageScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    dragNodeTouchEnded(pDragNode,worldPoint);
}

void RollSausageScene::TouchBegan(Touch *pTouch, Event *pEvent)
{
}

void RollSausageScene::TouchMove(Touch *pTouch, Event *pEvent)
{
    if (!m_pHotdog) {
        return;
    }
    if(!m_bCanRoll){
        return;
    }
    Rect rect = m_pHotdog->getBoundingBox();
    rect.origin = m_pHotdog->getParent()->convertToWorldSpace(rect.origin);
    
    Vec2 pos = pTouch->getLocation();
    if (rect.containsPoint(pos)) {
        m_nMoveCount++;
        int step = m_nMoveCount/10;
        if (m_nMoveCount%10==0) {
            step = m_nMoveCount/10;
            m_bCanRoll = false;
            
            if(step<4){
                m_pHotdog->setTexture(localPath("make4/hotdog"+std::to_string(step+2)+".png"));
            }
            m_pGuideLayer->removeGuide();
            if (step==3) {
                m_pTouchLayer->setTouchLayerEnable(false);
                _finishRoll();
            }
        }
    }
}

void RollSausageScene::TouchEnded(Touch *pTouch, Event *pEvent)
{
    m_bCanRoll = true;
    
}

#pragma mark - initData
void RollSausageScene::_initData()
{
    setExPath("content/make/japanese/");
    m_nNextSceneTag = GameUIEvent::eSceneTagCookHotdog;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagCookHotdog;
}


void RollSausageScene::_showPlate()
{
    auto action = Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                   CallFuncN::create([=](Node* pNode){
        DragNode* pDrag = dynamic_cast<DragNode*>(pNode);
        if (pDrag) {
            pDrag->setOrgPositionDefault();
            pDrag->setTouchEnabled(true);
        }
    }), NULL);
    m_pPlateLeft = Sprite::create(localPath("make2/plate.png"));
    this->addChildToContentLayer(m_pPlateLeft);
    CMVisibleRect::setPositionAdapted(m_pPlateLeft, 170-visibleSize.width, 680);
    m_pPlateLeft->runAction(action);
    
    for (int i = 0; i<3; i++) {
        DragNode* pSausage = _createDrageNode(localPath("make3/5.png"));
        this->addChildToContentLayer(pSausage);
        pSausage->setTag(i);
        pSausage->setLocalZOrder(i);
        pSausage->index = 0;
        pSausage->setName("sausage");
        CMVisibleRect::setPositionAdapted(pSausage, 170-visibleSize.width, 730-40*(i%3));
        m_SausageVector.push_back(pSausage);
//        pSausage->setScale(0.5);
        pSausage->runAction(action->clone());
        
    }    
}

void RollSausageScene::_showPlateRight()
{
    auto action = Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                   CallFuncN::create([=](Node* pNode){
        DragNode* pDrag = dynamic_cast<DragNode*>(pNode);
        if (pDrag) {
            pDrag->setOrgPositionDefault();
            pDrag->setTouchEnabled(true);
        }
    }), NULL);
    m_pPlateRight = Sprite::create(localPath("make2/plate.png"));
    this->addChildToContentLayer(m_pPlateRight);
    CMVisibleRect::setPositionAdapted(m_pPlateRight, 480+visibleSize.width, 680);
    m_pPlateRight->runAction(action);
    
    for (int i = 0; i<3; i++) {
        DragNode* pBread = _createDrageNode(localPath("make2/rolls7.png"));
        this->addChildToContentLayer(pBread);
        pBread->setTag(i);
        pBread->setLocalZOrder(i);
        pBread->index = 0;
        pBread->setName("pancake");
        CMVisibleRect::setPositionAdapted(pBread, 480+visibleSize.width, 700-10*(i%3));
        m_PancakeVector.push_back(pBread);
        pBread->setScale(0.7);
        pBread->runAction(action->clone());
        
    }
    
}

void RollSausageScene::_showPlateDown()
{
    auto action = MoveBy::create(0.5, Vec2(visibleSize.width, 0));
    Sprite* pPlate = Sprite::create(localPath("make4/pan.png"));
    this->addChildToContentLayer(pPlate);
    CMVisibleRect::setPositionAdapted(pPlate, 320-visibleSize.width, 350);
    pPlate->runAction(Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                             CallFunc::create([=](){
        m_pPlateDown = pPlate;
        m_bReady = true;
    }), NULL));
    
}

void RollSausageScene::_finishRoll()
{
    
    m_PlateDownVector.clear();
    m_pHotdog->runAction(Sequence::create(DelayTime::create(0.5),
                                          ScaleTo::create(0.3, 1.2),
                                          MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                          CallFunc::create([=](){
        m_nAddCount++;
        
        for_each(m_PancakeVector.begin(), m_PancakeVector.end(), [=](DragNode* pPancake){
            pPancake->setTouchEnabled(true);
        });
        for_each(m_SausageVector.begin(), m_SausageVector.end(), [=](DragNode* pSausage){
            pSausage->setTouchEnabled(true);
        });
        if (m_nAddCount==3) {
            _showFinishDisplay();
        }else{
            m_bReady = true;
        }
    }), NULL));
    
}

void RollSausageScene::_showFinishDisplay()
{
    m_pPlateLeft->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)));
    m_pPlateRight->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, 0)));
    m_pPlateDown->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, 0)));
    
    
    LayerColor* pGrayLayer = LayerColor::create(Color4B(0, 0, 0, 0));
    this->addChildToUILayer(pGrayLayer);
    pGrayLayer->runAction(FadeTo::create(0.5, 160));
    
    Sprite* pPlate = Sprite::create(localPath("make4/pan.png"));
    CMVisibleRect::setPositionAdapted(pPlate, 320+visibleSize.width, 300+50);
    
    auto action = Sequence::create(DelayTime::create(0.5),
                                   JumpBy::create(0.5, Vec2(-visibleSize.width, 0), 200, 1), NULL);
    pPlate->runAction(action);
    this->addChildToUILayer(pPlate);
    for (int i = 0; i<3; i++) {
        Sprite* pHotdog = Sprite::create(localPath("make4/hotdog5.png"));
        this->addChildToUILayer(pHotdog);
        CMVisibleRect::setPositionAdapted(pHotdog, 320+visibleSize.width, 400+50-80*(i%3));
        pHotdog->runAction(action->clone());
    }
    
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("done.mp3");
        AudioHelp::getInstance()->_playEffectNiceWork();
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
        this->addChildToUILayer(pParticle);
        pParticle->setLocalZOrder(10);
        pParticle->setPosition(visibleSize*0.5);
    }),
                                     DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<CookHotdogScene>();
    }), NULL));
}