
#include "CutDonutScene.h"


CutDonutScene::CutDonutScene()
{
    m_pBowl = nullptr;
    m_nRollCount = 0;
    m_nRollStep = 0;
    m_bRollToolReady = false;
    m_pDoughGrid = nullptr;
    
    m_nCutCount = 0;
}

CutDonutScene::~CutDonutScene()
{
    
}
bool CutDonutScene::init()
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
    
    m_pGameUI->showNormalLayout();
    return true;
}

void CutDonutScene::onEnter()
{
    ExtensionScene::onEnter();
}

void CutDonutScene::onExit()
{
    ExtensionScene::onExit();
}

void CutDonutScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void CutDonutScene::onButtonCallback(Button* btn)
{
    if (btn->getTag()==GameUILayoutLayer::eUIButtonTagReset) {
        _reset();
    }
    ExtensionScene::onButtonCallback(btn);
}

void CutDonutScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(10);
    if (pDragNode->getName()=="rolltool") {
        if (!m_bRollToolReady) {
            m_bRollToolReady = true;
            m_pRollTool->runAction(RotateBy::create(0.3, 20));
        }
        AudioHelp::getInstance()->playLoopEffect("roll_dough.mp3");
        m_pDoughGrid->runAction(RepeatForever::create(Sequence::create(Waves::create(1, Size(20, 20), 2, 5, true, true),
                                                                       DelayTime::create(0.1), NULL)));
    }
}

void CutDonutScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
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
            m_pDough = Sprite::create(localPath("make2/paste"+std::to_string(m_nRollStep)+".png"));
            m_pDoughGrid->addChild(m_pDough);
            m_pDough->setOpacity(100);
            m_pDough->runAction(Sequence::create(FadeTo::create(0.1, 255),
                                                 CallFunc::create([=](){
                if (3==m_nRollStep || 8==m_nRollStep) {
                    pDragNode->runAction(RotateBy::create(0.1, -90));
                }else if (6==m_nRollStep) {
                    pDragNode->runAction(RotateBy::create(0.1, 90));
                }else if (9==m_nRollStep) {
                    pDragNode->setTouchEnabled(false);
                    m_pDoughGrid->stopAllActions();
                    m_pDoughGrid->runAction(Waves::create(0.1, Size(20, 20), 2, 5, false, false));
                    m_pFlourSprinkle->runAction(FadeTo::create(0.3, 0));
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
        
        if (2==m_nRollStep || 3==m_nRollStep || 7==m_nRollStep || 8==m_nRollStep) {
            m_pDough->setScale(m_pDough->getScaleX()+0.002, 1);
        }else {
            m_pDough->setScale(1,m_pDough->getScaleY()+0.002);
        }
    }
}

void CutDonutScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    if (pDragNode->getName()=="rolltool"){
        AudioHelp::getInstance()->stopLoopEffect();
        m_pDoughGrid->stopAllActions();
        m_pDoughGrid->runAction(Waves::create(0.1, Size(20, 20), 2, 5, false, false));
    }else if (pDragNode->getName()=="cuttool"){
        
        bool bCut = false;
        for (int i = 0; i<m_pGuideVector.size(); i++) {
            Sprite* pCirlce = m_pGuideVector.at(i);
            Rect doughRect = pCirlce->getBoundingBox();
            doughRect.origin = pCirlce->getParent()->convertToWorldSpace(doughRect.origin);
            if (doughRect.containsPoint(worldPoint)){
                Vec2 pos = pCirlce->getPosition();
                pCirlce->setVisible(false);
                pDragNode->setTouchEnabled(false);
                pDragNode->setPosition(pos);
                AudioHelp::getInstance()->playEffect("cut.mp3");
                pDragNode->runAction(Sequence::create(RotateBy::create(0.5, 180),
                                                      DelayTime::create(0.5),
                                                      CallFunc::create([=](){
                    Sprite* pSprite = Sprite::create(localPath("make2/donut_die1.png"));
                    this->addChildToContentLayer(pSprite);
                    pSprite->setPosition(pos);
                    m_pCircleVector.push_back(pSprite);
                    m_pGuideVector.erase(m_pGuideVector.begin()+i);
                    pDragNode->back();
                    m_nCutCount++;
                    pDragNode->setTouchEnabled(true);
                    if (m_nCutCount==6) {
                        _finishCut();
                        pDragNode->setTouchEnabled(false);
                        pDragNode->runAction(Sequence::create(DelayTime::create(1),
                                                              MoveBy::create(0.5, Vec2(visibleSize.width, 0)), NULL));
                    }
                }), NULL));
                bCut = true;
                break;
            }
        }
        if (!bCut) {
            pDragNode->runAction(Sequence::create(EaseBackIn::create(MoveTo::create(0.5, pDragNode->getOrgPosition())),
                                             CallFunc::create([=](){
                pDragNode->setLocalZOrder(0);
            }),
                                             NULL));
        }
    }
    
}

void CutDonutScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    if (pDragNode->getName()=="rolltool"){
        AudioHelp::getInstance()->stopLoopEffect();
        m_pDoughGrid->stopAllActions();
        m_pDoughGrid->runAction(Waves::create(0.1, Size(20, 20), 2, 5, false, false));
    }
}
#pragma mark - initData
void CutDonutScene::_initData()
{
    setExPath("content/make/donut/");
    m_nNextSceneTag = GameUIEvent::eSceneTagFryDonut;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagFryDonut;
}
#pragma mark ==== roll UI =====
void CutDonutScene::_createRollUI()
{
    m_pFlourSprinkle = Sprite::create(localPath("make2/powder.png"));
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


void CutDonutScene::_pourDough()
{
    m_pBowl = Sprite::create(localPath("make1/bowl_down.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 210-visibleSize.width/2, 350);
    
    m_pBowlFront = Sprite::create(localPath("make1/bowl_on.png"));
    this->addChildToContentLayer(m_pBowlFront);
    CMVisibleRect::setPositionAdapted(m_pBowlFront, 210-visibleSize.width/2, 350);
    
    m_pDoughPan = Sprite::create(localPath("make1/stir3_5.png"));
    this->addChildToContentLayer(m_pDoughPan);
    CMVisibleRect::setPositionAdapted(m_pDoughPan, 210-visibleSize.width/2, 350+50);
    
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
                                            JumpBy::create(0.8, Vec2::ZERO, 120, 1),
                                           MoveBy::create(0.5, Vec2(50, -30)),
                                           CallFunc::create([=](){
        m_pDoughGrid->setVisible(true);
        m_pDoughPan->removeFromParent();
        m_pDoughPan = nullptr;
    }), NULL));
}

void CutDonutScene::_showSpreadFlour()
{
    Sprite* pFlourBag = Sprite::create(localPath("make1/flour1_down.png"));
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
        Sprite* pFlour = Sprite::create(localPath("make2/powder0.png"));
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

void CutDonutScene::_showRollTool()
{
    AudioHelp::getInstance()->playEffect("vo_roll_dough.mp3");
    AudioHelp::getInstance()->playEffect("cut_tool_fall.mp3");
    m_pRollTool = _createDrageNode(localPath("make2/rolling.png"));
    this->addChildToContentLayer(m_pRollTool);
    m_pRollTool->setLocalZOrder(10);
    CMVisibleRect::setPositionAdapted(m_pRollTool, 480+visibleSize.width, 320);
    m_pRollTool->setTouchEnabled(false);
    m_pRollTool->setName("rolltool");
    m_pRollTool->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                            CallFunc::create([=](){
        m_pRollTool->setOrgPositionDefault();
        m_pRollTool->setTouchEnabled(true);
    }), NULL));
}

void CutDonutScene::_removeRollTool()
{
    AudioHelp::getInstance()->stopLoopEffect();
    
}

void CutDonutScene::_finishRoll()
{
    AudioHelp::getInstance()->stopLoopEffect();
    _showCutTool();
    _produceCircleGuide();
}

void CutDonutScene::_finishCut()
{
    m_pGameUI->hideReset();
    for_each(m_pCircleVector.begin(), m_pCircleVector.end(), [=](Sprite* pSprite){
        Vec2 pos = pSprite->getPosition();
        Sprite* pCircle = Sprite::create(localPath("make2/donut_paste2.png"));
        this->addChildToContentLayer(pCircle);
        pCircle->setPosition(pos);
        m_pCircleFinishVector.push_back(pCircle);
        pCircle->runAction(Sequence::create(JumpBy::create(1, Vec2::ZERO, 200, 1),
                                            CallFuncN::create([=](Node* pNode){
            Sprite* pTemp = (Sprite*)pNode;
            pTemp->setTexture(localPath("make2/donut_paste1.png"));
        }), NULL));
        pSprite->removeFromParent();
    });
    m_pDough->runAction(Sequence::create(FadeOut::create(0.5),
                                         DelayTime::create(1),
                                         CallFunc::create([=](){
        m_pGameUI->showNextLayout();
        AudioHelp::getInstance()->playEffect("done.mp3");
        AudioHelp::getInstance()->playEffect(((arc4random()%100)>50?"vo_nice_work.mp3":"vo_well_done.mp3"));
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
        this->addChildToUILayer(pParticle);
        pParticle->setLocalZOrder(10);
        pParticle->setPosition(visibleSize*0.5);
    }), NULL));
    m_pCircleVector.clear();
}

void CutDonutScene::_showCutTool()
{
    AudioHelp::getInstance()->playEffect("vo_cut6donuts.mp3");
    m_pCutTool = _createDrageNode(localPath("make2/donut_die.png"));
    this->addChildToContentLayer(m_pCutTool);
    CMVisibleRect::setPositionAdapted(m_pCutTool, 320+visibleSize.width, 800);
    m_pCutTool->setTouchEnabled(false);
    m_pCutTool->setName("cuttool");
    m_pCutTool->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                           CallFunc::create([=](){
        m_pCutTool->setOrgPositionDefault();
        m_pCutTool->setTouchEnabled(true);
    }), NULL));
    
}

void CutDonutScene::_produceCircleGuide()
{
    Vec2 pos = m_pDoughGrid->getPosition();
    Vec2 posDough = pos;
    
    Sprite* pCircle = Sprite::create(localPath("make2/marking.png"));
    
    Size circleSize = pCircle->getContentSize();
    Rect doughRect = m_pDough->getBoundingBox();
    doughRect.origin += Vec2(40, 50);
    doughRect.size = Size(467, 480);
    doughRect.origin = m_pDough->getParent()->convertToWorldSpace(doughRect.origin);
    for(int i = 0 ;i<6 ;i++){
        Sprite* pC = Sprite::createWithTexture(pCircle->getTexture());
        pC->setScale(0.8);
        pC->setPosition(doughRect.origin+Vec2(110+220*(i%2), 70+160*(i/2)));
        this->addChildToContentLayer(pC);
        m_pGuideVector.push_back(pC);
    }
}

void CutDonutScene::_reset()
{
    for_each(m_pCircleVector.begin(), m_pCircleVector.end(), [=](Node* pNode){
        pNode->removeFromParent();
    });
    m_nCutCount = 0;
    m_pCircleVector.clear();
    m_pCutTool->stopAllActions();
    m_pCutTool->back();
}