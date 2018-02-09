
#include "MakeIceScene.h"

static Vec2 gWaterPos[][6] ={
    {
        Vec2(108, 272),
        Vec2(266, 272),
        Vec2(102, 145),
        Vec2(273, 145),
    },
    {
        Vec2(91, 311),
        Vec2(264, 311),
        Vec2(438, 311),
        Vec2(173, 184),
        Vec2(356, 184)
    },
    {
        Vec2(119, 278),
        Vec2(299, 278),
        Vec2(116, 129),
        Vec2(302, 129),
    },
};

MakeIceScene::MakeIceScene()
{
    m_pScrollView = nullptr;
    m_pTray = nullptr;
    m_pWaterBottle = nullptr;
    m_nShapeIndex = 0;
    m_nUpCount = 0;
    m_nDownCount = 0;
    m_nPourCount = 0;
    m_bFreezed = false;
}

MakeIceScene::~MakeIceScene()
{
    
}

bool MakeIceScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    Sprite* pBg = Sprite::create("content/make/make_bg.jpg");
    this->addChildToBGLayer(pBg);
    pBg->setPosition(visibleSize*0.5);
    
    Sprite *pTable = Sprite::create(localPath("table.png"));
    pTable->setOpacity(0);
    CMVisibleRect::setPositionAdapted(pTable, 320, 180);
    this->addChildToContentLayer(pTable);
    
//    Sprite *pTableCloth = Sprite::create(localPath("table_cloth.png"));
//    CMVisibleRect::setPositionAdapted(pTableCloth, 320-visibleSize.width, 20+400);
//    this->addChildToContentLayer(pTableCloth);
    
    pTable->runAction(Sequence::create(DelayTime::create(1),
                                       EaseBounceOut::create(FadeIn::create(1)),
                                       CallFunc::create([=]()
                                                        {
//                                                            pTableCloth->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, -400)));
                                                        }), NULL));
    _showShapScrollView();
    
    m_pTip = TipLayer::create();
    this->addChildToUILayer(m_pTip);
    m_pTip->showTip("content/common/tip/cyis.png");
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(20);
    return true;
}

void MakeIceScene::onEnter()
{
    ExtensionScene::onEnter();
    FlurryEventManager::getInstance()->logCurrentModuleEnterEvent(Flurry_EVENT_ICEMAKE);
//    Scene* scene = (Scene*)this->getParent();
//    
//    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
//    scene->getPhysicsWorld()->setGravity(Vect(0,-98*10));
}

void MakeIceScene::onExit()
{
    FlurryEventManager::getInstance()->logCurrentModuleExitEvent(Flurry_EVENT_ICEMAKE);
    
    ExtensionScene::onExit();
}

void MakeIceScene::onItemClicked(LockItem* item)
{
    if (!m_bShapTouchEnable) {
        return;
    }
    AudioHelp::getInstance()->playSelectedEffect();
    int tag = item->getTag();
    m_nShapeIndex = tag;
    GameDataManager::getInstance()->m_nShapeIndex = m_nShapeIndex;
    m_nUpCount = 2;
    m_nDownCount =2;
    if (tag==1) {
        m_nUpCount = 3;
        m_nDownCount =2;
    }
    
    _hidShapScrollView();
    _showTray();
}

