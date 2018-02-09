
#include "MixFlourScene.h"

MixFlourScene::MixFlourScene()
{
    m_pBar = nullptr;
    m_pWhrisk = nullptr;
    m_nAddCount = 0;
    m_nMixCount = 0;
    m_nIngridentCount = 5;
    m_nLeftShow = 0;
    m_nRightShow = 0;
    m_bWhriskIsReady = false;
}

MixFlourScene::~MixFlourScene()
{
    
}
bool MixFlourScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    memset(m_nAddStep, 0, sizeof(m_nAddStep));
    
    //    bg
    Sprite* pBg = Sprite::create(localPath("make_bk.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    std::stringstream ostrKid;
    int kidIndex = arc4random()%3;
    ostrKid<<"content/common/make/child"<<kidIndex+1<<"/";
    Sprite* pKid = Sprite::create(ostrKid.str()+"1.png");
    pKid->setTag(0);
    pKid->setAnchorPoint(Vec2(1,0.5));
    CMVisibleRect::setPosition(pKid, 680,450);
    this->addChildToContentLayer(pKid);
    pKid->setLocalZOrder(-1);
    
    Sprite* pEye = Sprite::create(ostrKid.str()+"wink.png");
    pKid->addChild(pEye);
    pEye->setName("eye");
    pEye->setPosition(pKid->getContentSize()*0.5);
    pEye->setOpacity(0);
    pEye->runAction(RepeatForever::create(Sequence::create(FadeIn::create(0.02),
                                                           DelayTime::create(0.3),
                                                           FadeOut::create(0.02),
                                                           DelayTime::create(3),
                                                           DelayTime::create(3),
                                                           NULL)));
    Sprite* pTable = Sprite::create(localPath("table.png"));
    CMVisibleRect::setPosition(pTable, 480, 250);
    this->addChildToContentLayer(pTable);
    
    //    left
    m_pMilk = _createDrageNode(localPath("milk0.png"));
    this->addChildToContentLayer(m_pMilk);
    CMVisibleRect::setPositionAdapted(m_pMilk, 170, 430+m_pMilk->getContentSize().height*0.5+visibleSize.height);
    m_pMilk->setActionPoint(Vec2(165, 210));
    m_pMilk->setTag(0);
    m_pMilk->setLocalZOrder(0);
    
    m_pBufferBox = Sprite::create(localPath("butter0_0.png"));
    this->addChildToContentLayer(m_pBufferBox);
    CMVisibleRect::setPositionAdapted(m_pBufferBox, 170, 410+m_pBufferBox->getContentSize().height*0.5+visibleSize.height);
    m_pBufferBox->setLocalZOrder(10);
    
    Sprite* pSpoon = Sprite::create(localPath("butter2_0.png"));
    m_pBufferBox->addChild(pSpoon);
    pSpoon->setPosition(Vec2(50, 40));
    pSpoon->setName("spoon");
    
    m_pBufferSpoon = _createDrageNode(localPath("butter2_1.png"));
    m_pBufferBox->addChild(m_pBufferSpoon);
    m_pBufferSpoon->setTag(0);
    m_pBufferSpoon->setPosition(m_pBufferBox->getContentSize()*0.5);
        m_pBufferSpoon->setVisible(false);
    m_pBufferSpoon->setTouchEnabled(false);
    m_pBufferSpoon->setActionPoint(Vec2(30, 190));
    m_pBufferSpoon->index = 0;
    
    m_pSugar = _createDrageNode(localPath("sugar0.png"));
    this->addChildToContentLayer(m_pSugar);
    CMVisibleRect::setPositionAdapted(m_pSugar, 170, 460+m_pSugar->getContentSize().height*0.5+visibleSize.height);
    m_pSugar->setTag(0);
    m_pSugar->setLocalZOrder(0);
    m_pSugar->setActionPoint(Vec2(39, 140));
    
    
    //    right
    m_pFlour = Sprite::create(localPath("flour0.png"));
    this->addChildToContentLayer(m_pFlour);
    CMVisibleRect::setPositionAdapted(m_pFlour, 800, 540+visibleSize.height);
    m_pFlour->setLocalZOrder(10);
    
    m_pFlourSpoon = _createDrageNode(localPath("spoon0.png"));
    m_pFlour->addChild(m_pFlourSpoon);
    m_pFlourSpoon->setTag(0);
    m_pFlourSpoon->setPosition(Vec2(130, 260));
    m_pFlourSpoon->setVisible(false);
    m_pFlourSpoon->setTouchEnabled(false);
    m_pFlourSpoon->setActionPoint(Vec2(36, 46));
    
    m_pEggBox = Sprite::create(localPath("egg0_0.png"));
    this->addChildToContentLayer(m_pEggBox);
    m_pEggBox->setLocalZOrder(10);
    CMVisibleRect::setPositionAdapted(m_pEggBox, 780, 450+m_pEggBox->getContentSize().height*0.5+visibleSize.height);
    
    m_pEgg = _createDrageNode(localPath("egg1.png"));
    m_pEggBox->addChild(m_pEgg);
    m_pEgg->setPosition(m_pEggBox->getContentSize()*0.5);
    m_pEgg->setTag(1);
    m_pEgg->setLocalZOrder(1);
    m_pEgg->setContentSize(m_pEggBox->getContentSize());
    m_pEgg->setTouchEnabled(false);
    m_pEgg->setVisible(false);
    
    m_pEgg2 = _createDrageNode(localPath("egg1.png"));
    m_pEggBox->addChild(m_pEgg2);
    m_pEgg2->setPosition(m_pEggBox->getContentSize()*0.5);
    m_pEgg2->setTag(2);
    m_pEgg2->setLocalZOrder(2);
    m_pEgg2->setContentSize(m_pEggBox->getContentSize());
    m_pEgg2->setTouchEnabled(false);
    m_pEgg2->setVisible(false);
    
    m_pBowl = Sprite::create(localPath("bowl0.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 480, 180);
    
    m_pBowlFront = Sprite::create(localPath("bowl1.png"));
    this->addChildToContentLayer(m_pBowlFront);
    CMVisibleRect::setPositionAdapted(m_pBowlFront, 480, 180);
    m_pBowlFront->setLocalZOrder(9);
    
    m_pMixture = Node::create();
    this->addChildToContentLayer(m_pMixture);
    CMVisibleRect::setPositionAdapted(m_pMixture, 450, 210);
    m_pMixture->setPosition(Vec2(m_pBowl->getPositionX()-32, m_pBowl->getPositionY()+32));
    m_pMixture->setLocalZOrder(5);
    
    m_pFlourSpoon->setOrgPositionDefault();
    m_pEgg->setOrgPositionDefault();
    m_pEgg2->setOrgPositionDefault();
    m_pSugar->setOrgPositionDefault();
    m_pMilk->setOrgPositionDefault();
    
//    TipLayer *pTipLayer = TipLayer::create();
//    this->addChildToUILayer(pTipLayer);
//    pTipLayer->showTip("Add ingredients!");
//    pTipLayer->setLocalZOrder(100);
    
    m_VectorIngredient.pushBack(m_pFlour);
    m_VectorIngredient.pushBack(m_pMilk);
    m_VectorIngredient.pushBack(m_pEggBox);
    m_VectorIngredient.pushBack(m_pSugar);
    m_VectorIngredient.pushBack(m_pBufferBox);
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    
    m_pGuideLayer2 = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer2);
    
    _showIngrident(0);
    m_nNextSceneTag = GameUIEvent::eSceneTagBake;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagBake;
    return true;
}

#pragma mark - initData
void MixFlourScene::_initData()
{
    
    setExPath("content/make/mix/");
}

void MixFlourScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    m_pGuideLayer2->removeGuide();
    if (pDragNode==m_pWhrisk){
        if (m_bWhriskIsReady) {
            pDragNode->setLocalZOrder(5);
            
            m_pMixture->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.2),
                                                                         CallFunc::create([=]()
                                                                                          {
                                                                                              m_pMixture->setScaleX(-1);
                                                                                          }),
                                                                         DelayTime::create(0.2),
                                                                         CallFunc::create([=]()
                                                                                          {
                                                                                              m_pMixture->setScaleX(1);
                                                                                          }),
                                                                         NULL)));
            ParticleSystemQuad *pParticle = (ParticleSystemQuad*)m_pWhrisk->getChildByName("whrisk_point");
            if (!pParticle) {
//                pParticle = ParticleSystemQuad::create("particle/point.plist");
//                m_pWhrisk->addChild(pParticle);
//                pParticle->setPosition(m_pWhrisk->convertToNodeSpace(m_pWhrisk->getWorldSpaceActionPoint()));
//                pParticle->setName("whrisk_point");
            }
        }
    }else{
        pDragNode->setLocalZOrder(10);
        if (pDragNode==m_pFlourSpoon) {
            m_pFlour->setTexture(localPath("flour1.png"));
            pDragNode->setVisible(true);
        }else if (pDragNode==m_pEgg || pDragNode==m_pEgg2) {
            pDragNode->setVisible(true);
        }else if (pDragNode==m_pBufferSpoon) {
            if (!m_pBufferSpoon->isVisible()) {
                m_pBufferBox->setTexture(localPath("butter1_0.png"));
                Sprite* pBoxCover = Sprite::create(localPath("butter0_1.png"));
                this->addChildToContentLayer(pBoxCover);
                pBoxCover->setPosition(m_pBufferBox->getPosition());
                pBoxCover->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 300)));
            }
            m_pBufferSpoon->setVisible(true);
            Node* pNode = m_pBufferBox->getChildByName("spoon");
            if (pNode) {
                pNode->setVisible(false);
            }
        }
    }
    _hideShadow(pDragNode);
}

void MixFlourScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    Rect rect = m_pBowl->getBoundingBox();
    rect.origin = m_pBowl->getParent()->convertToWorldSpace(rect.origin);
    if (rect.containsPoint(worldPoint)) {
        if (m_pWhrisk==pDragNode) {
            if (m_bWhriskIsReady) {
                m_nMixCount++;
//                m_pBar->doSchedule();
                AudioHelp::getInstance()->playLoopEffect("stir.mp3");
                if (m_nMixCount%50==0) {
                    int mixtureStep = m_nMixCount/50;
                    if (mixtureStep<=4){
                        std::stringstream ostr;
                        ostr<<"batter"<<mixtureStep-1<<".png";
                        Sprite* pMixture = Sprite::create(localPath(ostr.str()));
                        m_pMixture->removeAllChildren();
                        m_pMixture->addChild(pMixture);
                    }
                    if (mixtureStep==4) {
                        //                        finished  show next button
                        //                        pDragNode->ignoreMoving = true;
//                                                m_pGameUI->showNextLayout();
                        m_pWhrisk->setTouchEnabled(false);
                        m_pWhrisk->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, 300)));
                        
                        Sprite* pPan = Sprite::create(localPath("pan0.png"));
                        this->addChildToContentLayer(pPan);
                        CMVisibleRect::setPositionAdapted(pPan, 170, 510+visibleSize.height);
                        m_pPan = pPan;
                        
                        Sprite* pPanFront = Sprite::create(localPath("pan1.png"));
                        this->addChildToContentLayer(pPanFront);
                        CMVisibleRect::setPositionAdapted(pPanFront, 170, 510+visibleSize.height);
                        
                        _showIngrident(pPan);
                        _showIngrident(pPanFront);
                        
                        m_pBowlMixture = _createDrageNode(localPath("bowl0.png"));
                        this->addChildToContentLayer(m_pBowlMixture);
                        Sprite* pBowlFront = Sprite::createWithTexture(m_pBowlFront->getTexture());
                        m_pBowlMixture->addChild(pBowlFront,5);
                        
                        Sprite* pMixture = Sprite::create(localPath("batter4.png"));
                        m_pBowlMixture->addChild(pMixture);
                        pMixture->setPosition(Vec2(-32, 32));
                        pMixture->setName("mixture");
                        
                        m_pBowlMixture->setTouchEnabled(false);
                        m_pBowlMixture->setPosition(m_pBowl->getPosition());
                        m_pBowlMixture->runAction(Sequence::create(Spawn::create(MoveBy::create(0.5, Vec2(300, 100)),
                                                                          ScaleTo::create(0.5, 0.8), NULL),
                                                            CallFunc::create([=]()
                                                                             {
                                                                                 m_pBowlMixture->setOrgPositionDefault();
                                                                                 m_pBowlMixture->setTouchEnabled(true);
                                                                             }), NULL));
                        
                        m_pBowl->setVisible(false);
                        m_pBowlFront->setVisible(false);
                        m_pMixture->setVisible(false);
                        
                    }
                }
            }else{
                m_bWhriskIsReady = true;
                pDragNode->setTouchEnabled(false);
                pDragNode->setLocalZOrder(5);
                
                m_pWhrisk->getDragSprite()->setTexture(localPath("scoop1.png"));
                Rect rect = m_pBowl->getBoundingBox();
                Rect limitRect = Rect(rect.origin.x+170, rect.origin.y+270, rect.size.width-280, rect.size.height-250);
                pDragNode->setLimitRect(limitRect);
                pDragNode->setOrgPositionDefault();
                pDragNode->setTouchEnabled(true);
                m_pMixture->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.2),
                                                                             CallFunc::create([=]()
                                                                                              {
                                                                                                  m_pMixture->setScaleX(-1);
                                                                                              }),
                                                                             DelayTime::create(0.2),
                                                                             CallFunc::create([=]()
                                                                                              {
                                                                                                  m_pMixture->setScaleX(1);
                                                                                              }),
                                                                             NULL)));
                ParticleSystemQuad *pParticle = (ParticleSystemQuad*)m_pWhrisk->getChildByName("whrisk_point");
                if (!pParticle) {
//                    pParticle = ParticleSystemQuad::create("particle/point.plist");
//                    m_pWhrisk->addChild(pParticle);
//                    pParticle->setPosition(m_pWhrisk->convertToNodeSpace(m_pWhrisk->getWorldSpaceActionPoint()));
//                    pParticle->setName("whrisk_point");
                }
            }
        }
    }
    if (pDragNode==m_pBufferSpoon) {
        if (m_pBufferSpoon->index==0) {
            Rect rect = m_pBufferBox->getBoundingBox();
            rect.origin = m_pBufferBox->getParent()->convertToWorldSpace(rect.origin);
            if (rect.containsPoint(worldPoint)){
                m_pBufferBox->setTexture(localPath("butter1_1.png"));
                m_pBufferSpoon->getDragSprite()->setTexture(localPath("butter2_2.png"));
                m_pBufferSpoon->index = 1;
            }
        }
    }else if (pDragNode==m_pBowlMixture) {
    }
}

void MixFlourScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    AudioHelp::getInstance()->stopLoopEffect();
    if (pDragNode==m_pWhrisk){
        ParticleSystemQuad *pParticle = (ParticleSystemQuad*)m_pWhrisk->getChildByName("whrisk_point");
        if (pParticle) {
            pParticle->removeFromParent();
        }
//        m_pBar->pauseProgress();
        
        m_pWhrisk->getDragSprite()->stopAllActions();
        m_pWhrisk->getDragSprite()->setPosition(Vec2::ZERO);
        
        m_pMixture->stopAllActions();
        m_pMixture->setScaleX(1);
        return;
    }else if (pDragNode==m_pBowlMixture) {
        Rect rect = m_pBowlMixture->getBoundingBox();
        if (rect.intersectsRect(m_pPan->getBoundingBox())) {
            pDragNode->setTouchEnabled(false);
            pDragNode->setPosition(Vec2(m_pPan->getPositionX()+100, m_pPan->getPositionY()+400));
            pDragNode->runAction(RotateBy::create(0.5, -120));
            m_pBowlFront->runAction(RotateBy::create(0.5, -120));
            
            Node* pMixture = pDragNode->getChildByName("mixture");
            if (pMixture) {
                pMixture->runAction(Sequence::create(DelayTime::create(0.5),
                                                     Spawn::create(MoveBy::create(1, Vec2(-150, 330)),
                                                                   ScaleTo::create(1, 1.2,0.5),
                                                                   RotateBy::create(0.8, 100), NULL),
                                                     Spawn::create(ScaleTo::create(0.5, 0.5),
                                                                   FadeOut::create(1),
                                                                   CallFunc::create([=]()
                                                                                    {
                                                                                        Sprite* pBatter = Sprite::create(localPath("pan_batter.png"));
                                                                                        m_pPan->addChild(pBatter);
                                                                                        pBatter->setPosition(m_pPan->getContentSize()*0.5);
                                                                                        pBatter->setOpacity(0);
                                                                                        pBatter->runAction(FadeIn::create(1));
                                                                                    }), NULL),
                                                     CallFunc::create([=]()
                                                                      {
                                                                          pDragNode->runAction(Sequence::create(RotateTo::create(0.5, 1),
                                                                                                                MoveBy::create(0.5, Vec2(visibleSize.width, 300)), NULL));
                                                                          m_pGameUI->showNextLayout();
                                                                          
                                                                          ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
                                                                          this->addChildToUILayer(pParticle);
                                                                          pParticle->setPosition(visibleSize*0.5);
                                                                      }), NULL));
            }
        }
        return;
    }
    Rect rect = m_pBowl->getBoundingBox();
    rect.origin = m_pBowl->getParent()->convertToWorldSpace(rect.origin);
    if (rect.containsPoint(pDragNode->getParent()->convertToWorldSpace(pDragNode->getPosition()))) {
        pDragNode->setTouchEnabled(false);
        //        pDragNode->setVisible(false);
        std::string lNameStr = " ";
        Vec2 addPos = Vec2(0, 0);
        if (pDragNode==m_pFlourSpoon) {
            pDragNode->setLocalZOrder(5);
            m_pFlourSpoon->setPosition(m_pFlourSpoon->getParent()->convertToNodeSpace(m_pBowl->convertToWorldSpace(Vec2(320, 390))));
            pDragNode->runAction(Sequence::create(RotateBy::create(0.5, -30),
                                                  CallFunc::create([=]()
                                                                   {
                                                                       AudioHelp::getInstance()->playPourPowderEffect();
                                                                       ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/flour.plist");
//                                                                       pParticle->setScaleX(-1);
                                                                       this->addChildToContentLayer(pParticle);
                                                                       pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pDragNode->getWorldSpaceActionPoint()));
                                                                       pParticle->setLocalZOrder(10);
                                                                       
                                                                       Sprite* pSprite = Sprite::create(localPath("spoon0.png"));
                                                                       pDragNode->addChild(pSprite,10);
                                                                       pSprite->runAction(FadeOut::create(2));
                                                                       
                                                                       pDragNode->getDragSprite()->setTexture(localPath("spoon1.png"));
//                                                                       Action* pAction = Sequence::create(MoveBy::create(0.5, Vec2(-100, 50)),
//                                                                                                         MoveBy::create(1, Vec2(200, 0)),
//                                                                                                         MoveBy::create(0.5, Vec2(-100, -50)),NULL);
                                                                       
                                                                       Action* pAction = Sequence::create(JumpBy::create(1, Vec2(50, 0), 50, 3), NULL);
                                                                       pDragNode->runAction(pAction);
                                                                       pParticle->runAction(pAction->clone());
                                                                   }),
                                                  DelayTime::create(2),
                                                  RotateTo::create(0.3, 0),
                                                  CallFunc::create([=]()
                                                                   {
                                                                       m_pFlour->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, 0)));
                                                                       _addFinished();
                                                                   }),
                                                  NULL));
            lNameStr = localPath("flour2.png");
        }else if (pDragNode==m_pEgg || pDragNode==m_pEgg2) {
            if (pDragNode==m_pEgg) {
                m_nAddStep[1] = m_nAddCount;
            }else{
                m_nAddStep[2] = m_nAddCount;
            }
            lNameStr = localPath("egg3.png");
            pDragNode->setVisible(false);
            AudioHelp::getInstance()->playAddEggEffect();
            Sprite* pEgg = Sprite::create(localPath("egg1.png"));
            this->addChildToContentLayer(pEgg);
            pEgg->setLocalZOrder(6);
            
            
            if (pDragNode==m_pEgg) {
                addPos = Vec2(-20, 60);
            }else{
                addPos = Vec2(60, 0);
            }
            
            pEgg->setPosition(Vec2(visibleSize.width/2+addPos.x, visibleSize.height/2+addPos.y));
            pEgg->runAction(Sequence::create(AnimationHelp::createAnimate(localPath("egg"), 1, 2,false),
                                             CallFunc::create([=]()
                                                              {
                                                                  if (pDragNode==m_pEgg) {
                                                                      m_pEgg2->setTouchEnabled(true);
                                                                  }else{
                                                                      m_pEggBox->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, 20)));
                                                                      _addFinished();
                                                                  }
                                                                  pEgg->removeFromParent();
                                                              }),
                                             NULL));
            
            Sprite* pEggIn = Sprite::create(localPath("egg3.png"));
            this->addChildToContentLayer(pEggIn);
            pEggIn->setLocalZOrder(5);
            pEggIn->setPosition(pEgg->getPosition());
            pEggIn->setScale(0.1,0.5);
            pEggIn->runAction(Sequence::create(DelayTime::create(0.3),
                                               Spawn::create(ScaleTo::create(0.5, 0.8),
                                                             MoveBy::create(0.5, Vec2(0, -80)), NULL),
                                               FadeOut::create(0.1), NULL));
        }else if (pDragNode==m_pSugar) {
            lNameStr = localPath("sugar.png");
            m_nAddStep[6] = m_nAddCount;
            pDragNode->setPosition(Vec2(m_pBowl->getPositionX()-200, m_pBowl->getPositionY()+200));
            pDragNode->setLocalZOrder(5);
            addPos = Vec2(-60, 30);
            pDragNode->runAction(Sequence::create(RotateBy::create(0.3, 120),
                                                  CallFunc::create([=]()
                                                                   {
                                                                       AudioHelp::getInstance()->playPourPowderEffect();
                                                                       ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/sugar.plist");
                                                                       this->addChildToContentLayer(pParticle);
                                                                       pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pDragNode->getWorldSpaceActionPoint()));
                                                                       pParticle->setLocalZOrder(5);
                                                                       pParticle->runAction(JumpBy::create(1.5, Vec2::ZERO, 30, 4));
                                                                   }),
                                                  JumpBy::create(1.5, Vec2::ZERO, 30, 4),
                                                  RotateTo::create(0.3, 0),
                                                  MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                                  CallFunc::create([=]()
                                                                   {
                                                                       pDragNode->setVisible(false);
                                                                       _addFinished();
                                                                   }),
                                                  NULL));
        }else if (pDragNode==m_pMilk) {
            m_nAddStep[4] = m_nAddCount;
            lNameStr = localPath("milkbatter.png");
            pDragNode->setPosition(Vec2(m_pBowl->getPositionX()-200, m_pBowl->getPositionY()+300));
            pDragNode->getDragSprite()->setTexture(localPath("milk2.png"));
            AudioHelp::getInstance()->playPourWaterEffect();
            
            Sprite* pStream = Sprite::create(localPath("milk1_2.png"));
            this->addChildToContentLayer(pStream);
            pStream->setLocalZOrder(5);
            pStream->setAnchorPoint(Vec2(0.5, 0.9));
            pStream->setVisible(false);
            pDragNode->runAction(Sequence::create(RotateBy::create(0.5, 100),
                                                  CallFunc::create([=]()
                                                                   {
                                                                       AudioHelp::getInstance()->playPourMilkEffect();
                                                                       pDragNode->setLocalZOrder(5);
//                                                                       ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/milk.plist");
//                                                                       this->addChildToContentLayer(pParticle);
//                                                                       pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pDragNode->getWorldSpaceActionPoint()));
//                                                                       pParticle->setLocalZOrder(5);
                                                                       
                                                                       pStream->setLocalZOrder(6);
                                                                       pStream->setPosition(m_pContentLayer->convertToNodeSpace(pDragNode->getWorldSpaceActionPoint()));
                                                                       pStream->setVisible(true);
                                                                       
                                                                       
                                                                   }),
                                                  DelayTime::create(3),
                                                  CallFunc::create([=]()
                                                                   {
                                                                       AudioHelp::getInstance()->stopPourWaterEffect();
                                                                       pStream->setVisible(false);
                                                                   }),
                                                  RotateTo::create(0.3, 0),
                                                  MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                                  CallFunc::create([=]()
                                                                   {
                                                                       pDragNode->setVisible(false);
                                                                       _addFinished();
                                                                   }),
                                                  NULL));
        }else if (pDragNode==m_pBufferSpoon) {
            if (m_pBufferSpoon->index==1){
                lNameStr = localPath("butter3.png");
                addPos = Vec2(60, 30);
                pDragNode->runAction(Sequence::create(JumpBy::create(0.5, Vec2::ZERO, 10, 2),
                                                      CallFunc::create([=]()
                                                                       {
                                                                           pDragNode->getDragSprite()->setTexture(localPath("butter2_1.png"));
                                                                           m_pBufferBox->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 40)));
                                                                           _addFinished();
                                                                       }), NULL));
            }else{
                pDragNode->setTouchEnabled(true);
            }
        }
        Sprite* pAdd = Sprite::create(lNameStr);
        if (pAdd) {
            pAdd->setOpacity(0);
            m_pMixture->addChild(pAdd);
            pAdd->setPosition(addPos);
            pAdd->runAction(Sequence::create(DelayTime::create(0.5),
                                             FadeIn::create(1), NULL));
        }
    }else{
        _showShadow(pDragNode);
    }
    
}

void MixFlourScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    AudioHelp::getInstance()->stopLoopEffect();
    if (pDragNode==m_pWhrisk&&m_bWhriskIsReady){
        ParticleSystemQuad *pParticle = (ParticleSystemQuad*)m_pWhrisk->getChildByName("whrisk_point");
        if (pParticle) {
            pParticle->removeFromParent();
        }
//        m_pBar->pauseProgress();
        
        m_pWhrisk->getDragSprite()->stopAllActions();
        m_pWhrisk->getDragSprite()->setPosition(Vec2::ZERO);
        
        m_pMixture->stopAllActions();
        m_pMixture->setScaleX(1);
        return;
    }else if (pDragNode==m_pBowlMixture) {
        
    }
    _showShadow(pDragNode);
}


DragNode* MixFlourScene::_createDrageNode(std::string path,bool enable)
{
    DragNode* pTool = DragNode::create(path);
    pTool->setLimitRect(Rect(0, 90, visibleSize.width, visibleSize.height));
    pTool->setTouchEnabled(enable);
    pTool->dragNodeTouchBegin = CC_CALLBACK_2(MixFlourScene::dragNodeTouchBegin, this);
    pTool->dragNodeTouchMoved = CC_CALLBACK_2(MixFlourScene::dragNodeTouchMoved, this);
    pTool->dragNodeTouchEnded = CC_CALLBACK_2(MixFlourScene::dragNodeTouchEnded, this);
    pTool->dragNodeClicked = CC_CALLBACK_2(MixFlourScene::dragNodeClicked, this);
    return pTool;
    
}

