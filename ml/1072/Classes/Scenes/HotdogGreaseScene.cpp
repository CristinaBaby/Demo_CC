
#include "HotdogGreaseScene.h"
#include "SceneManager.h"

HotdogGreaseScene::HotdogGreaseScene()
{
    m_nAddOilCount = 0;
    m_pCurSausage = nullptr;
    m_nMoveCount = 0;
    m_nFinishCount = 0;
}

HotdogGreaseScene::~HotdogGreaseScene()
{
    
}
bool HotdogGreaseScene::init()
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
    
    _showPan();
    _showOil();
    
    m_pTouchLayer = TouchLayer::create();
    this->addChildToUILayer(m_pTouchLayer);
    m_pTouchLayer->onTouchBegan = CC_CALLBACK_2(HotdogGreaseScene::TouchBegan, this);
    m_pTouchLayer->onTouchMoved = CC_CALLBACK_2(HotdogGreaseScene::TouchMove, this);
    m_pTouchLayer->onTouchEnded = CC_CALLBACK_2(HotdogGreaseScene::TouchEnded, this);
    m_pTouchLayer->setTouchLayerEnable(false);
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    
    m_pGameUI->showNormalLayout();
    return true;
}

void HotdogGreaseScene::onEnter()
{
    ExtensionScene::onEnter();
}

void HotdogGreaseScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void HotdogGreaseScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if (name=="oil_spoon"){
        if (pDragNode->index==0){
            pDragNode->index = 1;
            pDragNode->setDragSprite(Sprite::create(localPath("oil_spoon1_1.png")));
            Sprite* pOil = (Sprite*)pDragNode->getChildByName("oil");
            if (!pOil) {
                pOil = Sprite::create(localPath("oil_spoon1_2.png"));
                pDragNode->addChild(pOil);
                pOil->setName("oil");
            }
            pOil->setPosition(Vec2::ZERO);
            pOil->setScale(1);
            pOil->setOpacity(255);
        }
        
    }
}

void HotdogGreaseScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name=="oil_spoon") {
        if (pDragNode->index==1){
            Rect rect = m_pPan->getBoundingBox();
            if (rect.containsPoint(pDragNode->getPosition())){
                pDragNode->setTouchEnabled(false);
                pDragNode->setPosition(m_pOil->getPosition()+Vec2(50,200));
                
                AudioHelp::getInstance()->playEffect("add_water.mp3");
                if(m_nAddOilCount==0){
                    m_pOil->setScale(0.2);
                    m_pOil->runAction(Sequence::create(FadeIn::create(1),
                                                       ScaleTo::create(1, 0.6), NULL));
                }else if (m_nAddOilCount==1) {
                    m_pOil->runAction(ScaleTo::create(2, 1));
                }
                AudioHelp::getInstance()->playEffect("add_water.mp3");
                
                Sprite* pOilStream = Sprite::create(localPath("oil_spoon1_3.png"));
                pDragNode->addChild(pOilStream);
                pOilStream->setName("stream");
                pOilStream->runAction(AnimationHelp::createAnimate(localPath("oil_spoon1_"), 3, 4,false));
                
                Node* pOil = pDragNode->getChildByName("oil");
                if (pOil) {
                    pOil->runAction(Sequence::create(DelayTime::create(0.5),
                                                     Spawn::create(MoveBy::create(2, Vec2(-60, -20)),
                                                                   ScaleTo::create(2, 0.3), NULL),
                                                     FadeOut::create(0.3),
                                                     CallFunc::create([=](){
                        pOilStream->removeFromParent();
                        if(m_nAddOilCount==0){
                            pDragNode->back(1,[=](){
                                pDragNode->index = 0;
                                pDragNode->setTouchEnabled(true);
                                pDragNode->setDragSprite(Sprite::create(localPath("oil_spoon_1.png")));
                            });
                        }else if (m_nAddOilCount==1){
                            _finishAddOil();
                        }
                        m_nAddOilCount++;
                    }), NULL));
                }
            }
        }
    }
}

void HotdogGreaseScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    std::string name = pDragNode->getName();
    if (name=="sausage"){
        if (!m_pCurSausage) {
            if (pDragNode->index==0) {
                Rect rect = m_pPan->getBoundingBox();
                rect.origin = m_pPan->getParent()->convertToWorldSpace(rect.origin);
                Rect rect2 = pDragNode->getDragSprite()->getBoundingBox();
                rect2.origin = pDragNode->convertToWorldSpace(rect2.origin);
                if (rect.intersectsRect(rect2)) {
                    m_pCurSausage = pDragNode;
                    Sprite* pSausage = Sprite::create(localPath("sausage1.png"));
                    pSausage->setName("sausage");
                    m_pSausageGrid->addChild(pSausage);
                    
                    pDragNode->setTouchEnabled(false);
                    pDragNode->setVisible(false);
                    pDragNode->setOrgPositionDefault();
                    pDragNode->index = 1;
                    
                    pDragNode->getDragSprite()->setTexture(localPath("sausage5.png"));
                    pDragNode->setPosition(m_pPan->getPosition());
                    m_pGuideLayer->showGuideMove(m_pPan->getPosition(), m_pPan->getPosition()+Vec2(0, -100));
                    m_pTouchLayer->setTouchLayerEnable();
                    if (m_nFinishCount ==0) {
                        AudioHelp::getInstance()->playEffect("vo_hotdog_wrap_oil.mp3");
                    }
                }else{
                    pDragNode->back(pDragNode->getTag());
                }
            }
        }else{
            if (pDragNode->index==0){
                pDragNode->back(pDragNode->getTag());
            }else{
                Rect rect = m_pPlateLeft->getBoundingBox();
                rect.origin = m_pPlateLeft->getParent()->convertToWorldSpace(rect.origin);
                Rect rect2 = pDragNode->getDragSprite()->getBoundingBox();
                rect2.origin = pDragNode->convertToWorldSpace(rect2.origin);
                if (rect.intersectsRect(rect2)) {
                    pDragNode->setTouchEnabled(false);
                    CMVisibleRect::setPosition(pDragNode, 150, 850-60*m_nFinishCount);
                    m_nFinishCount++;
                    m_pCurSausage = nullptr;
                    if (m_nFinishCount==3) {
                        _finishAll();
                    }
                    
                }else{
                    pDragNode->back(pDragNode->getTag());
                }
            }
        }
    }
    
}

void HotdogGreaseScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

void HotdogGreaseScene::TouchBegan(Touch *pTouch, Event *pEvent)
{
}

void HotdogGreaseScene::TouchMove(Touch *pTouch, Event *pEvent)
{
    Rect rect = m_pPan->getBoundingBox();
    Vec2 pos = pTouch->getLocation();
    if (rect.containsPoint(pos)) {
        m_nMoveCount++;
        static int count = 180;
        auto action = m_pSausageGrid->getActionManager()->getActionByTag(100, m_pSausageGrid);
        if (!action) {
            action = RepeatForever::create(Sequence::create(Waves::create(1, Size(5, 5), 2, 5, true, true),
                                                            DelayTime::create(0.1), NULL));
            action->setTag(100);
            m_pSausageGrid->runAction(action);
        }
        if (m_nMoveCount%10==0){
            static int k = 0;
            Sprite* pSprite = Sprite::create(localPath("oil_particle"+std::to_string((k++%3)+1)+".png"));
            this->addChildToContentLayer(pSprite);
            pSprite->setPosition(m_pPan->getPosition()+Vec2(-50.0+10*(arc4random()%10), -50.0+10*(arc4random()%10)));
            pSprite->runAction(Sequence::create(JumpBy::create(0.5, Vec2::ZERO, 20, 1),
                                                FadeOut::create(0.5), NULL));
        }
        if (m_nMoveCount%count==0) {
            int step = m_nMoveCount/count;
            Sprite* pSausage = (Sprite*)m_pSausageGrid->getChildByName("sausage");
            if (pSausage) {
                pSausage->setTexture(localPath("sausage"+std::to_string(step+1)+".png"));
            }
            m_pOil->setOpacity(m_pOil->getOpacity()-0.5);
            m_pGuideLayer->removeGuide();
            if (step==4) {
                m_pTouchLayer->setTouchLayerEnable(false);
                m_pCurSausage->setVisible(true);
                m_pCurSausage->setTouchEnabled(true);
                m_pSausageGrid->removeAllChildren();
                m_nMoveCount = 0;
                if(m_nFinishCount==0){
                    _showPlateLeft();
                }
                m_pGuideLayer->showGuideMove(m_pPan->getPosition(), m_pPlateLeft->getPosition(),1);
            }
        }
    }
}