void MakeIceScene::onFreezerCallback(FreezerNode *)
{
    m_pGuideLayer->removeGuide();
    if (m_pFreezer->getIsOpen()) {
        if (!m_bFreezed) {
            m_pFreezer->setTouchEnable(false);
            m_pTray->setTouchEnabled(true);
            Vec2 pos = Vec2(m_pFreezer->getDoorWorldRect().getMidX(), m_pFreezer->getDoorWorldRect().getMidY());
            pos = m_pGuideLayer->convertToNodeSpace(pos);
            m_pGuideLayer->showGuideMove(m_pTray->getPosition(), pos);
        }else{
            m_pFreezer->setTouchEnable(false);
            m_pTray->runAction(Sequence::create(DelayTime::create(0.6),
                                                CallFunc::create([=]()
                                                                 {
                                                                     m_pTray->setVisible(true);
                                                                     m_pFreezer->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, 0)));
                                                                     m_pTrayInFreezer->setVisible(false);
                                                                     AudioHelp::getInstance()->playEffect("ice_finish.mp3");
                                                                 }),
                                                Spawn::create(ScaleTo::create(0.5, 1.1),
                                                              MoveBy::create(0.5, Vec2(0, -50)), NULL),
                                                DelayTime::create(0.3),
                                                CallFunc::create([=]()
                                                                 {
                                                                     _iceJump();
                                                                 }),
                                                Spawn::create(ScaleTo::create(0.3, 0.9,1.1),
                                                              JumpBy::create(0.5, Vec2::ZERO, 50, 1), NULL),
                                                ScaleTo::create(0.3, 1),
                                                Spawn::create(ScaleTo::create(0.3, 0.9,1.1),
                                                              JumpBy::create(0.5, Vec2::ZERO, 50, 1), NULL),
                                                ScaleTo::create(0.3, 1),
                                                DelayTime::create(2),
                                                CallFunc::create([=]()
                                                                 {
                                                                     m_pTray->getDragSprite()->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, 0)));
                                                                     m_pGameUI->showNextLayout();
                                                                     
                                                                     AudioHelp::getInstance()->playEffect("finish_particle.mp3");
                                                                     ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
                                                                     this->addChildToUILayer(pParticle);
                                                                     pParticle->setLocalZOrder(10);
                                                                     pParticle->setPosition(visibleSize*0.5);
                                                                 }),
                                                NULL));
        }
    }else{
    }
}

void MakeIceScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    _hideShadow(pDragNode);
    if (pDragNode==m_pWaterBottle) {
        m_pGuideLayer->removeGuide();
    }
}

void MakeIceScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    
}

void MakeIceScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    if (pDragNode==m_pWaterBottle) {
        Rect rect = m_pTray->getDragSprite()->getBoundingBox();
        rect.origin = m_pTray->convertToWorldSpace(rect.origin);
        Vec2 point = pDragNode->getPosition();
        point= pDragNode->getParent()->convertToWorldSpace(point);
        if (rect.containsPoint(point)) {
            _pourWater();
        }
    }else if (pDragNode==m_pTray) {
        Rect rect = m_pFreezer->getDoorWorldRect();
        if (rect.containsPoint(worldPoint)) {
            m_pTray->setTouchEnabled(false);
            m_pTray->setPosition(Vec2(visibleSize.width/2, m_pTray->getPositionY()));
            m_pTray->runAction(Sequence::create(ScaleTo::create(0.5, 0.5),
                                                CallFunc::create([=]()
                                                                 {
                                                                     m_pTrayInFreezer = Sprite::createWithTexture(m_pTray->getDragSprite()->getTexture());
                                                                     for (int i = 0; i<m_nUpCount+m_nDownCount; i++) {
                                                                         std::stringstream ostr;
                                                                         ostr<<"tray"<<m_nShapeIndex<<"_water"<<i<<".png";
                                                                         
                                                                         Sprite* pWater = Sprite::create(localPath(ostr.str()));
                                                                         m_pTrayInFreezer->addChild(pWater);
                                                                         pWater->setTag(i);
                                                                         pWater->setPosition(gWaterPos[m_nShapeIndex][i]);
                                                                     }
                                                                     m_pFreezer->addChild(m_pTrayInFreezer);
                                                                     m_pTrayInFreezer->setScale(0.3,0.2);
                                                                     m_pTrayInFreezer->setPosition(Vec2(0, 130));
                                                                     
                                                                     m_pTray->setVisible(false);
                                                                     m_pFreezer->closeDoor();
                                                                     AudioHelp::getInstance()->playEffect("put_into_fridge.mp3");
                                                                 }), NULL));
            this->runAction(Sequence::create(DelayTime::create(1),
                                             CallFunc::create([=]()
                                                              {
                                                                  AudioHelp::getInstance()->playEffect("waiting.mp3");
                                                                  m_pFreezer->doFreeze();
                                                                  m_pTrayClipping->setVisible(false);
                                                                  
                                                                  for (int i = 0; i<m_nUpCount+m_nDownCount; i++) {
                                                                      std::stringstream ostr;
                                                                      ostr<<"tray"<<m_nShapeIndex<<"_ice"<<i<<".png";
                                                                      Sprite* pIce = Sprite::create(localPath(ostr.str()));
                                                                      m_pTray->addChild(pIce);
                                                                      pIce->setTag(i);
                                                                      pIce->setAnchorPoint(Vec2(0.5, 0));
                                                                      
                                                                      pIce->setPosition(Vec2(gWaterPos[m_nShapeIndex][i].x-m_pTray->getContentSize().width/2, gWaterPos[m_nShapeIndex][i].y-m_pTray->getContentSize().height/2-pIce->getContentSize().height/2));
                                                                      m_IceVector.pushBack(pIce);
                                                                      
                                                                      
                                                                  }
                                                              }),
                                             DelayTime::create(4.5),
                                             CallFunc::create([=]()
                                                              {
                                                                  AudioHelp::getInstance()->playEffect("fridge_off.mp3");
                                                              }),
                                             DelayTime::create(0.5),
                                             CallFunc::create([=]()
                                                              {
                                                                  m_bFreezed = true;
                                                                  m_pFreezer->stopFreeze();
                                                                  m_pFreezer->setTouchEnable(true);
                                                                  Vec2 pos = Vec2(m_pFreezer->getDoorWorldRect().getMidX(), m_pFreezer->getDoorWorldRect().getMidY());
                                                                  pos = m_pGuideLayer->convertToNodeSpace(pos);
                                                                  m_pGuideLayer->showGuideTap(pos);
                                                                  AudioHelp::getInstance()->playEffect("done.mp3");
                                                              }), NULL));
        }
    }
    
}

void MakeIceScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    _showShadow(pDragNode);
}

#pragma mark - initData
void MakeIceScene::_initData()
{
    
    setExPath("content/make/ice/");
    m_nNextSceneTag = GameUIEvent::eSceneTagCrushIce;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagCrushIce;
    
}

void MakeIceScene::_showShadow(DragNode* pDragNode)
{
    if (pDragNode==m_pWaterBottle) {
        pDragNode->getDragSprite()->setTexture(localPath("kettle0.png"));
    }
}

void MakeIceScene::_hideShadow(DragNode* pDragNode)
{
    if (pDragNode==m_pWaterBottle) {
        pDragNode->getDragSprite()->setTexture(localPath("kettle1.png"));
    }
    
}
void MakeIceScene::_showShapScrollView()
{
    if (!m_pScrollView) {
        Sprite* pBanner = Sprite::create("content/make/ice/box.png");
        
        m_pScrollView = ExpandScrollView::create();
        m_pScrollView->setContentSize(pBanner->getContentSize());
        m_pScrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
        this->addChildToContentLayer(m_pScrollView);
        m_pScrollView->addChild(pBanner);
        pBanner->setPosition(pBanner->getContentSize()*0.5);
        
        int count = 3;
        for (int i = 0; i<count; i++) {
            std::stringstream ostr;
            ostr<<"ice"<<i<<".png";
            LockItem* pItem = LockItem::create();
            if ((i<6 && i>2 && GameDataManager::getInstance()->getShapeLocked(i))) {
                pItem->initWithName(localPath(ostr.str()), "",true);
            }else{
                pItem->initWithName(localPath(ostr.str()), "",false);
            }
            pItem->setTag(i);
            pItem->onItemClicked = CC_CALLBACK_1(MakeIceScene::onItemClicked, this);
            pItem->setPosition(Vec2(120+180*i, 120));
            m_pScrollView->getInnerContainer()->addChild(pItem);
            m_pScrollView->getInnerContainer()->setContentSize(Size(150+200*count, m_pScrollView->getContentSize().height));
        }
    }
    m_bShapTouchEnable = false;
    m_pScrollView->stopAllActions();
    m_pScrollView->setTouchEnabled(false);
    m_pScrollView->getInnerContainer()->setPosition(Vec2::ZERO);
    CMVisibleRect::setPositionAdapted(m_pScrollView, (640-m_pScrollView->getContentSize().width)/2, 600+visibleSize.height/2,kBorderNone,kBorderTop);
    m_pScrollView->runAction(Sequence::create(DelayTime::create(1),
                                              MoveBy::create(0.5, Vec2(0, -visibleSize.height/2)),
                                              CallFunc::create([=]()
                                                               {
                                                                   
                                                                   AudioHelp::getInstance()->playEffect("choose_ice_shapes.mp3");
                                                                   m_bShapTouchEnable = true;
                                                               }), NULL));
    
}

