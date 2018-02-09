
#include "TwistRiseDoughScene.h"
#include "SceneManager.h"

TwistRiseDoughScene::TwistRiseDoughScene()
{
    m_pBowl = nullptr;
    m_pWhrisk = nullptr;
    m_nStep = 0;
    m_nAddCount = 0;
    m_nMixCount = 0;
    m_bWhriskIsReady = false;
    m_bReadyAdd = true;
    m_bCanTouch = false;
}

TwistRiseDoughScene::~TwistRiseDoughScene()
{
}
bool TwistRiseDoughScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    Sprite* pBg = Sprite::create("content/make/bg/bg_make1.jpg");
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        _createMixUI();
    }), NULL));
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=]{
        _showMixIngredient(0);
    }),
                                     DelayTime::create(1),
                                     CallFunc::create([=]{
        AudioHelp::getInstance()->playEffect("vo_add_ingredients.mp3");
        m_pGuideLayer->showGuideMove(Vec2(visibleSize.width*0.6, visibleSize.height*0.8),m_pBowl->getPosition());
    }), NULL));
    
//    test
//    _showMixIngredient(1);
//    m_nAddCount = 4;
//    m_nStep = 1;
//    this->runAction(Sequence::create(DelayTime::create(1),
//                                     CallFunc::create([=]{
//        _moveDownMixIngredient(IngredientTag::eTagButter);
//        _moveDownMixIngredient(IngredientTag::eTagMilk);
//    }), NULL));
    
    m_pGameUI->showNormalLayout();
    AudioHelp::getInstance()->playBackGroundMusic("background.mp3");
    return true;
}

void TwistRiseDoughScene::onEnter()
{
    ExtensionScene::onEnter();
    Analytics::getInstance()->sendScreenEvent(Flurry_EVENT_MAKE_TWIST);
}

void TwistRiseDoughScene::onExit()
{
    ExtensionScene::onExit();
}

void TwistRiseDoughScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void TwistRiseDoughScene::onButtonCallback(Button* btn)
{
    ExtensionScene::onButtonCallback(btn);
}

void TwistRiseDoughScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    ContainerNode* pContainer = (ContainerNode*)pDragNode->getUserData();
    if (pContainer) {
        pContainer->showShadow(false);
    }
    int tag = pDragNode->getTag();
    if (IngredientTag::eTagEgg == tag) {
        m_pEgg0_0->setVisible(false);
        pDragNode->setVisible(true);
    }
    pDragNode->setScale(1);
    pDragNode->setLocalZOrder(50);
    if (IngredientTag::eTagWhrisk==tag) {
        
        if (m_bWhriskIsReady){
            Sprite *stick = (Sprite*)pDragNode->getChildByName("stick");
            
            if (stick){
                stick->runAction(RepeatForever::create(AnimationHelp::createAnimate(localPath("donut/make1/blender_"), 1, 3)));
            }
            
            if (m_nMixCount/80>=8) {
                AudioHelp::getInstance()->stopLoopEffect();
                AudioHelp::getInstance()->playLoopEffect("stir_machine.mp3");
            }else{
                AudioHelp::getInstance()->playLoopEffect("machine_stir.mp3");
            }
            pDragNode->setLocalZOrder(5);
            m_pWhrisk->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.01, Vec2(0.1, 1)),
                                                                        MoveBy::create(0.01, Vec2(-0.2, -2)),
                                                                        MoveBy::create(0.01, Vec2(0.1, 1)),
                                                                        NULL)));
            Rect rect = m_pBowl->getBoundingBox();
            Rect limitRect = Rect(rect.origin.x+190, rect.origin.y+250, rect.size.width-200, rect.size.height*0.4);
            pDragNode->setLimitRect(limitRect);
            pDragNode->setOrgPositionDefault();
            pDragNode->setTouchEnabled(true);
            m_pMixture->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.3),
                                                                         CallFunc::create([=](){
                m_pMixture->setScaleX(-m_pMixture->getScaleX());
            }), NULL)));
        }
    }
    
}

void TwistRiseDoughScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    if (m_pWhrisk==pDragNode){
        Rect rect = m_pBowl->getBoundingBox();
        rect.origin = m_pBowl->getParent()->convertToWorldSpace(rect.origin);
        if (rect.containsPoint(worldPoint)){
            _mixBatter();
        }
        
    }
}

void TwistRiseDoughScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    AudioHelp::getInstance()->stopLoopEffect();
    pDragNode->stopAllActions();
 
    if (pDragNode==m_pWhrisk) {
        Sprite *stick = (Sprite*)pDragNode->getChildByName("stick");
        if (stick) {
            stick->stopAllActions();
        }
        m_pWhrisk->stopAllActions();
        m_pMixture->stopAllActions();
        m_pMixture->setRotation(0);
    }else{
        Rect rect = m_pBowl->getBoundingBox();
        rect.size = rect.size+Size(100, 100);
        if (rect.containsPoint(pDragNode->getPosition()) && m_bReadyAdd) {
            pDragNode->setTouchEnabled(false);
            m_bReadyAdd = false;
            _addIngredient(pDragNode);
        }else{
            _back(pDragNode);
        }
    }
}

void TwistRiseDoughScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->stopAllActions();
    
    if (pDragNode==m_pWhrisk) {
        Sprite *stick = (Sprite*)pDragNode->getChildByName("stick");
        if (stick) {
            stick->stopAllActions();
        }
        m_pWhrisk->stopAllActions();
        m_pMixture->stopAllActions();
        m_pMixture->setRotation(0);
    }

    _back(pDragNode);
}

#pragma mark - initData
void TwistRiseDoughScene::_initData()
{
    setExPath("content/make/");
    
    m_nNextSceneTag = GameUIEvent::eSceneTagRubTwist;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagRubTwist;
}

#pragma mark ==== Mix UI =====
void TwistRiseDoughScene::_createMixUI()
{
    m_pBowl = Sprite::create(localPath("twist/make1/bowl.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 210+110-visibleSize.width/2, 350);
    
    m_pBowlFront = Sprite::create(localPath("twist/make1/bowl_on.png"));
    this->addChildToContentLayer(m_pBowlFront);
    CMVisibleRect::setPositionAdapted(m_pBowlFront, 210+110-visibleSize.width/2, 350);
    
    m_pMixtureClipping = ClippingNode::create(Sprite::create(localPath("twist/make1/bowl_mask.png")));
    this->addChildToContentLayer(m_pMixtureClipping);
    CMVisibleRect::setPositionAdapted(m_pMixtureClipping, 210+110, 350);
    m_pMixtureClipping->setLocalZOrder(5);
    m_pMixtureClipping->setAlphaThreshold(0.5);
    
    m_pMixture = Node::create();
    m_pMixtureClipping->addChild(m_pMixture);
    m_pMixture->setPosition(Vec2(0, -25));
    
    m_pBowl->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
    m_pBowlFront->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
    m_pBowlFront->setLocalZOrder(8);
    
//    AudioHelp::getInstance()->playEffect("vo_add_ingredients.mp3");
}

void TwistRiseDoughScene::_showMixIngredient(int index)
{
    AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
    if (index==0) {
        m_pSugar = _createDrageNode(localPath("donut/make1/sugar_down.png"),false);
        this->addChildToContentLayer(m_pSugar);
        CMVisibleRect::setPositionAdapted(m_pSugar, 330+visibleSize.width, 730);
        m_pSugar->setActionPoint(Vec2(7, 90));
        m_pSugar->setTag(IngredientTag::eTagSugar);
        
        m_pFlour = _createDrageNode(localPath("donut/make1/flour2.png"),false);
        this->addChildToContentLayer(m_pFlour);
        CMVisibleRect::setPositionAdapted(m_pFlour, 570+visibleSize.width, 770);
        m_pFlour->setActionPoint(Vec2(62, 277));
        m_pFlour->setTag(IngredientTag::eTagFlour);
        
        m_pYeast = _createDrageNode(localPath("donut/make1/baking_powder1.png"),false);
        this->addChildToContentLayer(m_pYeast);
        CMVisibleRect::setPositionAdapted(m_pYeast, 450+visibleSize.width, 760);
        m_pYeast->setActionPoint(Vec2(5, 186));
        m_pYeast->setTag(IngredientTag::eTagYeast);
        
        ContainerNode* pSugar = ContainerNode::create(m_pSugar);
        pSugar->addShadow(localPath("donut/make1/sugar_shadow.png"));
        pSugar->addFront(localPath("donut/make1/sugar_on.png"));
        pSugar->addFood(localPath("donut/make1/sugar.png"),localPath("donut/make1/sugar_down.png"));
        m_pSugar->setUserData(pSugar);
        
        
        ContainerNode* pFlour = ContainerNode::create(m_pFlour);
        pFlour->addShadow(localPath("donut/make1/flour2_shadow.png"));
        m_pFlour->setUserData(pFlour);
        
        ContainerNode* pPowder = ContainerNode::create(m_pYeast);
        pPowder->addShadow(localPath("donut/make1/baking_powder3.png"));
        pPowder->addFood(localPath("donut/make1/baking_powder2.png"));
        m_pYeast->setUserData(pPowder);
        
        m_pSugar->setScale(0.6);
        m_pFlour->setScale(0.6);
        m_pYeast->setScale(0.6);
        auto action = Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                       CallFuncN::create([=](Node* pNode){
            DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
            if (pIngredient) {
                pIngredient->setOrgPositionDefault();
                pIngredient->setTouchEnabled(true);
            }
        }), NULL);
        
        m_pYeast->runAction(action);
        m_pSugar->runAction(action->clone());
        m_pFlour->runAction(action->clone());
    }else if (index==1){
        m_pWater = _createDrageNode(localPath("donut/make1/kettle1_down.png"),false);
        this->addChildToContentLayer(m_pWater);
        CMVisibleRect::setPositionAdapted(m_pWater, 570+visibleSize.width, 760);
        m_pWater->setActionPoint(Vec2(28, 231));
        m_pWater->setTag(IngredientTag::eTagWater);
        
        m_pMilk = _createDrageNode(localPath("donut/make1/milk_1.png"),false);
        this->addChildToContentLayer(m_pMilk);
        CMVisibleRect::setPositionAdapted(m_pMilk, 420+visibleSize.width, 750);
        m_pMilk->setActionPoint(Vec2(12, 144));
        m_pMilk->setTag(IngredientTag::eTagMilk);
        
        m_pEgg0_0 = Sprite::create(localPath("donut/make1/egg2.png"));
        this->addChildToContentLayer(m_pEgg0_0);
        CMVisibleRect::setPositionAdapted(m_pEgg0_0, 310+visibleSize.width, 720);
        m_pEgg0 = _createDrageNode(localPath("donut/make1/egg_1.png"),false);
        this->addChildToContentLayer(m_pEgg0);
        CMVisibleRect::setPositionAdapted(m_pEgg0, 310-20+visibleSize.width, 730);
        m_pEgg0->setTag(IngredientTag::eTagEgg);
        
        
        KettleNode* pKettleWater = KettleNode::create(m_pWater);
        pKettleWater->addShadow(localPath("donut/make1/kettle_shadow.png"));
        pKettleWater->addWater(localPath("donut/make1/kettle1_water.png"), localPath("donut/make1/kettle_mask.png"));
        pKettleWater->addFront(localPath("donut/make1/kettle1_on.png"));
        pKettleWater->addPourWater(localPath("donut/make1/kettle2_waiter2.png"));
        m_pWater->setUserData(pKettleWater);
        
        KettleNode* pKettle = KettleNode::create(m_pMilk);
        pKettle->addShadow(localPath("donut/make1/milk_shadow.png"));
        pKettle->addWater(localPath("donut/make1/milk_2.png"), localPath("donut/make1/milk_2.png"));
        m_pMilk->setUserData(pKettle);
        
        m_pEgg0->setVisible(false);
        m_pEgg0_0->setScale(0.6);
        m_pEgg0->setScale(0.6);
        m_pWater->setScale(0.6);
        m_pMilk->setScale(0.6);
        
        auto action = Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                       CallFuncN::create([=](Node* pNode){
            DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
            if (pIngredient) {
                pIngredient->setOrgPositionDefault();
                pIngredient->setTouchEnabled(true);
            }
        }), NULL);
        m_pWater->runAction(action);
        m_pMilk->runAction(action->clone());
        m_pEgg0->runAction(action->clone());
        m_pEgg0_0->runAction(action->clone());
    }else if (index==2) {
        m_pSalt = _createDrageNode(localPath("donut/make1/salt.png"));
        this->addChildToContentLayer(m_pSalt);
        CMVisibleRect::setPositionAdapted(m_pSalt, 310+visibleSize.width, 750);
        m_pSalt->setTag(IngredientTag::eTagSalt);
        m_pSalt->setActionPoint(Vec2(69, 168));
        
        
        m_pButter = _createDrageNode(localPath("donut/make1/butter.png"));
        this->addChildToContentLayer(m_pButter);
        CMVisibleRect::setPositionAdapted(m_pButter, 450+visibleSize.width, 720);
        m_pButter->setTag(IngredientTag::eTagButter);
        
        m_pOil = _createDrageNode(localPath("twist/make1/oil1.png"));
        this->addChildToContentLayer(m_pOil);
        CMVisibleRect::setPositionAdapted(m_pOil, 560+visibleSize.width, 780);
        m_pOil->setTag(IngredientTag::eTagOil);
        
        ContainerNode* pSalt = ContainerNode::create(m_pSalt);
        pSalt->addShadow(localPath("donut/make1/salt_shadow.png"));
        m_pSalt->setUserData(pSalt);
        
        
        ContainerNode* pButter = ContainerNode::create(m_pButter);
        pButter->addShadow(localPath("donut/make1/butter_shadow.png"));
        pButter->addFood(localPath("donut/make1/butter_1.png"));
        m_pButter->setUserData(pButter);
        
        m_pSalt->setScale(0.6);
        m_pButter->setScale(0.6);
        m_pOil->setScale(0.6);
        auto action = Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                       CallFuncN::create([=](Node* pNode){
            DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
            if (pIngredient) {
                pIngredient->setOrgPositionDefault();
                pIngredient->setTouchEnabled(true);
            }
        }), NULL);
        m_pButter->runAction(action);
        m_pSalt->runAction(action->clone());
        m_pOil->runAction(action->clone());
    }
}