void MixFlourScene::_showShadow(DragNode* pDragNode)
{
    pDragNode->back(pDragNode->getTag(),[=](){
        if (pDragNode==m_pEgg) {
            pDragNode->setVisible(false);
            m_pEggBox->setTexture(localPath("egg0_0.png"));
        }else if (pDragNode==m_pEgg2) {
            pDragNode->setVisible(false);
            m_pEggBox->setTexture(localPath("egg0_1.png"));
        }else if (pDragNode==m_pSugar) {
            pDragNode->getDragSprite()->setTexture(localPath("sugar0.png"));
        }else if (pDragNode==m_pMilk) {
            pDragNode->getDragSprite()->setTexture(localPath("milk0.png"));
        }else if (pDragNode==m_pBowlMixture) {
            pDragNode->getDragSprite()->setTexture(localPath("bowl0.png"));
        }
    });
    
}
void MixFlourScene::_hideShadow(DragNode* pDragNode)
{
    if (pDragNode==m_pEgg) {
        m_pEggBox->setTexture(localPath("egg0_1.png"));
    }else if (pDragNode==m_pEgg2) {
        m_pEggBox->setTexture(localPath("egg0_2.png"));
    }else if (pDragNode==m_pSugar) {
        pDragNode->getDragSprite()->setTexture(localPath("sugar1.png"));
    }else if (pDragNode==m_pMilk) {
        pDragNode->getDragSprite()->setTexture(localPath("milk1.png"));
    }else if (pDragNode==m_pBowlMixture) {
        pDragNode->getDragSprite()->setTexture(localPath("bowl2.png"));
    }
}


