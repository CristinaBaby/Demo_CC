
#include "MakeCottonScene.h"
#include "SceneManager.h"
#include "IAPManager.h"

static Vec2 gShapePos[4] = {
    Vec2(190,22),
    Vec2(460,6),
    Vec2(695,6),
    Vec2(892,24),
};


static Vec2 gCandyAnc[4] ={
    Vec2(0.75, 0.75),
    Vec2(0.6, 0.7),
    Vec2(0.66, 0.64),
    Vec2(0.68, 0.71),
};

MakeCottonScene::MakeCottonScene()
{
    m_pSilk = nullptr;
    m_pCotton = nullptr;
    m_bStickReady = false;
}

MakeCottonScene::~MakeCottonScene()
{
    
}
bool MakeCottonScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    //    bg
    Sprite* pBg = Sprite::create(localPath("bg.png"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    m_pBg = pBg;
    
    m_pTable = Sprite::create(localPath("table1.png"));
    CMVisibleRect::setPositionAdapted(m_pTable, 480, 100,kBorderNone,kBorderBottom);
    this->addChildToBGLayer(m_pTable);
    
    m_pShed = Sprite::create(localPath("awnings.png"));
    CMVisibleRect::setPosition(m_pShed, 480, 0,kBorderNone,kBorderTop);
    this->addChildToBGLayer(m_pShed);
    m_pBg = pBg;
    
    _showFlavorUI();
    
    AudioHelp::getInstance()->playBackGroundMusic("bg_make.mp3");
    m_pGameUI->showNormalLayout();
    return true;
}

void MakeCottonScene::onEnter()
{
    ExtensionScene::onEnter();
    Analytics::getInstance()->sendScreenEvent(Flurry_EVENT_MAKE);
}
#pragma mark - initData
void MakeCottonScene::_initData()
{
    setExPath("content/make/");
    m_nNextSceneTag = GameUIEvent::eSceneTagDecorate;
    m_nStepCount = 0;
    m_nMoveCount = 0;
    m_nFlag = 0;
}

void MakeCottonScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    if (m_pFlavorScroll) {
        Vector<Node*> pChildren = m_pFlavorScroll->getInnerContainer()->getChildren();
        for_each(pChildren.begin(), pChildren.end(), [=](Node* pNode){
            LockItem* pItem = dynamic_cast<LockItem*>(pNode);
            if (pItem) {
                int type = pItem->lockType;if (IAPManager::getInstance()->getItemIsBought(0)){
                    pItem->unlock();
                }else if (IAPManager::getInstance()->getItemIsBought(1)) {
                        if (type==0) {
                            pItem->unlock();
                        }
                    }
            }
        });
    }
}
void MakeCottonScene::onSwitchStateCallback(bool on)
{
    if (on) {
        m_pGuideLayer->removeGuide();
        m_pMachine->setSwitchEnable(false);
        AudioHelp::getInstance()->stopLoopEffect();
        m_pMachine->runAction(Sequence::create(DelayTime::create(2),
                                               MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                               CallFunc::create([=](){
            _finishMakeSilk();
        }), NULL));
        
//        m_pMachine->runAction(Spawn::create(ScaleTo::create(1, 1),
//                                            MoveBy::create(0.8, Vec2(0, -200)), NULL));
//        _foreground();
        
    }
}

void MakeCottonScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    int tag = pDragNode->getTag();
    if (tag==eDragTagStick) {
        if (m_bStickReady) {
            pDragNode->setLocalZOrder(5);
            if (!m_pSilk) {
                std::stringstream ostr;
                ostr<<"content/make/make/silk/silk"<<m_nFlavorIndex+1<<"_";
                m_pSilk = Sprite::create(string(ostr.str()).append("1.png"));
                m_pSilk->runAction(AnimationHelp::createAnimate(ostr.str(), 1, 4,false,true,0.5));
                m_pMachine->m_pOutClp->addChild(m_pSilk,1);
                //        m_pSilk->setAnchorPoint(Vec2(0.5, 1));
                m_pSilk->setPosition(m_pSilk->getParent()->convertToNodeSpace(worldPoint));
            }
            auto pAction = RepeatForever::create(Sequence::create(CallFunc::create([=](){
                m_pSilk->setScaleX(-1);
            }),DelayTime::create(0.1),
                                                                  CallFunc::create([=](){
                m_pSilk->setScaleX(1);
            }),DelayTime::create(0.1), NULL));
            pAction->setTag(10);
            m_pSilk->runAction(pAction);
        }else{
            pDragNode->setLocalZOrder(10);
            m_pStick->runAction(MoveTo::create(0.5, Vec2(100, 230)));
            m_pStick->runAction(ScaleTo::create(1, 1.4));
            m_pStick->runAction(RotateBy::create(1, -130));
            m_pStick->setTouchEnabled(false);
            m_pMachine->runAction(Spawn::create(ScaleTo::create(1, 1),
                                                MoveBy::create(0.8, Vec2(0, -200)),
                                                CallFunc::create([=](){
                
                pDragNode->setLocalZOrder(5);
                Rect rect = Rect(-373+290, -373+350, 280, 300);
                rect.origin = m_pMachine->convertToWorldSpace(rect.origin);
                m_pStick->setLimitRect(rect);
                m_pStick->setTouchEnabled(true);
            }), NULL));
            _foreground();
            m_bStickReady = true;
//            m_pStick->setLimitRect(Rect(330, 110, 480, 300));
//            m_pStick->setLimitRect(Rect(110, 360, 480, 300));
            
            if (!m_pSilk) {
                std::stringstream ostr;
                ostr<<"content/make/make/silk/silk"<<m_nFlavorIndex+1<<"_";
                m_pSilk = Sprite::create(string(ostr.str()).append("1.png"));
                m_pSilk->runAction(AnimationHelp::createAnimate(ostr.str(), 1, 4,false,true,0.5));
                m_pMachine->m_pOutClp->addChild(m_pSilk,1);
                //        m_pSilk->setAnchorPoint(Vec2(0.5, 1));
                m_pSilk->setPosition(m_pSilk->getParent()->convertToNodeSpace(worldPoint));
            }
            auto pAction = RepeatForever::create(Sequence::create(CallFunc::create([=](){
                m_pSilk->setScaleX(-1);
            }),DelayTime::create(0.1),
                                                                  CallFunc::create([=](){
                m_pSilk->setScaleX(1);
            }),DelayTime::create(0.1), NULL));
            pAction->setTag(10);
            m_pSilk->runAction(pAction);
        }
    }else{
        pDragNode->setLocalZOrder(10);
    }
}
void MakeCottonScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    int tag = pDragNode->getTag();
    if (tag==eDragTagStick){
        if (m_bStickReady){
            _makeCandy(pDragNode,worldPoint);
        }
        
    }
}

void MakeCottonScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    int tag = pDragNode->getTag();
    if (eDragTagFlavorBottle==tag) {
//        Rect rect = Rect(260, 280, 390, 160);
//        rect.origin = CMVisibleRect::getPositionAdapted(rect.origin);
        Rect rect = Rect(-373+150, -373+350, 470, 300);
        rect.origin = m_pMachine->convertToWorldSpace(rect.origin);
        if (rect.containsPoint(worldPoint)) {
            pDragNode->setTouchEnabled(false);
            this->runAction(Sequence::create(DelayTime::create(0.5),
                                             CallFunc::create([=](){
                _pourSugar();
                pDragNode->removeFromParent();
            }), NULL));
        }
    }else if (tag==eDragTagStick){
        if(m_pSilk){
            m_pSilk->stopActionByTag(10);
        }
    }
}
void MakeCottonScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

void MakeCottonScene::_distant(){
    m_pBg->runAction(ScaleTo::create(0.8, 1));
    m_pTable->runAction(MoveBy::create(0.8, Vec2(1, 300)));
    m_pShed->runAction(MoveBy::create(0.8, Vec2(1, -50)));
}