void MakeIceScene::_hidShapScrollView()
{
    m_bShapTouchEnable = false;
    if (m_pScrollView) {
        m_pScrollView->stopAllActions();
        
        m_pScrollView->stopAllActions();
        m_pScrollView->runAction(EaseBackIn::create(MoveBy::create(0.5, Vec2(0, visibleSize.height/2))));
    }
}

void MakeIceScene::_showTray()
{
    if (!m_pTray) {
        std::stringstream ostr;
        ostr<<"tray"<<m_nShapeIndex<<".png";
        m_pTray = _createDrageNode(localPath(ostr.str()));
        this->addChildToContentLayer(m_pTray);
        
        m_pTrayClipping=  ClippingNode::create();
        m_pTray->addChild(m_pTrayClipping);
        m_pTrayClipping->setPosition(Vec2::ZERO);
        m_pTrayClipping->setAlphaThreshold(0.5);
    }
    m_pTray->setTouchEnabled(false);
    
    std::stringstream ostr;
    ostr<<"mask"<<m_nShapeIndex<<".png";
    m_pTrayMask = Sprite::create(localPath(ostr.str()));
    m_pTrayClipping->setStencil(m_pTrayMask);

    for (int i = 0; i<m_nUpCount+m_nDownCount; i++) {
        std::stringstream ostr;
        ostr<<"tray"<<m_nShapeIndex<<"_water"<<i<<".png";
        
        Sprite* pWater = Sprite::create(localPath(ostr.str()));
        m_pTrayClipping->addChild(pWater);
        pWater->setTag(i);
        pWater->setAnchorPoint(Vec2(0.5, 0.1));
        pWater->setPosition(Vec2(gWaterPos[m_nShapeIndex][i].x-m_pTray->getContentSize().width/2, gWaterPos[m_nShapeIndex][i].y-m_pTray->getContentSize().height/2-pWater->getContentSize().height*0.4));
        pWater->setOpacity(0);
        m_WaterVector.pushBack(pWater);
    }
    m_pTray->stopAllActions();
    CMVisibleRect::setPositionAdapted(m_pTray, 320, 250-visibleSize.height/2);
    m_pTray->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, visibleSize.height/2)),
                                        RotateBy::create(0.3, 10),
                                        RotateBy::create(0.3, -10),
                                        CallFunc::create([=]()
                                                         {
                                                         }), NULL));
    
    if (!m_pWaterBottle) {
        m_pWaterBottle = _createDrageNode(localPath("kettle0.png"));
        this->addChildToContentLayer(m_pWaterBottle);
        m_pWaterBottle->setActionPoint(Vec2(13, 302));
    }
    CMVisibleRect::setPositionAdapted(m_pWaterBottle, 400+visibleSize.width, 600);
    m_pWaterBottle->setTouchEnabled(false);
    m_pWaterBottle->runAction(Sequence::create(DelayTime::create(1),
                                               CallFunc::create([=]()
                                                                {
                                                                    AudioHelp::getInstance()->playEffect("drag_pour.mp3");
                                                                }),
                                               EaseSineOut::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0))),
                                               CallFunc::create([=]()
                                                                {
                                                                    m_pWaterBottle->setTouchEnabled(true);
                                                                    m_pGuideLayer->showGuideMove(m_pWaterBottle->getPosition(), m_pTray->getPosition());
                                                                }), NULL));
}

void MakeIceScene::_showFreezer()
{
    m_pFreezer = FreezerNode::create();
    this->addChildToContentLayer(m_pFreezer);
    m_pFreezer->onDoorCallback = CC_CALLBACK_1(MakeIceScene::onFreezerCallback, this);
    CMVisibleRect::setPosition(m_pFreezer, 320+visibleSize.width, 600);
    m_pFreezer->setLocalZOrder(-1);
    m_pFreezer->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)));
    AudioHelp::getInstance()->playEffect("time2refrigrate.mp3");
}

