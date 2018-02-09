

#include "PutinSausageScene.h"
#include "SceneManager.h"

PutinSausageScene::PutinSausageScene()
{
    m_pPlateRight = nullptr;
    m_nAddCount = 0;
}

PutinSausageScene::~PutinSausageScene()
{
    
}
bool PutinSausageScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    Sprite* pBg = Sprite::create("content/make/bg/bg2.jpg");
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
        AudioHelp::getInstance()->playEffect("vo_drag_sausage_bread.mp3");
        m_pGuideLayer->showGuideMove(m_pPlateRight->getPosition(), m_pPlateDown->getPosition());
    }),   NULL));
    
    m_pGameUI->showNormalLayout();
    return true;
}

void PutinSausageScene::onEnter()
{
    ExtensionScene::onEnter();
}

void PutinSausageScene::onExit()
{
    ExtensionScene::onExit();
}

void PutinSausageScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void PutinSausageScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    pDragNode->setLocalZOrder(20);
    pDragNode->getDragSprite()->stopAllActions();
    pDragNode->getDragSprite()->setScale(1);
    if (pDragNode->index==0) {
        
    }else{
        ProgressBarNew* pBar = (ProgressBarNew*)pDragNode->getUserData();
        if(pBar){
            pBar->pauseProgress();
        }
        
        pDragNode->stopAllActions();
    }
}
void PutinSausageScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    
}
void PutinSausageScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    if (!m_pPlateDown) {
        pDragNode->setTouchEnabled(false);
        pDragNode->back(pDragNode->getTag(),
                        [=](){
                            
                            pDragNode->setTouchEnabled(true);
                        });
        pDragNode->getDragSprite()->runAction(ScaleTo::create(0.5, 0.5));
        return;
    }
    m_pGuideLayer->removeGuide();
    std::string name = pDragNode->getName();
    Rect doughRect = m_pPlateDown->getBoundingBox();
    doughRect.origin = m_pPlateDown->getParent()->convertToWorldSpace(doughRect.origin);
    
    Rect rect = pDragNode->getDragSprite()->getBoundingBox();
    rect.origin = pDragNode->convertToWorldSpace(rect.origin);
    int index = m_PlateDownVector.size();
    if (name == "bread"){
        if (rect.intersectsRect(doughRect) && index==0){
            AudioHelp::getInstance()->playEffect("fall.mp3");
            pDragNode->setTouchEnabled(false);
            pDragNode->setPosition(m_pPlateDown->getPosition());
            m_PlateDownVector.push_back(pDragNode);
            if(m_nAddCount==0){
                m_pGuideLayer->showGuideMove(m_pPlateLeft->getPosition(), m_pPlateDown->getPosition());
            }
        }else{
            pDragNode->setTouchEnabled(false);
            pDragNode->back(pDragNode->getTag(),
                            [=](){
                                
                                pDragNode->setTouchEnabled(true);
                            });
            pDragNode->getDragSprite()->runAction(ScaleTo::create(0.5, 0.5));
        }
        
    }else if (name == "sausage") {
        if (rect.intersectsRect(doughRect) && index==1){
            AudioHelp::getInstance()->playEffect("fall.mp3");
            pDragNode->setTouchEnabled(false);
            pDragNode->setPosition(m_pPlateDown->getPosition()+Vec2(0,20));
            pDragNode->getDragSprite()->setScale(1.3);
            m_PlateDownVector.push_back(pDragNode);
            _finishPut();
            
        }else{
            pDragNode->setTouchEnabled(false);
            pDragNode->back(pDragNode->getTag(),
                            [=](){
                                
                                pDragNode->setTouchEnabled(true);
                            });
            pDragNode->getDragSprite()->runAction(ScaleTo::create(0.5, 0.5));
        }
        
    }
}
void PutinSausageScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    dragNodeTouchEnded(pDragNode,worldPoint);
}
#pragma mark - initData
void PutinSausageScene::_initData()
{
    setExPath("content/make/normal/");
    m_nNextSceneTag = GameUIEvent::eSceneTagDecorate;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagDecorate;
}