void MixFlourScene::_showIngrident(int index)
{
    if (index>=m_VectorIngredient.size()) {
        return;
    }
    Node* pNode = m_VectorIngredient.at(index);
    pNode->setScale(0.6);
    if (index ==1 || index ==3) {
        DragNode* pDragNode = (DragNode*)pNode;
        pDragNode->setTouchEnabled(false);
        
    }
    bool isLeft = pNode->getPositionX()<visibleSize.width/2?true:false;
    ccBezierConfig cfg;
    cfg.controlPoint_1 = CMVisibleRect::getPosition(480-630*(isLeft?1:-1), 520);
    cfg.controlPoint_2 = CMVisibleRect::getPosition(480-550*(isLeft?1:-1), 340);
    cfg.endPosition = CMVisibleRect::getPosition(480-350*(isLeft?1:-1), 210);
    pNode->runAction(Sequence::create(MoveBy::create(1, Vec2(0, -visibleSize.height)),
                                      ScaleTo::create(0.3, 0.6,0.5),
                                      ScaleTo::create(0.1, 0.6),
                                      CallFunc::create([=]()
                                                       {
                                                           AudioHelp::getInstance()->playEffect("slide.mp3");
                                                       }),
                                      Spawn::create(BezierTo::create(1, cfg),
                                                    ScaleTo::create(1, 1), NULL),
                                      CallFunc::create([=]()
                                                       {
                                                           if (index ==1 || index ==3) {
                                                               DragNode* pDragNode = (DragNode*)pNode;
                                                               pDragNode->setTouchEnabled(true);
                                                               pDragNode->setOrgPositionDefault();
                                                           }else{
                                                               
                                                           }
                                                           if (index==0) {
                                                               m_pGuideLayer->showGuideArrow(Vec2(cfg.endPosition.x, cfg.endPosition.y+100), -1);
                                                               m_pGuideLayer2->showGuideTapDirection(Vec2(m_pBowl->getPosition().x, m_pBowl->getPosition().y+100),false);
                                                               m_pFlourSpoon->setTouchEnabled(true);
                                                           }else if (index==2) {
                                                               m_pEgg->setTouchEnabled(true);
                                                           }else if (index==3) {
                                                               m_pBufferSpoon->setTouchEnabled(true);
                                                           }
                                                       }), NULL));
}

