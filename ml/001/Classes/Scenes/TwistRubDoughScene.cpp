
#include "TwistRubDoughScene.h"
#include "KnifeNode.h"
#include "SceneManager.h"

TwistRubDoughScene::TwistRubDoughScene()
{
    m_pBowl = nullptr;
    m_nRollCount = 0;
    m_nRollStep = 0;
    m_bRollToolReady = false;
    m_pDoughGrid = nullptr;
    
    m_nCutCount = 0;
    m_pCurDough = nullptr;
    m_nRubCount = 0;
    m_nMoveCount = 0;
}

TwistRubDoughScene::~TwistRubDoughScene()
{
    
}
bool TwistRubDoughScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    Sprite* pBg = Sprite::create("content/make/bg/bg_make2.jpg");
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    
    _createRollUI();
    _showSpreadFlour();
    
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_sprinkle_flour.mp3");
    }),
                                     DelayTime::create(2),
                                     CallFunc::create([=](){
        this->_pourDough();
    }),
                                     DelayTime::create(2),
                                     CallFunc::create([=](){
        this->_showRollTool();
    }),NULL));
    
    m_pTouchLayer = TouchLayer::create();
    this->addChildToUILayer(m_pTouchLayer);
    m_pTouchLayer->setTouchLayerEnable(false);
    m_pTouchLayer->onTouchBegan = CC_CALLBACK_2(TwistRubDoughScene::TouchBegan, this);
    m_pTouchLayer->onTouchMoved = CC_CALLBACK_2(TwistRubDoughScene::TouchMove, this);
    m_pTouchLayer->onTouchEnded = CC_CALLBACK_2(TwistRubDoughScene::TouchEnded, this);

    m_pGameUI->showNormalLayout();
    return true;
}

void TwistRubDoughScene::onEnter()
{
    ExtensionScene::onEnter();
}

void TwistRubDoughScene::onExit()
{
    ExtensionScene::onExit();
}

void TwistRubDoughScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void TwistRubDoughScene::onButtonCallback(Button* btn)
{
    if (btn->getTag()==GameUILayoutLayer::eUIButtonTagReset) {
        _reset();
    }
    ExtensionScene::onButtonCallback(btn);
}

void TwistRubDoughScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(10);
    m_pGuideLayer->removeGuide();
    if (pDragNode->getName()=="rolltool") {
        AudioHelp::getInstance()->playLoopEffect("roll_dough.mp3");
        if (!m_bRollToolReady) {
            m_bRollToolReady = true;
            m_pRollTool->runAction(RotateBy::create(0.3, 20));
        }
        m_pDoughGrid->runAction(RepeatForever::create(Sequence::create(Waves::create(1, Size(20, 20), 2, 5, true, true),
                                                                       DelayTime::create(0.1), NULL)));
    }else if (pDragNode->getName()=="cuttool"){
        Node* pShadow = pDragNode->getChildByName("shadow");
        if (pShadow) {
            pShadow->setVisible(false);
            pShadow->removeFromParent();
            pDragNode->runAction(RotateBy::create(0.5, -90));
        }
    }
}

void TwistRubDoughScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    if (pDragNode->getName()=="rolltool") {
        m_nRollCount++;
        if (m_nRollCount%10==0){
            int opacity = m_pFlourSprinkle->getOpacity()-1;
            if (opacity>0) {
                m_pFlourSprinkle->setOpacity(opacity);
            }
        }
        if (m_nRollCount%60==0) {
            m_nRollStep++;
            m_pDough->runAction(FadeOut::create(0.3));
            m_pDough = Sprite::create(localPath("make2/dough"+std::to_string(m_nRollStep)+".png"));
            m_pDoughGrid->addChild(m_pDough);
            m_pDough->setOpacity(100);
            m_pDough->runAction(Sequence::create(FadeTo::create(0.1, 255),
                                                 CallFunc::create([=](){
                if (3==m_nRollStep) {
                    pDragNode->runAction(RotateBy::create(0.1, -90));
                }else if (5==m_nRollStep) {
                    pDragNode->runAction(RotateBy::create(0.1, 90));
                }else if (7==m_nRollStep) {
                    pDragNode->setTouchEnabled(false);
                    m_pDoughGrid->stopAllActions();
                    m_pDoughGrid->runAction(Waves::create(0.1, Size(20, 20), 2, 5, false, false));
//                    m_pFlourSprinkle->runAction(FadeTo::create(0.3, 0));
                    
                    AudioHelp::getInstance()->stopLoopEffect();
                    m_pRollTool->runAction(Sequence::create(DelayTime::create(0.5),
                                                            MoveBy::create(1, Vec2(visibleSize.width*1.2, 0)),
                                                            MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                            CallFunc::create([=](){
                        this->_finishRoll();
                    }), NULL));
                    m_pDough->setScale(1);
                    return;
                }
            }), NULL));
        }
        
        if (3==m_nRollStep || 4==m_nRollStep) {
            m_pDough->setScale(m_pDough->getScaleX()+0.002, 1);
        }else {
            m_pDough->setScale(1,m_pDough->getScaleY()+0.002);
        }
    }else if (pDragNode->getName()=="cuttool"){
        for (int i = 0; i<m_CircleVector.size(); i++) {
            Sprite* pCirlce = m_CircleVector.at(i);
            Rect doughRect = pCirlce->getBoundingBox();
            doughRect.origin = pCirlce->getParent()->convertToWorldSpace(doughRect.origin);
            if (doughRect.containsPoint(worldPoint)) {
                pDragNode->setTouchEnabled(false);
                pDragNode->setVisible(false);
                
                m_CircleVector.erase(m_CircleVector.begin()+i);
                
                KnifeNode* pKnife = KnifeNode::create();
                this->addChildToUILayer(pKnife);
                pKnife->setLocalZOrder(10);
                pKnife->setPosition(pCirlce->getPosition());
//                pKnife->cut(pKnife->getPosition(),doughRect.size.height*0.5);
                pKnife->runAction(CircleMove::create(1, pCirlce->getPosition()+Vec2(0, 20), 100, 360));
                
                AudioHelp::getInstance()->playEffect("cut.mp3");
                this->runAction(Sequence::create(DelayTime::create(1),
                                                 CallFunc::create([=](){
                    pDragNode->setPosition(pKnife->getPosition());
                    
                    DragNode* pNode = _createDrageNode(localPath("make2/dough_small.png"),false);
                    pNode->setPosition(pCirlce->getPosition());
                    this->addChildToContentLayer(pNode);
                    m_CircleFinishVector.push_back(pNode);
                    
                    pCirlce->setVisible(false);
                    
                    pDragNode->setTouchEnabled(true);
                    pDragNode->setVisible(true);
                    if (m_CircleFinishVector.size()==4) {
                        _finishCut();
                        pDragNode->setTouchEnabled(false);
                        pDragNode->runAction(MoveBy::create(0.5, Vec2(visibleSize.width*1.5, 0)));
                    }
                    pKnife->removeFromParent();
                }), NULL));
                break;
            }
            
        }
        
    }
}

void TwistRubDoughScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    if (pDragNode->getName()=="rolltool"){
        AudioHelp::getInstance()->stopLoopEffect();
        m_pDoughGrid->stopAllActions();
        m_pDoughGrid->runAction(Waves::create(0.1, Size(20, 20), 2, 5, false, false));
    }else if (pDragNode->getName()=="cuttool"){
    }else if (pDragNode->getName()=="dough"){
        pDragNode->setVisible(false);
        pDragNode->setTouchEnabled(false);
        pDragNode->setPosition(Vec2(visibleSize.width/2, visibleSize.height*0.4));
        m_pCurDough = Sprite::create(localPath("make2/dough_small.png"));
        m_pRubGrid->addChild(m_pCurDough);
        
        m_pGuideLayer->showGuideTapVertical(m_pRubGrid->getPosition());
        m_pTouchLayer->setTouchLayerEnable();
        if(m_nRubCount==0){
            AudioHelp::getInstance()->playEffect("vo_twist_donut.mp3");
        }
    }
    
}

void TwistRubDoughScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    if (pDragNode->getName()=="rolltool"){
        AudioHelp::getInstance()->stopLoopEffect();
        m_pDoughGrid->stopAllActions();
        m_pDoughGrid->runAction(Waves::create(0.1, Size(20, 20), 2, 5, false, false));
    }
}

void TwistRubDoughScene::TouchBegan(Touch *pTouch, Event *pEvent)
{
    if (!m_pCurDough) {
        return;
    }
    Rect rect = m_pCurDough->getBoundingBox();
    rect.origin = m_pCurDough->getParent()->convertToWorldSpace(rect.origin);
    
    Vec2 pos = pTouch->getLocation();
    
    if (rect.containsPoint(pos)){
        m_pRubGrid->runAction(RepeatForever::create(Sequence::create(Waves::create(1, Size(20, 20), 2, 5, true, true),
                                                                     DelayTime::create(0.1), NULL)));
    }
    m_prePos = pTouch->getLocation();
//    int step = m_nMoveCount/20;
//    if (step>=8) {
//        AudioHelp::getInstance()->playEffect("decoration_tap.mp3");
//    }
}

void TwistRubDoughScene::TouchMove(Touch *pTouch, Event *pEvent)
{
    if (!m_pCurDough) {
        return;
    }
    Rect rect = m_pCurDough->getBoundingBox();
    rect.origin = m_pCurDough->getParent()->convertToWorldSpace(rect.origin);
    
    Vec2 pos = pTouch->getLocation();
    
    if (rect.containsPoint(pos)) {
        int step = m_nMoveCount/20;
        if (step==8) {
        }else if (step==9) {
        }else if (step>9) {
            if (fabs((pTouch->getLocation()-m_prePos).x)>50) {
                m_nMoveCount+=1;
            }
        }else{
            m_nMoveCount++;
        }
        if (m_nMoveCount%20==0) {
            step = m_nMoveCount/20;
            m_pCurDough->setTexture(localPath("make2/animation/animation"+std::to_string(step)+".png"));
//            if (step==8){
//                m_pGuideLayer->showGuideMove(m_pRubGrid->getPosition()+Vec2(-100, 100), m_pRubGrid->getPosition());
//            }else if (step==9) {
//                m_pGuideLayer->showGuideMove(m_pRubGrid->getPosition()+Vec2(100, 100), m_pRubGrid->getPosition());
//            }else if (step==10) {
//                
//                m_pGuideLayer->showGuideTapHorizonal(m_pRubGrid->getPosition());
            //            }
            
            if (step>=10) {
                AudioHelp::getInstance()->playEffect("decoration_tap.mp3");
            }
            if (step==13) {
                m_pTouchLayer->setTouchLayerEnable(false);
                m_nMoveCount = 0;
                m_nRubCount++;
                Vec2 pos2  = CMVisibleRect::getPosition(70+50*(m_nRubCount-1), 650);
                Sprite* pSprite = Sprite::createWithTexture(m_pCurDough->getTexture());
                this->addChildToContentLayer(pSprite);
                pSprite->setPosition(m_pRubGrid->getPosition());
                m_TwistVector.push_back(pSprite);
                
                m_pGuideLayer->removeGuide();
                this->runAction(Sequence::create(DelayTime::create(0.3),
                                                 CallFunc::create([=](){
                    m_pCurDough->removeFromParent();
                    m_pCurDough = nullptr;
                    pSprite->runAction(Sequence::create(Spawn::create(MoveTo::create(0.5, pos2),
                                                                      ScaleTo::create(0.5, 0.5), NULL),
                                                        CallFunc::create([=](){
                        if (m_nRubCount>=4) {
                            //finish
                            _finishRub();
                            m_pTouchLayer->setTouchLayerEnable(false);
                        }else{
                            DragNode* pNode = m_CircleFinishVector.at(m_nRubCount);
                            pNode->setTouchEnabled(true);
                        }
                    }), NULL));
                }), NULL));
            }
        }
    }
}