void TwistRiseDoughScene::_moveDownMixIngredient(int tag)
{
    return;
    DragNode*pNode = dynamic_cast<DragNode*>(m_pContentLayer->getChildByTag(tag));
    if (!pNode) {
        return;
    }
    int index = tag - IngredientTag::eTagBegin-1;
    static Vec2 sEndPos[]={
        Vec2(510, 350), //sugar
        Vec2(540, 430), //powder
        Vec2(470, 570), //flour
        Vec2(530, 375), //water
        Vec2(530, 510), //milk
        Vec2(510, 230),//egg
        Vec2(520, 270), //butter
        Vec2(480, 400), //salt
        Vec2(540, 570), //Oil
    
    };
    Vec2 pos = CMVisibleRect::getPositionAdapted(sEndPos[index]);
    ccBezierConfig cfg;
    cfg.controlPoint_1 = cfg.controlPoint_2 = Vec2(600, 800);
    cfg.endPosition = pos;
    auto action = Sequence::create(Spawn::create(BezierTo::create(0.5, cfg),
                                                 ScaleTo::create(0.5, 1), NULL),
                                   CallFuncN::create([=](Node* pNode){
        DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
        if (pIngredient) {
            pIngredient->setOrgPositionDefault();
            pIngredient->setTouchEnabled(true);
        }
    }), NULL);
    pNode->setLocalZOrder(IngredientTag::eTagEnd-pNode->getTag());
    switch (tag) {
        case IngredientTag::eTagEgg:
        {
            m_pEgg0_0->runAction(action);
            m_pEgg0->runAction(action->clone());
            m_pEgg0_0->setLocalZOrder(IngredientTag::eTagEnd-m_pEgg0_0->getTag());
            m_pEgg0->setLocalZOrder(IngredientTag::eTagEnd-m_pEgg0->getTag());
        }
            break;
        case IngredientTag::eTagWater:
        {
            m_pWater->runAction(action);
            KettleNode* pContainer = (KettleNode*)m_pWater->getUserData();
            if (pContainer) {
                pContainer->waveWater(5);
            }
        }
            break;
        case IngredientTag::eTagMilk:
        {
            m_pMilk->runAction(action);
            KettleNode* pContainer = (KettleNode*)m_pMilk->getUserData();
            if (pContainer) {
                pContainer->waveWater(5);
            }
        }
            break;
            
        default:
        {
            pNode->runAction(action);
        }
            break;
    }
}
void TwistRiseDoughScene::_showWhisk()
{
    AudioHelp::getInstance()->playEffect("vo_mix_ingredients.mp3");
    m_pBowl->runAction(MoveBy::create(0.5, Vec2(-110, 0)));
    m_pBowlFront->runAction(MoveBy::create(0.5, Vec2(-110, 0)));
    m_pMixtureClipping->runAction(MoveBy::create(0.5, Vec2(-110, 0)));
    
    m_bWhriskIsReady = false;
    m_pWhrisk = _createDrageNode(localPath("donut/make1/blender.png"));
    this->addChildToContentLayer(m_pWhrisk);
    CMVisibleRect::setPositionAdapted(m_pWhrisk, 550+visibleSize.width, 360);
    m_pWhrisk->setActionPoint(Vec2(45, 60));
    m_pWhrisk->setLocalZOrder(10);
    m_pWhrisk->setTag(IngredientTag::eTagWhrisk);
    
    Sprite* pSprite = Sprite::create(localPath("donut/make1/blender_1.png"));
    m_pWhrisk->addChild(pSprite,-1);
    pSprite->setName("stick");
    pSprite->setVisible(false);
    
    Sprite* pShadow = Sprite::create(localPath("donut/make1/blender_shadow.png"));
    m_pWhrisk->addChild(pShadow,-1);
    pShadow->setName("shadow");
    
    m_pWhrisk->setTouchEnabled(false);
    m_pWhrisk->runAction(Sequence::create(DelayTime::create(0.5),
                                          EaseBackOut::create(MoveBy::create(1, Vec2(-visibleSize.width, 0))),
                                          CallFunc::create([=]()
                                                           {
                                                               m_pWhrisk->setOrgPositionDefault();
                                                               m_pWhrisk->setTouchEnabled(true);
                                                           }),
                                          NULL));
}