void MakeIceScene::_pourWater()
{
    m_pWaterBottle->setTouchEnabled(false);
    int index = m_nPourCount;
//    if (m_nPourCount>=m_nUpCount) {
//        index = m_nPourCount-m_nUpCount;
//    }
    log("=======%d",index);
    Node* pNode = m_WaterVector.at(index);
    Vec2 point = pNode->getPosition();
    point = pNode->getParent()->convertToWorldSpace(point);
    point = m_pWaterBottle->getParent()->convertToNodeSpace(point);
//    point.x+=100;
    point.x+=170;
    point.y+=360;
    
    m_pWaterBottle->setPosition(point);
    m_pWaterBottle->setTouchEnabled(false);
    m_pWaterBottle->runAction(Sequence::create(RotateBy::create(0.5, -100),
                                               CallFunc::create([=]()
                                                                {
                                                                    m_pWaterBottle->getDragSprite()->setTexture(localPath("kettle2.png"));
                                                                    
                                                                    
                                                                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/water2.plist");
                                                                    this->addChildToContentLayer(pParticle);
                                                                    pParticle->setName("dropwater");
                                                                    pParticle->setLocalZOrder(10);
                                                                    pParticle->setPosition(m_pContentLayer->convertToNodeSpace(m_pWaterBottle->getWorldSpaceActionPoint()));
                                                                    
                                                                    
                                                                    ParticleSystemQuad* pParticle2  = ParticleSystemQuad::create("particle/water.plist");
                                                                    this->addChildToContentLayer(pParticle2);
                                                                    pParticle2->setName("water");
                                                                    pParticle2->setLocalZOrder(10);
                                                                    
                                                                    pParticle2->setPosition(m_pContentLayer->convertToNodeSpace(m_pWaterBottle->getWorldSpaceActionPoint()));
                                                                    
                                                                    
                                                                    AudioHelp::getInstance()->playEffect("add_water.mp3");
                                                                }), NULL));
    
    
    if (pNode) {
        pNode->setScale(0.2);
        int temp = m_nPourCount;
        log("====temp %d====",temp);
        pNode->runAction(Sequence::create(DelayTime::create(1),
                                          FadeIn::create(0.1),
                                          ScaleTo::create(1.5, 1),
                                          CallFunc::create([=]()
                                                           {
                                                               log("====temp %d====",temp);
                                                               ParticleSystemQuad* pParticle = (ParticleSystemQuad*)m_pContentLayer->getChildByName("water");
                                                               if (pParticle) {
                                                                   pParticle->stopSystem();
                                                                   pParticle->removeFromParent();
                                                               }
                                                               ParticleSystemQuad* pParticle2 = (ParticleSystemQuad*)m_pContentLayer->getChildByName("dropwater");
                                                               if (pParticle2) {
                                                                   pParticle2->stopSystem();
                                                                   pParticle2->removeFromParent();
                                                               }
                                                               if (temp>=m_nDownCount+m_nUpCount-1) {
                                                                   log("====1====");
                                                                   _showFreezer();
                                                                   m_pWaterBottle->runAction(Sequence::create(RotateBy::create(0.5, 100),
                                                                                                            
                                                                                                              MoveBy::create(0.5, Vec2(visibleSize.width, 100)),
                                                                                                              CallFunc::create([=]()
                                                                                                                               {
                                                                                                                                   
                                                                                                                                   Vec2 pos = Vec2(m_pFreezer->getDoorWorldRect().getMidX(), m_pFreezer->getDoorWorldRect().getMidY());
                                                                                                                                   pos = m_pGuideLayer->convertToNodeSpace(pos);
                                                                                                                                   m_pGuideLayer->showGuideTap(pos);
                                                                                                                               }),NULL));
                                                                   m_pWaterBottle->setTouchEnabled(true);
                                                                   
                                                               }else{
                                                                   log("====3====");
                   m_pWaterBottle->runAction(Sequence::create(RotateBy::create(0.5, 100),
                                                              CallFunc::create([=]()
                                                                               {
                                                                                   m_pWaterBottle->getDragSprite()->setTexture(localPath("kettle1.png"));
                                                                               }),
                                                              MoveTo::create(0.5, Vec2(visibleSize.width/2, visibleSize.height*0.65)),
                                                              CallFunc::create([=]()
                                                                               {
                                                                                   m_pWaterBottle->setTouchEnabled(true);
//                                                                                   _pourWater();
                                                                               }),NULL));
               }
                                                           }), NULL));
    }
    m_nPourCount++;
}