void TwistRubDoughScene::TouchEnded(Touch *pTouch, Event *pEvent)
{
    m_pRubGrid->stopAllActions();
    
    m_pGuideLayer->removeGuide();
    
    if (!m_pCurDough) {
        return;
    }
    if(m_nMoveCount<=0){
        return;
    }
    Vec2 pos = pTouch->getLocation();
    
    int step = m_nMoveCount/20;
    if (step==8) {
        if ((pTouch->getLocation()-m_prePos).x>50 && (pTouch->getLocation()-m_prePos).y<-50) {
            m_nMoveCount+=20;
        }
    }else if (step==9) {
        if (fabs((pTouch->getLocation()-m_prePos).x)>50) {
            m_nMoveCount+=20;
        }
    }
    if (m_nMoveCount%20==0) {
        step = m_nMoveCount/20;
        m_pCurDough->setTexture(localPath("make2/animation/animation"+std::to_string(step)+".png"));
        if (step==8 || step==9){
            m_prePos = pTouch->getLocation();
        }
        if (step>=8) {
            AudioHelp::getInstance()->playEffect("decoration_tap.mp3");
        }
        if (step==8) {
            m_pGuideLayer->showGuideMove(m_pRubGrid->getPosition()+Vec2(-100, 100), m_pRubGrid->getPosition());
        }else if (step==9) {
            m_pGuideLayer->showGuideMove(m_pRubGrid->getPosition()+Vec2(100, 100), m_pRubGrid->getPosition());
        }else if (step>=10 && step<12){
            m_pGuideLayer->showGuideTapHorizonal(m_pRubGrid->getPosition());
        }else if (step<13){
            m_pGuideLayer->showGuideTapHorizonal(m_pRubGrid->getPosition()-Vec2(0, -50));
        }
    }
}
#pragma mark - initData
void TwistRubDoughScene::_initData()
{
    setExPath("content/make/twist/");
    m_nNextSceneTag = GameUIEvent::eSceneTagFryTwist;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagFryTwist;
}
#pragma mark ==== roll UI =====
void TwistRubDoughScene::_createRollUI()
{
    m_pFlourSprinkle = Sprite::create("content/make/donut/make2/powder.png");
    this->addChildToContentLayer(m_pFlourSprinkle);
    CMVisibleRect::setPositionAdapted(m_pFlourSprinkle, 320, 480);
    
    m_pDoughGrid = NodeGrid::create();
    this->addChildToContentLayer(m_pDoughGrid);
    CMVisibleRect::setPositionAdapted(m_pDoughGrid, 320, 480);
    
    m_pDough = Sprite::create(localPath("make2/paste0.png"));
    m_pDoughGrid->addChild(m_pDough);
    
    m_pDoughGrid->setVisible(false);
    m_pFlourSprinkle->setOpacity(0);
}


void TwistRubDoughScene::_pourDough()
{
    m_pBowl = Sprite::create(localPath("make1/bowl.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 210-visibleSize.width/2, 350);
    
    m_pBowlFront = Sprite::create(localPath("make1/bowl_on.png"));
    this->addChildToContentLayer(m_pBowlFront);
    CMVisibleRect::setPositionAdapted(m_pBowlFront, 210-visibleSize.width/2, 350);
    
    m_pDoughPan = Sprite::create(localPath("make1/stir10.png"));
    this->addChildToContentLayer(m_pDoughPan);
    CMVisibleRect::setPositionAdapted(m_pDoughPan, 210-visibleSize.width/2, 450);
    m_pDoughPan->setScale(1.1);
    
    m_pBowl->setLocalZOrder(5);
    m_pDoughPan->setLocalZOrder(6);
    m_pBowlFront->setLocalZOrder(10);
    auto bowlAction = Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width/2, 50+100)),
                                       RotateBy::create(0.3, 50),
                                       DelayTime::create(0.3),
                                       MoveBy::create(0.3, Vec2(-200, 0)),
                                       RotateBy::create(0.3, Vec3(0, -40, 0)),
                                       MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)),
                                       CallFuncN::create([=](Node* pNode){
        pNode->removeFromParent();
    }), NULL);
    m_pBowl->runAction(bowlAction);
    m_pBowlFront->runAction(bowlAction->clone());
    m_pDoughPan->runAction(Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width/2, 50)),
                                            RotateBy::create(0.3, 50),
                                            JumpBy::create(1, Vec2(50, 0), 180, 1),
                                            MoveBy::create(0.5, Vec2(0, -30)),
                                            CallFunc::create([=](){
        m_pDoughGrid->setVisible(true);
        m_pDoughPan->removeFromParent();
        m_pDoughPan = nullptr;
    }), NULL));
}