void MixFlourScene::_showIngrident(cocos2d::Node *pNode)
{
    pNode->setScale(0.6);
    bool isLeft = pNode->getPositionX()<visibleSize.width/2?true:false;
    ccBezierConfig cfg;
    cfg.controlPoint_1 = CMVisibleRect::getPosition(480-630*(isLeft?1:-1), 520);
    cfg.controlPoint_2 = CMVisibleRect::getPosition(480-550*(isLeft?1:-1), 340);
    cfg.endPosition = CMVisibleRect::getPosition(480, 120);
    pNode->runAction(Sequence::create(MoveBy::create(1, Vec2(0, -visibleSize.height)),
                                      ScaleTo::create(0.3, 0.6,0.5),
                                      ScaleTo::create(0.1, 0.6),
                                      CallFunc::create([=]()
                                                       {
                                                           AudioHelp::getInstance()->playEffect("slide.mp3");
                                                       }),
                                      Spawn::create(BezierTo::create(1, cfg),
                                                    ScaleTo::create(1, 1), NULL),
                                      CallFunc::create([=]()
                                                       {
                                                           if (pNode==m_pFlour) {                                                               
                                                               m_pGuideLayer->showGuideArrow(cfg.endPosition, -1);
                                                               m_pGuideLayer2->showGuideTapDirection(m_pBowl->getPosition(),false);
                                                           }
                                                       }),
                                      NULL));
}
void MixFlourScene::_addFinished()
{
    m_nAddCount++;
    _showIngrident(m_nAddCount);
    if (m_nAddCount>=m_nIngridentCount) {
        //            add finished
        if (!m_pWhrisk) {
            
            m_pWhrisk = _createDrageNode(localPath("scoop0.png"));
            this->addChildToContentLayer(m_pWhrisk);
            CMVisibleRect::setPositionAdapted(m_pWhrisk, 460, 380+visibleSize.height);
            m_pWhrisk->setActionPoint(Vec2(65, 30));
            m_pWhrisk->setLocalZOrder(10);
            
            m_pWhrisk->runAction(Sequence::create(DelayTime::create(0.5),
                                                  EaseBackOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height))),
                                                  CallFunc::create([=]()
                                                                   {
                                                                       m_pWhrisk->setOrgPositionDefault();
                                                                       m_pWhrisk->setTouchEnabled(true);
                                                                   }),
                                                  NULL));