void MakeCottonScene::_foreground()
{
    m_pBg->runAction(ScaleTo::create(1, 1.2));
    m_pTable->runAction(MoveBy::create(1, Vec2(1, -300)));
    m_pShed->runAction(MoveBy::create(1, Vec2(1, 50)));
}

void MakeCottonScene::_showFlavorUI()
{
    AudioHelp::getInstance()->playEffect("vo_choose_flavor.mp3");
    m_pBoard = Sprite::create(localPath("table2.png"));
    this->addChildToContentLayer(m_pBoard);
    CMVisibleRect::setPositionAdapted(m_pBoard, 480, 400);
    
    m_pFlavorScroll = ExpandScrollView::create();
    m_pFlavorScroll->setContentSize(Size(visibleSize.width,350));
    this->addChildToContentLayer(m_pFlavorScroll);
    CMVisibleRect::setPositionAdapted(m_pFlavorScroll, 0, 320,kBorderLeft);
    m_pFlavorScroll->setClippingEnabled(false);
    m_pFlavorScroll->setScrollBarAutoHideEnabled(true);
    m_pFlavorScroll->setScrollBarEnabled(false);
    
    //bottle用了clippingNode m_pFlavorScroll 要将ClippingEnabled设置为false  不然 bottle会被截掉
    int count = 15;
    bool networkAvailable = SSCInternalLibManager::getInstance()->checkNetworkAvailable();
    for (int i = 0; i<count; i++) {
        LockItem* pItem = LockItem::create();
        string lockName = "content/common/lock.png";
        bool locked = true;
        int type = 0;
        int freeCount = 5;
        int videoCount = 5;
        if (!networkAvailable) {
            videoCount = 0;
        }
        if (IAPManager::getInstance()->getItemIsBought(0)){
            freeCount = count;
        }
        if (i>=freeCount){
            type = (i-freeCount)%2;
            if (i+1>videoCount*2+freeCount) {
                type = 0;
            }
            if (type==1) {
                if (IAPManager::getInstance()->getItemIsBought(0)) {
                    lockName = "";
                    locked = false;
                }else{
                    if (ConfigManager::getInstance()->getVideoUnLocked("flavor", i)){
                        lockName = "";
                        locked = false;
                    }else{
                        lockName = "content/common/video.png";
                    }
                    
                }
            }else{
                if (IAPManager::getInstance()->getItemIsBought(1) || gNoneIap) {
                    lockName = "";
                    locked = false;
                }
            }
        }else{
            lockName = "";
            locked = false;
        }
        pItem->initWithName(localPath("flavor/cup_down.png"), lockName,locked);
        
        if (type==1 && videoCount>0) {
            if (!ConfigManager::getInstance()->getVideoUnLocked("flavor", i)){
                LockUIManager::getInstance()->registerLock("flavor", i, pItem);
                LockUIManager::getInstance()->setLockOriginPos(Vec2(50, -80));
            }
        }
        Sprite* Lock = pItem->getLockSprite();
        if (Lock) {
            Lock->setPosition(Vec2(50, -80));
            if (type == 0) {
                Lock->setTag(TAG_IAP_LOCK);
            }else{
                Lock->setTag(TAG_REWARD_LOCK);
            }
        }
        
        pItem->lockType = type;
        
        pItem->setTag(i);
        pItem->setScale(0.8);
        
        SugarBottle* pBottle = SugarBottle::create();
        pBottle->initSugar(i);
        pItem->addChild(pBottle);
        pBottle->setName("bottol");
        
        
        m_pFlavorScroll->getInnerContainer()->addChild(pItem);
        pItem->setPosition(Vec2(70+150*i, 150));
        pItem->onItemClicked = CC_CALLBACK_1(MakeCottonScene::_onFlavorClicked, this);

    }
    m_pFlavorScroll->getInnerContainer()->setContentSize(Size(70+150*count, m_pFlavorScroll->getContentSize().height));
    m_pFlavorScroll->setTouchEnabled(false);
    m_pFlavorScroll->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    m_bItemEnable = false;
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        m_pFlavorScroll->scrollToRight(3, true);
    }),
                                     DelayTime::create(3),
                                     CallFunc::create([=](){
        m_pFlavorScroll->scrollToLeft(1, false);
    }),
                                     DelayTime::create(1.2),
                                     CallFunc::create([=](){
        
        m_pFlavorScroll->setTouchEnabled(true);
        m_bItemEnable = true;
    }), NULL));
}