void PutinSausageScene::_showPlate()
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
        DragNode* pSausage = _createDrageNode(localPath("make1/sausage5.png"));
        this->addChildToContentLayer(pSausage);
        pSausage->setTag(i);
        pSausage->setLocalZOrder(i);
        pSausage->index = 0;
        pSausage->setName("sausage");
        CMVisibleRect::setPositionAdapted(pSausage, 170-visibleSize.width, 730-40*(i%3));
        m_BreadLeftVector.push_back(pSausage);
        pSausage->getDragSprite()->setScale(0.5);
        pSausage->runAction(action->clone());
        
    }    
}

void PutinSausageScene::_showPlateRight()
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
        DragNode* pBread = _createDrageNode(localPath("make4/hotdog_1.png"));
        this->addChildToContentLayer(pBread);
        pBread->setTag(i);
        pBread->setLocalZOrder(i);
        pBread->index = 0;
        pBread->setName("bread");
        CMVisibleRect::setPositionAdapted(pBread, 480+visibleSize.width, 730-40*(i%3));
        m_BreadLeftVector.push_back(pBread);
        pBread->getDragSprite()->setScale(0.5);
        pBread->runAction(action->clone());
        
    }
    
}

void PutinSausageScene::_showPlateDown()
{
    auto action = MoveBy::create(0.5, Vec2(visibleSize.width, 0));
    Sprite* pPlate = Sprite::create(localPath("make4/plate.png"));
    this->addChildToContentLayer(pPlate);
    CMVisibleRect::setPositionAdapted(pPlate, 320-visibleSize.width, 350);
    pPlate->runAction(Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                             CallFunc::create([=](){
        m_pPlateDown = pPlate;
    }), NULL));
    
}

void PutinSausageScene::_finishPut()
{
    auto action = Sequence::create(DelayTime::create(1.2),
                                   MoveBy::create(0.5, Vec2(visibleSize.width, 0)), NULL);
    for_each(m_PlateDownVector.begin(), m_PlateDownVector.end(), [=](DragNode* pNode){
        pNode->runAction(action->clone());
    });
    m_pPlateDown->runAction(action);
    
    m_pPlateDown = nullptr;
    this->runAction(Sequence::create(DelayTime::create(1.2),
                                     CallFunc::create([=](){
        
        m_PlateDownVector.clear();
        m_nAddCount++;
        
        if (m_nAddCount==3) {
            _showFinishDisplay();
        }else{            
            _showPlateDown();
        }
    }), NULL));
    
}

void PutinSausageScene::_showFinishDisplay()
{
    m_pPlateLeft->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)));
    m_pPlateRight->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, 0)));
    
    LayerColor* pGrayLayer = LayerColor::create(Color4B(0, 0, 0, 0));
    this->addChildToUILayer(pGrayLayer);
    pGrayLayer->runAction(FadeTo::create(0.5, 160));
    
    for (int i = 0; i<3; i++) {
        Sprite* pPlate = Sprite::create(localPath("make4/plate.png"));
        this->addChildToUILayer(pPlate);
        
        Sprite* pHotdog = Sprite::create(localPath("make4/hotdog.png"));
        this->addChildToUILayer(pHotdog);
        if (i==0) {
            CMVisibleRect::setPositionAdapted(pPlate, 320+visibleSize.width, 540);
        }else if (i==1) {
            CMVisibleRect::setPositionAdapted(pPlate, 180+visibleSize.width, 300);
        }else if (i==2) {
            CMVisibleRect::setPositionAdapted(pPlate, 460+visibleSize.width, 300);
        }
        pHotdog->setPosition(pPlate->getPosition());
        pPlate->setScale(0.5);
        pHotdog->setScale(0.5);
        
        auto action = Sequence::create(DelayTime::create(0.5),
                                       JumpBy::create(0.5, Vec2(-visibleSize.width, 0), 200, 1), NULL);
        pPlate->runAction(action);
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
        SceneManager::replaceTheScene<DecorateScene>();
    }), NULL));
}