//            _resetBar();
//            m_pBar->beginProgress(20);
//            m_pBar->pauseProgress();
        };
        
    }
    
}


void MixFlourScene::_resumeFry()
{
    Director::getInstance()->getScheduler()->resumeTarget(this);
    m_pBar->resumeProgress();
}

void MixFlourScene::_pauseFry()
{
    Director::getInstance()->getScheduler()->pauseTarget(this);
    m_pBar->pauseProgress();
}

void MixFlourScene::_resetBar()
{
    if (!m_pBar) {
        m_pBar = ProgressBar::create("content/common/progress/bar_v.png", "content/common/progress/face1.png", "content/common/progress/face2.png", "content/common/progress/face3.png","content/common/progress/bar_mash_v.png",ProgressBar::Direction::eDirectionV);
        this->addChildToUILayer(m_pBar);
        CMVisibleRect::setPositionAdapted(m_pBar, 320, 550,kBorderNone,kBorderTop);
        m_pBar->progressNewState = [=](int state){
            if (state == ProgressBar::eStateOK) {
                AudioHelp::getInstance()->playEffect("princess_win.mp3");
                m_pGameUI->showNextLayout();
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
                this->addChildToUILayer(pParticle);
                pParticle->setPosition(visibleSize*0.5);
            }else if (state == ProgressBar::eStateBad) {
                log("===mix bad");
//                AudioHelp::getInstance()->playEffect("princess_no.mp3");
//                m_pGameUI->hideNext();
//                _hideBar();
            }
        };
    }
    m_pBar->stopProgress();
    _showBar();
    m_pBar->setPercentStage(240.0*100/560, 395.0*100/560, 100);
}

void MixFlourScene::_showBar()
{
    if (m_pBar) {
        m_pBar->stopAllActions();
        CMVisibleRect::setPositionAdapted(m_pBar, 50-visibleSize.width/2, visibleSize.height/2,kBorderLeft);
        m_pBar->setVisible(true);
        m_pBar->runAction(EaseBackOut::create(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0))));
    }
}

void MixFlourScene::_hideBar()
{
    if (m_pBar) {
        m_pBar->stopAllActions();
        CMVisibleRect::setPositionAdapted(m_pBar, 50, visibleSize.height/2,kBorderLeft);
        m_pBar->runAction(Sequence::create(EaseBackIn::create(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0))),
                                           CallFunc::create([=]()
                                                            {
                                                                m_pBar->setVisible(false);
                                                            }), NULL));
    }
}