void MakeCottonScene::_removeFlavorBottle()
{
    DragNode* pNode = _createDrageNode(localPath("flavor/cup_down.png"));
    this->addChildToContentLayer(pNode);
    pNode->setTag(eDragTagFlavorBottle);
    
    SugarBottle* pBottle = SugarBottle::create();
    pBottle->initSugar(m_nFlavorIndex);
    pNode->addChild(pBottle);
    pBottle->setName("bottol");
    
    Vector<Node*> pChildren = m_pFlavorScroll->getInnerContainer()->getChildren();
    for_each(pChildren.begin(), pChildren.end(), [=](Node* pChild){
        LockItem* pItem = dynamic_cast<LockItem*>(pChild);
        if (pItem) {
            Vec2 pos = pItem->getParent()->convertToWorldSpace(pItem->getPosition());
            if (pItem->getTag()==m_nFlavorIndex) {
                pItem->setVisible(false);
                pos = m_pContentLayer->convertToNodeSpace(pos);
                pNode->setPosition(pos);
            }else{
                int tag = pItem->getTag();
                if (tag<m_nFlavorIndex) {
                    pItem->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
                }else{
                    pItem->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
                }
            }
        }
    });
    pNode->setTouchEnabled(false);
    pNode->runAction(Sequence::create(DelayTime::create(1),
                                      MoveTo::create(0.5, Vec2(visibleSize.width/2, pNode->getPositionY())),
                                      CallFunc::create([=](){
        m_pBoard->runAction(MoveBy::create(0.5, Vec2(0, 70)));
    }),MoveBy::create(0.5, Vec2(0, 80)),
                                      CallFunc::create([=](){
        pNode->setOrgPositionDefault();
        pNode->setTouchEnabled(true);
        AudioHelp::getInstance()->playEffect("vo_pour_sugar.mp3");
    }), NULL));
}

void MakeCottonScene::_pourSugar()
{
    
    m_pBoard->runAction(Sequence::create(EaseBackIn::create(MoveBy::create(1, Vec2(0, visibleSize.height))),
                                         CallFunc::create([=](){
        m_pBoard->removeFromParent();
        m_pBoard = nullptr;
    }), NULL));
    
    m_pSugarBottle = SugarBottle::create();
    m_pSugarBottle->initSugar(m_nFlavorIndex);
    this->addChildToContentLayer(m_pSugarBottle);
    CMVisibleRect::setPosition(m_pSugarBottle, 300, 400+95);
    
    m_pSugarBottle->runAction(Sequence::create(ScaleTo::create(0.5, 1.3),
                                               CallFunc::create([=](){
        m_pSugarBottle->pourSugar();
        m_pMachine->addSugar(m_nFlavorIndex);
    }),
                                               DelayTime::create(2),
                                               MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                               CallFunc::create([=](){
        _finishPourSugar();
    }), NULL));
}