void TwistRiseDoughScene::_removeWhisk()
{
    m_pWhrisk->setTouchEnabled(false);
    m_pWhrisk->runAction(Sequence::create(DelayTime::create(0.5),
                                          EaseBackOut::create(MoveBy::create(1, Vec2(visibleSize.width, 0))),
                                          CallFunc::create([=]()
                                                           {
                                                               m_pWhrisk->removeFromParent();
                                                           }),
                                          NULL));
}


#pragma mark ======== deel ========================
void TwistRiseDoughScene::_back(DragNode* pIngredient)
{
    pIngredient->back(0,[=](){
        ContainerNode* pContainer = (ContainerNode*)pIngredient->getUserData();
        if (pContainer) {
            pContainer->showShadow();
        }
        int tag = pIngredient->getTag();
        if (IngredientTag::eTagEgg == tag) {
            m_pEgg0_0->setVisible(true);
            pIngredient->setVisible(false);
        }
    });
    pIngredient->runAction(ScaleTo::create(0.5, 0.6));
}

void TwistRiseDoughScene::_addIngredient(DragNode* pIngredient)
{
    int tag = pIngredient->getTag();
    std::string lNameStr = "";
    float dt = 1;
    Vec2 posAdd = Vec2::ZERO;
    pIngredient->setLocalZOrder(5);
    switch (tag) {
#pragma mark === add Sugar ===
        case IngredientTag::eTagSugar:
        {
            lNameStr = "donut/make1/bowl_sugar.png";
            pIngredient->setTouchEnabled(false);
            
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+80, m_pBowl->getPositionY()+200));
            pIngredient->runAction(Sequence::create(RotateBy::create(0.3, -120),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playPourPowderEffect();
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/sugar.plist");
                this->addChildToContentLayer(pParticle);
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                pParticle->setLocalZOrder(6);
                
                ContainerNode* pContainer = (ContainerNode*)pIngredient->getUserData();
                if (pContainer) {
                    pContainer->pourFood();
                }
            }),
                                                    JumpBy::create(1.5, Vec2::ZERO, 20, 5),
                                                    Spawn::create(RotateTo::create(0.3, 0),
                                                                  MoveBy::create(0.3, Vec2(0, 100)), NULL),
                                                    MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=]{
                m_nAddCount++;
                if (m_nAddCount==3) {
                    _finishAdd(0);
                }
                
                m_bReadyAdd = true;
                pIngredient->removeFromParent();
            }),
                                                    NULL));
        }
            break;