void TwistRubDoughScene::_showSpreadFlour()
{
    Sprite* pFlourBag = Sprite::create("content/make/donut/make1/flour1_down.png");
    this->addChildToContentLayer(pFlourBag);
    pFlourBag->setLocalZOrder(10);
    CMVisibleRect::setPositionAdapted(pFlourBag, 320+visibleSize.width, 480);
    ccBezierConfig cfg;
    cfg.controlPoint_1 = CMVisibleRect::getPosition(200, 200);
    cfg.controlPoint_2 = CMVisibleRect::getPosition(900, 400);
    cfg.endPosition = CMVisibleRect::getPosition(800, 900);
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/flour2.plist");
    pFlourBag->addChild(pParticle);
    pParticle->setPosition(Vec2(49, 25));
    pParticle->setVisible(false);
    
    pFlourBag->setRotation(100);
    pFlourBag->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                          Spawn::create(JumpBy::create(1, Vec2::ZERO, 30, 2),
                                                        RotateBy::create(0.5, -140),
                                                        Sequence::create(MoveBy::create(0.3, Vec2(200, 300)),
                                                                         MoveBy::create(0.5, Vec2(-500.0+20.0*(arc4random()%10), -200)), NULL), NULL),
                                          CallFunc::create([=](){
        pParticle->setVisible(true);
    }),
                                          ScaleTo::create(0.3, 1.1,0.8),
                                          ScaleTo::create(0.3, 1),
                                          //                                          MoveBy::create(1, Vec2(0, visibleSize.height)),
                                          BezierTo::create(0.5, cfg),
                                          CallFunc::create([=](){
        pParticle->removeFromParent();
    }), NULL));
    for (int i = 0; i<6; i++) {
        Sprite* pFlour = Sprite::create("content/make/donut/make2/powder0.png");
        this->addChildToContentLayer(pFlour);
        pFlour->setLocalZOrder(5);
        Vec2 pos = Vec2(100.0+10.0*(arc4random()%50),200.0+10.0*(arc4random()%50));
        CMVisibleRect::setPositionAdapted(pFlour, pos);
        pFlour->setOpacity(0);
        float dt = 0.1*(arc4random()%10);
        pFlour->runAction(Sequence::create(DelayTime::create(1.5+dt),
                                           FadeIn::create(0.5),
                                           Spawn::create(FadeOut::create(0.5),
                                                         ScaleTo::create(0.3, 2), NULL),
                                           CallFunc::create([=](){
            pFlour->removeFromParent();
        }), NULL));
    }
    m_pFlourSprinkle->runAction(Sequence::create(DelayTime::create(2),
                                                 FadeIn::create(2), NULL));
}

void TwistRubDoughScene::_showRollTool()
{
    AudioHelp::getInstance()->playEffect("vo_roll_dough.mp3");
    m_pRollTool = _createDrageNode("content/make/donut/make2/rolling.png");
    this->addChildToContentLayer(m_pRollTool);
    m_pRollTool->setLocalZOrder(10);
    CMVisibleRect::setPositionAdapted(m_pRollTool, 500+visibleSize.width, 600);
    m_pRollTool->setTouchEnabled(false);
    m_pRollTool->setName("rolltool");
    m_pRollTool->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                            CallFunc::create([=](){
        m_pRollTool->setOrgPositionDefault();
        m_pRollTool->setTouchEnabled(true);
    }), NULL));
}

void TwistRubDoughScene::_removeRollTool()
{
    
}

void TwistRubDoughScene::_finishRoll()
{
    AudioHelp::getInstance()->stopLoopEffect();
    _showCutTool();
    _produceCircleGuide();
}