void MakeCottonScene::_showShapeUI()
{
    AudioHelp::getInstance()->playEffect("vo_choose_shape.mp3");
    m_pRope = Sprite::create(localPath("rope.png"));
    this->addChildToContentLayer(m_pRope);
    CMVisibleRect::setPositionAdapted(m_pRope, 480, 500+visibleSize.height/2,kBorderNone,kBorderTop);
    m_pRope->runAction(Sequence::create(EaseBackOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height/2))),
                                        CallFunc::create([=](){
        
        int shapeCount = 4;
        bool networkAvailable = SSCInternalLibManager::getInstance()->checkNetworkAvailable();
        for (int i = 0; i<shapeCount; i++) {
            LockItem* pItem = LockItem::create();
            std::stringstream ostr;
            ostr<<"candy"<<i+1<<"/"<<m_nFlavorIndex+1<<".png";
            if (i==3) {
                if (!networkAvailable) {
                    pItem->initWithName(localPath("sack.png"), "content/common/lock.png",true);
                    Sprite* Lock = pItem->getLockSprite();
                    if (Lock) {
                        Lock->setTag(TAG_IAP_LOCK);
                    }
                    pItem->lockType = 0;
                }else{
                    if (IAPManager::getInstance()->getItemIsBought(0)) {
                        pItem->initWithName(localPath("sack.png"), "",false);
                    }else{
                        if (!ConfigManager::getInstance()->getVideoUnLocked("shape", i)) {
                            pItem->initWithName(localPath("sack.png"), "content/common/video.png",true);
                            LockUIManager::getInstance()->registerLock("shape", i, pItem);
                            LockUIManager::getInstance()->setLockOriginPos(Vec2(40, -60));
                            Sprite* Lock = pItem->getLockSprite();
                            if (Lock) {
                                Lock->setTag(TAG_REWARD_LOCK);
                            }
                            pItem->lockType = 1;
                        }else{
                            pItem->initWithName(localPath("sack.png"), "",false);
                        }                        
                    }
                }
            }else{
                pItem->initWithName(localPath("sack.png"), "",false);
            }
            pItem->getContentSprite()->setAnchorPoint(Vec2(0.5, 245.0/pItem->getContentSprite()->getContentSize().height));
            Sprite* pSprite = Sprite::create(localPath("make/")+ostr.str());
            pItem->addChild(pSprite,-1);
            pSprite->setPosition(Vec2(0, -115));
            pSprite->setScale(0.4);
            if (i==1) {
                pSprite->setRotation(130);
            }else{
                
                pSprite->setRotation(150);
            }
            
            Sprite* pBowknot= Sprite::create(localPath("sack_1.png"));
            pItem->addChild(pBowknot);
            pBowknot->setName("bowknot");
            
            Sprite* pLock = pItem->getLockSprite();
            if (pLock) {
                pLock->setPosition(Vec2(40, -60));
            }
            
            pItem->setTag(i);
            this->addChildToContentLayer(pItem);
            Vec2 pos = gShapePos[i];
            pos = m_pContentLayer->convertToNodeSpace(m_pRope->convertToWorldSpace(pos));
            pItem->setPosition(pos+Vec2(visibleSize.width,50));
            
            float rotation = -5+0.25*(arc4random()%20);
            pItem->runAction(RotateBy::create(1, rotation));
            pItem->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, -50)),
                                              CallFunc::create([=](){
                m_bItemEnable = true;
                pItem->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.5+i*0.2),
                                                                        RotateBy::create(2, -10),
                                                                        RotateBy::create(2, 10), NULL)));
            }), NULL));
            
            pItem->onItemClicked = CC_CALLBACK_1(MakeCottonScene::_onShapeClicked, this);
            m_ShapeItems.pushBack(pItem);
        }
    }), NULL));
    
}


void MakeCottonScene::_removeShapeBag()
{
    m_pRope->runAction(EaseBackIn::create(MoveBy::create(1, Vec2(0, visibleSize.height/2))));
    for_each(m_ShapeItems.begin(), m_ShapeItems.end(), [=](LockItem* pItem){
        int tag = pItem->getTag();
        if (tag==m_nShapeIndex) {
            pItem->stopAllActions();
            pItem->runAction(Sequence::create(MoveTo::create(0.5, Vec2(visibleSize.width*0.5,visibleSize.height*0.5+130)),
                                              CallFunc::create([=](){
                _openShaopBag(pItem);
            }), NULL));
        }else{
            pItem->stopAllActions();
            if (tag<m_nShapeIndex) {
                pItem->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
            }else{
                pItem->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
            }
        }
    });
}