#pragma mark === add Flour ===
        case IngredientTag::eTagFlour:
        {
            lNameStr = "donut/make1/bowl_flour.png";
            pIngredient->setTouchEnabled(false);
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+120, m_pBowl->getPositionY()+300));
            pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -80),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playPourPowderEffect();
                
                Sprite* pBag = Sprite::create(localPath("donut/make1/flour1_down.png"));
                this->addChildToContentLayer(pBag);
                pBag->setLocalZOrder(8);
                
                Sprite* pBagFront = Sprite::create(localPath("donut/make1/flour1_on.png"));
                this->addChildToContentLayer(pBagFront);
                pBagFront->setLocalZOrder(11);
                
                pBag->setPosition(pIngredient->getPosition());
                pBagFront->setPosition(pIngredient->getPosition());
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/sugar.plist");
                this->addChildToContentLayer(pParticle);
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pBag->convertToWorldSpace(Vec2(49, 27))));
                pParticle->setLocalZOrder(10);
                
                pBag->runAction(Sequence::create(ScaleTo::create(0.5, 1.1,0.9),
                                                 DelayTime::create(1),
                                                 ScaleTo::create(0.5, 1),
                                                 RotateBy::create(0.3, 80),
                                                 MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                 CallFunc::create([=](){
                    m_nAddCount++;
                    if (m_nAddCount==3) {
                        _finishAdd(0);
                    }
                    m_bReadyAdd = true;
                    pBagFront->removeFromParent();
                    pBag->removeFromParent();
                }),
                                                 NULL));
                pBagFront->runAction(Sequence::create(ScaleTo::create(0.5, 1.1,0.9),
                                                      DelayTime::create(1),
                                                      ScaleTo::create(0.5, 1),
                                                      RotateBy::create(0.3, 100),
                                                      MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                      NULL));
                pIngredient->removeFromParent();
            }),
                                                    NULL));
        }
            break;
#pragma mark === add Yeast ===
        case IngredientTag::eTagYeast:
        {
            lNameStr = "donut/make1/bowl_baking_powder.png";
            pIngredient->setTouchEnabled(false);
            posAdd = Vec2(0,-40);
            
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+200, m_pBowl->getPositionY()+250));
            pIngredient->setLocalZOrder(10);
            pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -100),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playPourPowderEffect();
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/sugar.plist");
                this->addChildToContentLayer(pParticle);
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                pParticle->setLocalZOrder(10);
                
                ContainerNode* pContainer = (ContainerNode*)pIngredient->getUserData();
                if (pContainer) {
                    pContainer->pourFood();
                }
            }),
                                                    ScaleTo::create(0.5, 1.1,0.9),
                                                    DelayTime::create(1),
                                                    ScaleTo::create(0.5, 1),
                                                    RotateTo::create(0.3, 0),
                                                    MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                m_nAddCount++;
                if (m_nAddCount==3) {
                    _finishAdd(0);
                }
                m_bReadyAdd = true;
                pIngredient->removeFromParent();
            }),
                                                    NULL));
        }
            break;