void TwistRubDoughScene::_finishCut()
{
    int i = 0;
    for_each(m_CircleFinishVector.begin(), m_CircleFinishVector.end(), [=,&i](DragNode* pNode){
        Vec2 pos = CMVisibleRect::getPosition(80+160*i, 650);
        pNode->setName("dough");
        pNode->runAction(Sequence::create(Spawn::create(MoveTo::create(0.5, pos),
                                                        ScaleTo::create(0.5, 0.8), NULL),
                                          CallFunc::create([=](){
            pNode->setOrgPositionDefault();
            if (i==0) {
                pNode->setTouchEnabled(true);
                m_pGuideLayer->showGuideMove(pos, Vec2(visibleSize.width/2, visibleSize.height*0.4));
            }
        }), NULL));
        i++;
    });
    m_pRubGrid = NodeGrid::create();
    this->addChildToContentLayer(m_pRubGrid);
    CMVisibleRect::setPositionAdapted(m_pRubGrid, 320, 400);
    
    m_pDough->runAction(Sequence::create(FadeOut::create(0.5),
                                         CallFunc::create([=](){
        m_pDough->removeFromParent();
    }), NULL));
    m_CircleVector.clear();
}

void TwistRubDoughScene::_finishRub()
{
    int i = 0;
    for_each(m_TwistVector.begin(), m_TwistVector.end(), [=,&i](Sprite* pSprite){
        pSprite->runAction(Spawn::create(MoveBy::create(0.5, Vec2(120+40*i++, -200)),
                                         ScaleTo::create(0.5, 1), NULL));
    });
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        
        SceneManager::replaceTheScene<FryTwistScene>();
    }), NULL));
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->playEffect(((arc4random()%100)>50?"vo_nice_work.mp3":"vo_well_done.mp3"));
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
}

void TwistRubDoughScene::_showCutTool()
{
    AudioHelp::getInstance()->playEffect("vo_cut4circle.mp3");
    AudioHelp::getInstance()->playEffect("cut_tool_fall.mp3");
    m_pCutTool = _createDrageNode(localPath("make2/knife1.png"));
    this->addChildToUILayer(m_pCutTool);
    CMVisibleRect::setPositionAdapted(m_pCutTool, 320+visibleSize.width, 800);
    m_pCutTool->setTouchEnabled(false);
    m_pCutTool->setName("cuttool");
    m_pCutTool->getDragSprite()->setAnchorPoint(Vec2(70.0/m_pCutTool->getContentSize().width, 260.0/m_pCutTool->getContentSize().height));
    
    Sprite* pKnife = Sprite::create(localPath("make2/knife2.png"));
    m_pCutTool->addChild(pKnife,-1);
    pKnife->setName("knife");
    
    Sprite* pShadow = Sprite::create(localPath("make2/knife3.png"));
    m_pCutTool->addChild(pShadow,-2);
    pShadow->setAnchorPoint(m_pCutTool->getDragSprite()->getAnchorPoint());
    pShadow->setName("shadow");
    
    m_pCutTool->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                           CallFunc::create([=](){
        m_pCutTool->setOrgPositionDefault();
        m_pCutTool->setTouchEnabled(true);
    }), NULL));
    
}

void TwistRubDoughScene::_produceCircleGuide()
{
    Vec2 pos = m_pDoughGrid->getPosition();
    Vec2 posDough = pos;
    
    Sprite* pCircle = Sprite::create(localPath("make2/marking.png"));
    int radius = (sqrt(2)-1)*pCircle->getContentSize().height;
    float radom = 10.0*(arc4random()%(radius/10));
    float posY = sqrt(powf(2*radius, 2)-powf(radom, 2));
    Vec2 offset = Vec2(radom, posY);
    pCircle->setPosition(offset+posDough);
    this->addChildToContentLayer(pCircle);
    m_CircleVector.push_back(pCircle);
    
    pos = offset;
    for(int i = 1 ;i<4 ;i++){
        Sprite* pC = Sprite::createWithTexture(pCircle->getTexture());
        Vec2 temp = Vec2(pos.y, -pos.x);
        pC->setPosition(posDough+temp);
        this->addChildToContentLayer(pC);
        m_CircleVector.push_back(pC);
        pos = temp;
    }
}
void TwistRubDoughScene::_reset()
{
    
}