void MakeIceScene::_flipTray()
{
    m_pTray->runAction(Sequence::create(RotateBy::create(1, 100),
                                        CallFunc::create([=]()
                                                         {
                                                             m_pTray->setRotation(180+100);
                                                             if(m_nShapeIndex==1){
                                                                 m_pTray->setDragSprite(Sprite::create(localPath("tray1_1.png")));
                                                                 
                                                                 m_pTrayMask = Sprite::create(localPath("mask1_1.png"));
                                                                 m_pTrayClipping->setStencil(m_pTrayMask);
                                                                 for_each(m_WaterVector.begin(), m_WaterVector.end(), [=](Sprite* pWater)
                                                                                                                   {
                                                                                                                       pWater->removeFromParent();
                                                                                                                   });
                                                                 m_WaterVector.clear();
                                                                 for (int i = 0; i<m_nUpCount+m_nDownCount; i++) {
                                                                     std::stringstream ostr;
                                                                     ostr<<"tray"<<m_nShapeIndex<<"_water"<<i<<".png";
                                                                     Sprite* pWater = Sprite::create(localPath(ostr.str()));
                                                                     m_pTrayClipping->addChild(pWater);
                                                                     pWater->setTag(i);
                                                                     pWater->setAnchorPoint(Vec2(0.5, 0));
                                                                     pWater->setPosition(Vec2(gWaterPos[m_nShapeIndex+2][i].x-m_pTray->getContentSize().width/2, gWaterPos[m_nShapeIndex+2][i].y-m_pTray->getContentSize().height/2-pWater->getContentSize().height/2));
                                                                     m_WaterVector.pushBack(pWater);
                                                                     if (i<m_nDownCount) {
                                                                         pWater->setOpacity(0);
                                                                     }else{
                                                                         pWater->setOpacity(255);
                                                                     }
                                                                 }

                                                             }else{
                                                                 int i = 0;
                                                                 for_each(m_WaterVector.begin(), m_WaterVector.end(), [=,&i](Sprite* pWater)
                                                                          {
                                                                              if (i>=m_nUpCount) {
                                                                                  pWater->setOpacity(255);
                                                                              }else{
                                                                                  pWater->setOpacity(0);
                                                                              }
                                                                              i++;
                                                                          });
                                                             }
                                                             
                                                             
                                                             
                                                         }),
                                        RotateBy::create(0.8, 80),
                                        CallFunc::create([=]()
                                                         {
                                                             m_pTray->setRotation(0);
                                                             m_pWaterBottle->setTouchEnabled(true);
                                                         }), NULL));
}

void MakeIceScene::_iceJump()
{
    AudioHelp::getInstance()->playEffect("ice_cube_fall.mp3");
    for_each(m_IceVector.begin(), m_IceVector.end(), [=](Sprite* pNode)
             {
                 stringstream ostr;
                 ostr<<"ice"<<m_nShapeIndex<<".png";
                 Sprite* pIce = Sprite::create(localPath(ostr.str()));
                 pIce->setPosition(Vec2(pNode->getPosition().x, pNode->getPosition().y+pNode->getContentSize().height/2));
                 m_pTray->addChild(pIce);
                 int dir = (arc4random()%30)>10?-1.0:1.0;
                 pIce->setVisible(false);
                 int tag = pNode->getTag();
                 pIce->runAction(Sequence::create(DelayTime::create(0.1+0.5*tag),
                                                  CallFunc::create([=]()
                                                                   {
                                                                       pNode->removeFromParent();
                                                                       pIce->setVisible(true);
                                                                       int angle = 10.0f*dir*(arc4random()%5);
                                                                       pIce->runAction(Sequence::create(Spawn::create(RotateBy::create(0.5, 10.0*(arc4random()%20)),
                                                                                                                      JumpBy::create(1, Vec2(dir*100, -400-tag*20), 150, 1), NULL),
                                                                                                        EaseBackOut::create(RotateBy::create(1, angle)), NULL));
                                                                   }),
                                                  NULL));
             });
    
}