#pragma mark === add Egg ===
        case IngredientTag::eTagEgg:
        {
            lNameStr = "donut/make1/egg_4.png";
            posAdd = Vec2(50,10);
            pIngredient->setTouchEnabled(false);
            pIngredient->removeFromParent();
            Sprite* pEgg = Sprite::create(localPath("donut/make1/egg_1.png"));
            this->addChildToContentLayer(pEgg);
            pEgg->setPosition(m_pBowl->getPosition()+Vec2(50, 150));
            AudioHelp::getInstance()->playAddEggEffect();
            pEgg->runAction(Sequence::create(AnimationHelp::createAnimate(localPath("donut/make1/egg_"), 1, 2,false),
                                             CallFunc::create([=](){
                Sprite* pEgg2 = Sprite::create(localPath("donut/make1/egg_3.png"));
                this->addChildToContentLayer(pEgg2);
                pEgg2->setLocalZOrder(6);
                pEgg2->setPosition(m_pBowl->getPosition()+Vec2(50, 120));
                pEgg2->runAction(Sequence::create(DelayTime::create(1),
                                                  CallFunc::create([=](){
                    m_nAddCount++;
                    if (m_nAddCount==6) {
                        _finishAdd(1);
                    }
                    m_bReadyAdd = true;
                    pEgg2->removeFromParent();
                }), NULL));
                pEgg->removeFromParent();
            }), NULL));
        }
            break;
#pragma mark === add Water ===
        case IngredientTag::eTagWater:
        {
            lNameStr = "twist/make1/bowl_water.png";
            pIngredient->setTouchEnabled(false);
            posAdd = Vec2(0,30);
            
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+100, m_pBowl->getPositionY()+250));
            pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -100),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playPourMilkEffect();
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/water2.plist");
                this->addChildToContentLayer(pParticle);
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                pParticle->setLocalZOrder(6);
                pParticle->setName("waterstream");
                
                ParticleSystemQuad* pWaterParticle = ParticleSystemQuad::create("particle/water.plist");
                this->addChildToContentLayer(pWaterParticle);
                pWaterParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                pWaterParticle->setLocalZOrder(6);
                pWaterParticle->setName("waterdrop");
                
                KettleNode* pContainer = (KettleNode*)pIngredient->getUserData();
                if (pContainer) {
                    pContainer->pourWater();
                }
            }),
                                                    DelayTime::create(2),
                                                    RotateTo::create(0.3, 0),
                                                    MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                m_nAddCount++;
                if (m_nAddCount==6) {
                    _finishAdd(1);
                }
                m_bReadyAdd = true;
                pIngredient->removeFromParent();
            }),
                                                    NULL));
        }
            break;
#pragma mark === add Milk ===
        case IngredientTag::eTagMilk:
        {
            lNameStr = "twist/make1/bowl_milk.png";
            pIngredient->setTouchEnabled(false);
            
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+80, m_pBowl->getPositionY()+300));
            pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -80),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playPourMilkEffect();
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/milk.plist");
                this->addChildToContentLayer(pParticle);
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                pParticle->setLocalZOrder(6);
                pParticle->setName("milkstream");
                pParticle->setScaleX(-1);
                
                KettleNode* pContainer = (KettleNode*)pIngredient->getUserData();
                if (pContainer) {
                    pContainer->pourWater();
                }
            }),
                                                    DelayTime::create(2),
                                                    RotateTo::create(0.3, 0),
                                                    MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                m_nAddCount++;
                if (m_nAddCount==6) {
                    _finishAdd(1);
                }
                m_bReadyAdd = true;
                pIngredient->removeFromParent();
            }),
                                                    NULL));
        }
            break;
#pragma mark === add Butter ===
        case IngredientTag::eTagButter:
        {
            lNameStr = "twist/make1/bowl_butter.png";
            pIngredient->setTouchEnabled(false);
            
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+50, m_pBowl->getPositionY()+200));
            pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -30),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("buter.mp3");
                ContainerNode* pContainer = (ContainerNode*)pIngredient->getUserData();
                if (pContainer) {
                    pContainer->pourFood();
                }
                for (int i = 0; i<3; i++) {
                    Sprite* pButter = Sprite::create(localPath("donut/make1/butter_2.png"));
                    this->addChildToContentLayer(pButter);
                    pButter->setPosition(pIngredient->getPosition());
                    pButter->setLocalZOrder(10);
                    pButter->setRotation(i*20);
                    pButter->runAction(Sequence::create(DelayTime::create(0.01+0.1*i),
                                                        Spawn::create(MoveBy::create(0.5, Vec2(-50, -100)),
                                                                      RotateBy::create(0.5, 10+i*10), NULL),
                                                        CallFunc::create([=](){
                        pButter->removeFromParent();
                    }), NULL));
                }
            }),
                                                    DelayTime::create(1),
                                                    RotateBy::create(0.3, 30),
                                                    MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                m_nAddCount++;
                if (m_nAddCount==9) {
                    _finishAdd(2);
                }
                m_bReadyAdd = true;
            }),NULL));
        }
            break;