void MakeCottonScene::_openShaopBag(LockItem* pItem)
{
    Node*pBowknot = pItem->getChildByName("bowknot");
    if(pBowknot) {
        pBowknot->runAction(Sequence::create(MoveBy::create(0.1, Vec2(-5, -5)),
                                             MoveBy::create(0.1, Vec2(5, 5)),
                                             MoveBy::create(0.1, Vec2(-5, -5)),
                                             MoveBy::create(0.1, Vec2(5, 5)),
                                             MoveBy::create(0.5, Vec2(100, visibleSize.height)),
                                             CallFunc::create([=](){
            pBowknot->removeFromParent();
        }), NULL));
    }
    Node*pBag = pItem->getContentSprite();
//    pItem->getChildByName("bag");
    if(pBag){
        pBag->runAction(Sequence::create(DelayTime::create(1),
                                         ScaleTo::create(0.2, 1,1.2),
                                         ScaleTo::create(0.2, 1),
                                         ScaleTo::create(0.2, 1.2,1),
                                         ScaleTo::create(0.1, 1),
                                         ScaleTo::create(0.2, 1.2,1),
                                         ScaleTo::create(0.1, 1),
                                         CallFunc::create([=](){
            pBag->setVisible(false);
            pItem->runAction(JumpBy::create(0.3, Vec2::ZERO, 100, 1));
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/star0.plist");
            this->addChildToUILayer(pParticle);
            pParticle->setPosition(visibleSize*0.5);
            pParticle->runAction(Sequence::create(DelayTime::create(9), NULL));
            AudioHelp::getInstance()->playEffect("boxopen.mp3");
            pItem->runAction(Sequence::create(Repeat::create(Sequence::create(DelayTime::create(0.5),
                                                                              ScaleTo::create(0.5, 2),
                                                                              ScaleTo::create(0.5, 2.1), NULL),2),
                                              CallFunc::create([=](){
                pParticle->removeFromParent();
            }),
                                              CallFunc::create([=](){
                _finishChooseShap();
                pItem->removeFromParent();
            }), NULL));
        }), NULL));
    }
}

void MakeCottonScene::_showMakeCandyUI()
{
    
}

void MakeCottonScene::_showStick()
{
    m_pStick = _createDrageNode("content/make/stick.png");
    m_pMachine->addChild(m_pStick);
    m_pStick->setPosition(visibleSize);
    m_pStick->setLocalZOrder(5);
    m_pStick->setTag(eDragTagStick);
    
    m_pStick->setTouchEnabled(false);
    m_pStick->setPosition(Vec2(visibleSize.width+500, visibleSize.height-80));
    m_pStick->setActionPoint(Vec2(22, 240));
    m_pStick->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, -visibleSize.height)),
                                         CallFunc::create([=]()
                                                          {
                                                              AudioHelp::getInstance()->playEffect("vo_drag_stick.mp3");
                                                              m_pStick->setTouchEnabled(true);
                                                          }), NULL));
}

void MakeCottonScene::_showMachine()
{
    m_pMachine = CottonMachine::create();
    this->addChildToContentLayer(m_pMachine);
    CMVisibleRect::setPositionAdapted(m_pMachine, 480+visibleSize.width, 320,kBorderNone,kBorderBottom);
    m_pMachine->setScale(0.5);
    m_pMachine->setSwitchEnable(false);
    m_pMachine->onSwitchTurnOn = CC_CALLBACK_1(MakeCottonScene::onSwitchStateCallback, this);
    
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pMachine->runAction(Sequence::create(DelayTime::create(0.5),
                                           JumpBy::create(0.5, Vec2(-visibleSize.width, 0), 300, 1),
                                           DelayTime::create(0.3),
                                           Spawn::create(ScaleTo::create(1, 1),
                                                         MoveBy::create(0.8, Vec2(0, -200)),
                                                         CallFunc::create([=]()
                                                                          {
                                                                              _foreground();
                                                                          }), NULL),
                                           NULL));
}