void HotdogGreaseScene::TouchEnded(Touch *pTouch, Event *pEvent)
{
    m_pSausageGrid->stopAllActions();
    m_pGuideLayer->removeGuide();
    
}



#pragma mark - initData
void HotdogGreaseScene::_initData()
{
    setExPath("content/make/hotdog/2/");
}

void HotdogGreaseScene::_showPan()
{
    m_pPan = Sprite::create(localPath("plate.png"));
    CMVisibleRect::setPosition(m_pPan, 320-visibleSize.width, 380);
    this->addChildToContentLayer(m_pPan);
    m_pPan->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    
    m_pOil = Sprite::create(localPath("pan_oil.png"));
    CMVisibleRect::setPosition(m_pOil, 320, 380);
    this->addChildToContentLayer(m_pOil);
    m_pOil->setOpacity(0);
    
    m_pSausageGrid = NodeGrid::create();
    this->addChildToContentLayer(m_pSausageGrid);
    m_pSausageGrid->setPosition(m_pOil->getPosition());
}

void HotdogGreaseScene::_showOil()
{
    AudioHelp::getInstance()->playEffect("vo_add_hotdog_oil.mp3");
    m_pOilBowl = Sprite::create(localPath("oil.png"));
    CMVisibleRect::setPosition(m_pOilBowl, 240+visibleSize.width, 790);
    this->addChildToContentLayer(m_pOilBowl);
    m_pOilBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
    m_pOilSpoon = _createDrageNode(localPath("oil_spoon_1.png"));
    CMVisibleRect::setPosition(m_pOilSpoon, 260+visibleSize.width, 770);
    this->addChildToContentLayer(m_pOilSpoon);
    m_pOilSpoon->setName("oil_spoon");
    
    m_pOilSpoon->setTouchEnabled(false);
    m_pOilSpoon->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                            CallFunc::create([=](){
        m_pOilSpoon->setOrgPositionDefault();
        m_pOilSpoon->setTouchEnabled(true);
    }), NULL));
}

void HotdogGreaseScene::_showPlateLeft()
{
    m_pPlateLeft = Sprite::create(localPath("plate_small.png"));
    this->addChildToContentLayer(m_pPlateLeft);
    CMVisibleRect::setPosition(m_pPlateLeft, 150-visibleSize.width, 770);
    m_pPlateLeft->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    
}

void HotdogGreaseScene::_showPlateRight()
{
    AudioHelp::getInstance()->playEffect("vo_hotdog_sausage2pan.mp3");
    m_pPlateRight = Sprite::create(localPath("plate_small.png"));
    this->addChildToContentLayer(m_pPlateRight);
    CMVisibleRect::setPosition(m_pPlateRight, 480+visibleSize.width, 770);
    m_pPlateRight->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    
    for(int i = 0;i<3;i++){
        DragNode* pSausage = _createDrageNode(localPath("sausage1.png"));
        this->addChildToContentLayer(pSausage);
        CMVisibleRect::setPosition(pSausage, 480+visibleSize.width, 850-60*i);
        pSausage->setTouchEnabled(false);
        pSausage->setName("sausage");
        m_SausageVector.push_back(pSausage);
        pSausage->setTag(i);
        pSausage->setLocalZOrder(i);
        pSausage->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                             CallFunc::create([=](){
            pSausage->setOrgPositionDefault();
            pSausage->setTouchEnabled(true);
            if (i==2) {
                m_pGuideLayer->showGuideMove(m_pPlateRight->getPosition(), m_pPan->getPosition());
            }
        }), NULL));
    }
}

void HotdogGreaseScene::_finishAddOil()
{
    m_pOilSpoon->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    m_pOilBowl->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));

    _showPlateRight();
    
}

void HotdogGreaseScene::_finishAll()
{
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<HotdogGrillSausageScene>();
    }), NULL));
}