#pragma mark === add Salt ===
        case IngredientTag::eTagSalt:
        {
            lNameStr = "twist/make1/bowl_slat.png";
            pIngredient->setTouchEnabled(false);
            
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+200, m_pBowl->getPositionY()+250));
            pIngredient->runAction(Sequence::create(RotateBy::create(0.3, -120),
                                                CallFunc::create([=](){
                AudioHelp::getInstance()->playPourPowderEffect();
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/sugar.plist");
                this->addChildToContentLayer(pParticle);
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                pParticle->setLocalZOrder(6);
            }),
                                                JumpBy::create(1.5, Vec2::ZERO, 20, 5),
                                                Spawn::create(RotateTo::create(0.3, 0),
                                                              MoveBy::create(0.3, Vec2(0, 100)), NULL),
                                                MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                CallFunc::create([=]{
                m_nAddCount++;
                if (m_nAddCount==9) {
                    _finishAdd(2);
                }
                
                m_bReadyAdd = true;
                pIngredient->removeFromParent();
            }),
                                                NULL));
        }
            break;
#pragma mark === add oil ===
        case IngredientTag::eTagOil:
        {
            lNameStr = "twist/make1/bowl_oil.png";
            pIngredient->setTouchEnabled(false);
            pIngredient->removeFromParent();
            
            Sprite* pBottle = Sprite::create(localPath("twist/make1/oil2.png"));
            pBottle->setPosition(Vec2(m_pBowl->getPositionX()+200, m_pBowl->getPositionY()+100));
            this->addChildToContentLayer(pBottle);
            
            Sprite* pOilStream = Sprite::create(localPath("twist/make1/oil2_1.png"));
            this->addChildToContentLayer(pOilStream);
            pOilStream->setPosition(pBottle->getPosition());
            pOilStream->setLocalZOrder(10);
            
            pOilStream->runAction(Repeat::create(AnimationHelp::createAnimate(localPath("twist/make1/oil2_"), 1, 2), 10));
            AudioHelp::getInstance()->playPourWaterEffect();
            pBottle->runAction(Sequence::create(DelayTime::create(2),CallFunc::create([=](){
                pOilStream->removeFromParent();
                AudioHelp::getInstance()->stopPourWaterEffect();
                
                m_nAddCount++;
                if (m_nAddCount==9) {
                    _finishAdd(2);
                }
                
                m_bReadyAdd = true;
            }),
                                                MoveBy::create(0.5, Vec2(visibleSize.width,0)),
                                                
                                                NULL));
        }
            break;
            
        default:
            break;
    }
    Sprite* pAdd = Sprite::create(localPath(lNameStr));
    pAdd->setPosition(posAdd);
    if (pAdd) {
        pAdd->setOpacity(0);
        m_pMixture->addChild(pAdd);
        pAdd->runAction(Sequence::create(DelayTime::create(dt),
                                         FadeIn::create(0.5),
                                         CallFunc::create([=](){
        }), NULL));
    }
}

void TwistRiseDoughScene::_mixBatter()
{
    if (m_bWhriskIsReady){
        m_nMixCount++;
        
        if (m_nMixCount%80==0) {
            int mixtureStep = m_nMixCount/80;
            int mixTotal = 10;
            if (mixtureStep<=mixTotal){
                std::stringstream ostr;
                ostr<<"twist/make1/stir"<<mixtureStep<<".png";
                Sprite* pMixture = Sprite::create(localPath(ostr.str()));
                m_pMixture->removeAllChildren();
                m_pMixture->addChild(pMixture);
                if (mixtureStep==8) {
                    AudioHelp::getInstance()->stopLoopEffect();
                    AudioHelp::getInstance()->playLoopEffect("stir_machine.mp3");
                }
            }
            if (mixtureStep>=mixTotal){
                //mix finish
                m_pMixture->stopAllActions();
                Sprite *stick = (Sprite*)m_pWhrisk->getChildByName("stick");
                if (stick) {
                    stick->stopAllActions();
                    stick->setVisible(false);
                }
                m_pWhrisk->setDragSprite(Sprite::create(localPath("donut/make1/blender.png")));
                _finishMix();
                m_nMixCount = 0;
                AudioHelp::getInstance()->stopLoopEffect();
            }
        }
    }else{
        Sprite *pShadow = (Sprite*)m_pWhrisk->getChildByName("shadow");
        if (pShadow){
            pShadow->removeFromParent();
        }
        
        m_bWhriskIsReady = true;
        AudioHelp::getInstance()->playLoopEffect("machine_stir.mp3");
        m_pWhrisk->setDragSprite(Sprite::create(localPath("donut/make1/blender_0.png")));
        m_pWhrisk->setLocalZOrder(5);
        
        Sprite *stick = (Sprite*)m_pWhrisk->getChildByName("stick");
        stick->setVisible(true);
        
        m_pWhrisk->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.01, Vec2(0.1, 1)),
                                                                    MoveBy::create(0.01, Vec2(-0.2, -2)),
                                                                    MoveBy::create(0.01, Vec2(0.1, 1)),
                                                                    NULL)));
        Rect rect = m_pBowl->getBoundingBox();
        Rect limitRect = Rect(rect.origin.x+190+40, rect.origin.y+250+40, rect.size.width-200-80, rect.size.height*0.4-40);
        m_pWhrisk->setLimitRect(limitRect);
        m_pWhrisk->setOrgPositionDefault();
        m_pWhrisk->setTouchEnabled(false);
        m_pWhrisk->setPosition(Vec2(m_pBowl->getPositionX()+200, m_pBowl->getPositionY()+100));
        m_pMixture->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.3),
                                                                     CallFunc::create([=](){
            m_pMixture->setScaleX(-m_pMixture->getScaleX());
        }), NULL)));
        m_pWhrisk->runAction(Sequence::create(MoveTo::create(0.5, Vec2(visibleSize.width*0.5,visibleSize.height*0.5+100)),
                                              CallFunc::create([=](){
            if (stick){
                stick->runAction(RepeatForever::create(AnimationHelp::createAnimate(localPath("donut/make1/blender_"), 1, 3)));
            }
            
            Rect rect = m_pBowl->getBoundingBox();
            Rect limitRect = Rect(rect.origin.x+190, rect.origin.y+250, rect.size.width-200, rect.size.height*0.4);
            m_pWhrisk->setLimitRect(limitRect);
            m_pWhrisk->setTouchEnabled(true);
        }), NULL));
        m_pBowl->runAction(MoveBy::create(0.5, Vec2(110, 0)));
        m_pBowlFront->runAction(MoveBy::create(0.5, Vec2(110, 0)));
        m_pMixtureClipping->runAction(MoveBy::create(0.5, Vec2(110, 0)));
    }
}