void MakeCottonScene::_makeCandy(DragNode* pDragNode,Point worldPoint)
{
    m_nMoveCount++;
    if (m_nMoveCount==20){
        if (!m_pCotton) {
            std::stringstream ostr;
            ostr<<"content/make/make/small/"<<m_nFlavorIndex+1<<"_1.png";
            Sprite* pTempCotton = Sprite::create(string(ostr.str()));
            pDragNode->addChild(pTempCotton);
            Vec2 pos = pTempCotton->getParent()->convertToNodeSpace(worldPoint);
            pTempCotton->setPosition(Vec2(pos.x-30, pos.y));
            std::stringstream ostrAnim;
            ostrAnim<<"content/make/make/small/"<<m_nFlavorIndex+1<<"_";
            pTempCotton->runAction(AnimationHelp::createAnimate(ostrAnim.str(), 1, 3,false,true,1));
            pTempCotton->setName("temp");
        }
        
    }
    if (m_nMoveCount%200==0) {
        bool finish = m_pMachine->makeCotton();
        m_nStepCount++;
        std::stringstream ostr;
        if(m_nStepCount==1){
            ostr<<"content/make/make/candy"<<m_nShapeIndex+1<<"/"<<m_nFlavorIndex+1<<".png";
            Node* pNode = pDragNode->getChildByName("temp");
            if (pNode) {
                pNode->removeFromParent();
            }
            AudioHelp::getInstance()->playEffect("vo_make_process.mp3");
        }
        if (!m_pCotton) {
            Rect rect = Rect(-373+290+120, -373+350+50, 280-50, 300+100);
            rect.origin = m_pMachine->convertToWorldSpace(rect.origin);
            m_pStick->setLimitRect(rect);
            m_pStick->setPosition(Vec2(100, 230));
            
            m_pCotton = Sprite::create(string(ostr.str()));
            m_pMachine->m_pOutClp->addChild(m_pCotton);
            Vec2 pos = m_pCotton->getParent()->convertToNodeSpace(m_pStick->getWorldSpaceActionPoint());
            m_pCotton->setPosition(Vec2(pos.x-30, pos.y+80));
            if (m_nShapeIndex==1) {
                m_pCotton->setRotation(16);
            }
            m_pCotton->setAnchorPoint(gCandyAnc[m_nShapeIndex]);
            m_pCotton->setPosition(pos);
            m_pCotton->runAction(FadeIn::create(0.5));
        }
        if(m_nStepCount==1){
            m_pCotton->stopAllActions();
            m_pCotton->setTexture(ostr.str());
            m_pCotton->setScale(0.6);;
            m_pCotton->runAction(Sequence::create(ScaleTo::create(5, 1),
                                                  CallFunc::create([=]()
                                                                   {
                                                                       m_nFlag++;
                                                                       if (m_nFlag==2) {
                                                                           _finish();
                                                                       }
                                                                   }), NULL));
        }
        if (finish) {
            std::stringstream ostr;
            ostr<<"content/make/make/silk/silk"<<m_nFlavorIndex+1<<"_";
            m_pSilk->runAction(Sequence::create(AnimationHelp::createAnimate(ostr.str(), 4, 1,false,true,0.5),
                                                FadeOut::create(1),
                                                CallFunc::create([=]()
                                                                 {
                                                                     m_nFlag++;
                                                                     if (m_nFlag==2) {
                                                                         _finish();
                                                                     }
                                                                 }),  NULL));
        }
    }
    if (m_pCotton){
        Vec2 pos = m_pCotton->getParent()->convertToNodeSpace(worldPoint);
//        m_pCotton->setPosition(Vec2(pos.x-30, pos.y+80));
        m_pCotton->setPosition(pos);
    }
    m_pSilk->setPosition(m_pSilk->getParent()->convertToNodeSpace(worldPoint));

}

void MakeCottonScene::_finishPourSugar()
{
    m_pMachine->runAction(Sequence::create(Spawn::create(ScaleTo::create(1, 0.6),
                                                         MoveBy::create(0.8, Vec2(0, 200)),
                                                         CallFunc::create([=]()
                                                                          {
                                                                              _distant();
                                                                          }), NULL),
                                           CallFunc::create([=]()
                                                            {
                                                                m_pGuideLayer->showGuideTap(m_pMachine->getButtonWorldPosition());
                                                                m_pMachine->setSwitchEnable();
                                                            }),
                                           NULL));
}