void TwistRiseDoughScene::_riseDough()
{
    Sprite* pWrap = Sprite::create(localPath("twist/make1/plastic_wrap1.png"));
    this->addChildToContentLayer(pWrap);
    pWrap->setPosition(m_pBowl->getPosition()+visibleSize);
    pWrap->setScale(0.2);
    pWrap->setLocalZOrder(10);
    
    pWrap->runAction(Sequence::create(Spawn::create(ScaleTo::create(1, 1),
                                                    MoveBy::create(1, Vec2(-visibleSize.width, -visibleSize.height)), NULL),
                                      CallFunc::create([=](){
        m_pMixture->runAction(Sequence::create(DelayTime::create(0.5),
                                               ScaleTo::create(2, 1.3*m_pMixture->getScaleX(),1.3*m_pMixture->getScaleY()),
                                               CallFunc::create([=](){
            pWrap->setTexture(localPath("twist/make1/plastic_wrap2.png"));
            
            AudioHelp::getInstance()->playEffect("done.mp3");
            AudioHelp::getInstance()->playEffect(((arc4random()%100)>50?"vo_nice_work.mp3":"vo_well_done.mp3"));
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
            this->addChildToUILayer(pParticle);
            pParticle->setLocalZOrder(10);
            pParticle->setPosition(visibleSize*0.5);
            
            m_pGameUI->showNextLayout();
        }), NULL));
    }), NULL));
}

void TwistRiseDoughScene::_playEffectNice(float)
{
    static int count = 0;
    count++;
    if (count==6) {
        int radom = arc4random()%100;
        if (radom>50){
            AudioHelp::getInstance()->playEffect("nice.mp3");
        }else {
            AudioHelp::getInstance()->playEffect("fantastic.mp3");
        }
    }
}


#pragma mark ======== finish ========================
void TwistRiseDoughScene::_finishAdd(int index)
{
    switch (index) {
        case 0:
        {
            _showMixIngredient(1);
            this->runAction(Sequence::create(DelayTime::create(1),
                                             CallFunc::create([=]{
                _moveDownMixIngredient(IngredientTag::eTagWater);
                _moveDownMixIngredient(IngredientTag::eTagMilk);
                _moveDownMixIngredient(IngredientTag::eTagEgg);
            }), NULL));
        }
            break;
        case 1:
        {
            _showMixIngredient(2);
            this->runAction(Sequence::create(DelayTime::create(1),
                                             CallFunc::create([=]{
                _moveDownMixIngredient(IngredientTag::eTagButter);
                _moveDownMixIngredient(IngredientTag::eTagSalt);
                _moveDownMixIngredient(IngredientTag::eTagOil);
            }), NULL));
        }
            break;
        case 2:
        {
            _showWhisk();
        }
            break;
        case 3:
        {
            _showWhisk();
        }
            break;
            
        default:
            break;
    }
}

void TwistRiseDoughScene::_finishMix()
{
    _removeWhisk();
    //    210
    if (0==m_nStep) {
    }
    _riseDough();
    m_nStep++;
}