void MakeCottonScene::_finishMakeSilk()
{
    _showShapeUI();
}

void MakeCottonScene::_finishChooseShap()
{
    m_pMachine->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                           CallFunc::create([=](){
        AudioHelp::getInstance()->playLoopEffect("machine.mp3");
        _showStick();
    }), NULL));
}

void MakeCottonScene::_finish()
{
    AudioHelp::getInstance()->playEffect("finish.mp3");
    AudioHelp::getInstance()->playEffect("vo_done.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setPosition(visibleSize*0.5);
    
    m_pStick->setTouchEnabled(false);
    Node* pFinishCandy = Node::create();
    Sprite* pStick = Sprite::create("content/make/stick.png");
    pFinishCandy->addChild(pStick);
    pStick->setRotation(-130);
    pStick->setAnchorPoint(Vec2(22.0f/pStick->getContentSize().width, 240.0f/pStick->getContentSize().height));
    
    std::stringstream ostr;
    ostr<<"content/make/make/candy"<<m_nShapeIndex+1<<"/"<<m_nFlavorIndex+1<<".png";
    Sprite* pCandy = Sprite::create(ostr.str());
    if (m_nShapeIndex==1) {
        pCandy->setRotation(16);
    }
    pCandy->setAnchorPoint(gCandyAnc[m_nShapeIndex]);
    pFinishCandy->addChild(pCandy);
    
    this->addChildToUILayer(pFinishCandy);
    pFinishCandy->setPosition(visibleSize*0.5);
    pFinishCandy->runAction(Sequence::create(RotateBy::create(1, 145),
                                             CallFunc::create([=](){
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/star0.plist");
        this->addChildToUILayer(pParticle);
        pParticle->setPosition(visibleSize*0.5);
    }),
                                             DelayTime::create(2),
                                             CallFunc::create([=](){
        SceneManager::replaceTheScene<DecorateScene>();
    }), NULL));
    m_pCotton->setVisible(false);
    m_pStick->setVisible(false);
    m_pSilk->setVisible(false);
}

void MakeCottonScene::_onFlavorClicked(LockItem* item)
{
    if (m_pFlavorScroll->isScrolled()) {
        return;
    }
    if (!m_bItemEnable) {
        return;
    }
    
    int tag = item->getTag();
    if (item->isLocked()){
        int type = item->lockType;
        if (type==0) {
            if (!gNoneIap){
                ShopLayer* pLayer = ShopLayer::create();
                this->addChildToUILayer(pLayer);
                pLayer->setLocalZOrder(100);
                pLayer->showBannerDismiss();
                return;
            }
        }else if(!ConfigManager::getInstance()->getVideoUnLocked("flavor", tag)){
            RewardManager::getInstance()->showRewardAds("flavor", tag);
            return;
        }
    }
    AudioHelp::getInstance()->playSelectedEffect();
    m_bItemEnable = false;
    m_pFlavorScroll->setTouchEnabled(false);
    m_nFlavorIndex = tag;
    GameDataManager::getInstance()->m_nFlavorIndex = m_nFlavorIndex;
    _removeFlavorBottle();
    _showMachine();
}


void MakeCottonScene::_onShapeClicked(LockItem* item)
{
    if (!m_bItemEnable) {
        return;
    }
    int tag = item->getTag();
    if (item->isLocked()){
        int type = item->lockType;
        if (type==0) {
            if (!gNoneIap){
                ShopLayer* pLayer = ShopLayer::create();
                this->addChildToUILayer(pLayer);
                pLayer->setLocalZOrder(100);
                pLayer->showBannerDismiss();
                return;
            }
        }else if(!ConfigManager::getInstance()->getVideoUnLocked("shape", tag)){
            RewardManager::getInstance()->showRewardAds("shape", tag);
            return;
        }
    }
    m_bItemEnable = false;
    m_nShapeIndex = tag;
    GameDataManager::getInstance()->m_nShapeIndex = m_nShapeIndex;
    _removeShapeBag();
    
}