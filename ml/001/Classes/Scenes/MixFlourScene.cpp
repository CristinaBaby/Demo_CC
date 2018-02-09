
#include "MixFlourScene.h"
#include "SceneManager.h"

MixFlourScene::MixFlourScene()
{
    m_pBowl = nullptr;
    m_pWhrisk = nullptr;
    m_nStep = 0;
    m_nAddCount = 0;
    m_nMixCount = 0;
    m_bWhriskIsReady = false;
    m_bReadyAdd = true;
    m_nRollCount = 0;
    m_bRollToolReady = false;
    m_pDoughGrid = nullptr;
    m_bOilReady = false;
    m_bSauceReady = false;
    m_nOilPercent = 100;
    m_nSaucePercent = 100;
    m_bGraterReady = false;
    m_nCheeseCount = 0;
    m_bCanTouch = false;
    m_pDecoration = nullptr;
    m_pDecorationRipe = nullptr;
    m_pDecorationScrollView = nullptr;
//    AudioHelp::getInstance()->registerEffectScene(ClassString(MixFlourScene));
}

MixFlourScene::~MixFlourScene()
{
    CC_SAFE_RELEASE(m_pDecorationRipe);
//    AudioHelp::getInstance()->removeEffectScene(ClassString(MixFlourScene));
}
bool MixFlourScene::init()
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

void MixFlourScene::onEnter()
{
    ExtensionScene::onEnter();
    Analytics::getInstance()->sendScreenEvent(Flurry_EVENT_MAKE_DONUT);
}

void MixFlourScene::onExit()
{
    ExtensionScene::onExit();
}

void MixFlourScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    m_pDecorationScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
    m_pDecorationScrollView->reloadData();
}

void MixFlourScene::onButtonCallback(Button* btn)
{
//    int tag = btn->getTag();
//    if (GameUILayoutLayer::eUIButtonTagNext==tag){
//        btn->setTouchEnabled(false);
//        m_pGameUI->hideNext();
//        return;
//    }
    ExtensionScene::onButtonCallback(btn);
}

void MixFlourScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    ContainerNode* pContainer = (ContainerNode*)pDragNode->getUserData();
    if (pContainer) {
        pContainer->showShadow(false);
    }
    pDragNode->setScale(1);
    int tag = pDragNode->getTag();
    if (IngredientTag::eTagEgg0 == tag) {
        m_pEgg0_0->setVisible(false);
        pDragNode->setVisible(true);
    }else if (IngredientTag::eTagEgg1 == tag){
        m_pEgg1_0->setVisible(false);
        pDragNode->setVisible(true);
    }
    if (pDragNode==m_pSholve) {
        pDragNode->setLocalZOrder(-2);
    }else{
        pDragNode->setLocalZOrder(50);
    }
    if (IngredientTag::eTagWhrisk==tag) {
        
        if (m_bWhriskIsReady){
            Sprite *stick = (Sprite*)pDragNode->getChildByName("stick");
            
            if (stick){
                stick->runAction(RepeatForever::create(AnimationHelp::createAnimate(localPath("make1/blender_"), 1, 3)));
            }
            
            if (m_nStep==2) {
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
    }else if (m_pRollTool==pDragNode) {
        if (m_bRollToolReady) {
            m_pDoughGrid->runAction(RepeatForever::create(Waves::create(3, Size(10, 10), 1, 5, true, true)));
        }
    }else if (m_pDragDough==pDragNode) {
        m_pDragDough->runAction(EaseBackOut::create(ScaleTo::create(0.3, 0.72)));
    }else if (m_pOilBrush==pDragNode) {
        if (m_bOilReady){
            m_pOilScribble->paint(worldPoint);
        }
    }else if (m_pSauceSpoon==pDragNode) {
        if (m_bSauceReady){
            m_pSauceScribble->paint(worldPoint);
        }
    }
    
}

void MixFlourScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    if (m_pWhrisk==pDragNode){
        Rect rect = m_pBowl->getBoundingBox();
        rect.origin = m_pBowl->getParent()->convertToWorldSpace(rect.origin);
        if (rect.containsPoint(worldPoint)){
            _mixBatter();
        }
        
    }else if (m_pRollTool == pDragNode) {
        Rect rect = m_pDough->getBoundingBox();
        rect.origin = m_pDough->getParent()->convertToWorldSpace(rect.origin);
        if (rect.containsPoint(worldPoint)){
            _rollDough();
        }
    }
}

void MixFlourScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    AudioHelp::getInstance()->stopLoopEffect();
    pDragNode->stopAllActions();
 
    if (m_pDoughGrid) {
        m_pDoughGrid->stopAllActions();
    }
    if (m_pRollTool==pDragNode) {
        return;
    }
    if (pDragNode==m_pWhrisk) {
        Sprite *stick = (Sprite*)pDragNode->getChildByName("stick");
        if (stick) {
            stick->stopAllActions();
        }
        m_pWhrisk->stopAllActions();
        m_pMixture->stopAllActions();
        m_pMixture->setRotation(0);
    }else if (pDragNode==m_pDragDough) {
        Rect rect = m_pPan->getBoundingBox();
        rect.origin = m_pPan->getParent()->convertToWorldSpace(rect.origin);
        if (rect.containsPoint(worldPoint)) {
            _addDoughPan();
        }
    }else{
        Rect rect = m_pBowl->getBoundingBox();
        rect.size = rect.size+Size(200, 180);
        rect.origin = rect.origin-Vec2(-80, -80);
        if (rect.containsPoint(pDragNode->getPosition()) && m_bReadyAdd) {
            pDragNode->setTouchEnabled(false);
            m_bReadyAdd = false;
            _addIngredient(pDragNode);
        }else{
            _back(pDragNode);
        }
    }
}

void MixFlourScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->stopAllActions();
    
    if (m_pDoughGrid) {
        m_pDoughGrid->stopAllActions();
    }else if (m_pDragDough==pDragNode) {
        m_pDragDough->runAction(EaseBackOut::create(ScaleTo::create(0.3, 0.7)));
    }else{
        _back(pDragNode);
    }
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
void MixFlourScene::TouchBegan(Touch *pTouch, Event *pEvent)
{
    
}
void MixFlourScene::TouchEnded(Touch *pTouch, Event *pEvent)
{
    if (!m_bCanTouch) {
        return;
    }
    if (m_nIndex<0) {
        return;
    }
    Vec2 pos = pTouch->getLocation();
    std::stringstream ostr;
    ostr<<"content/category/beforbake/"<<m_sCurTypePath<<"/"<<m_nIndex<<".png";
    Sprite* unit = Sprite::create(ostr.str());
    
    int radius = 230-MIN(unit->getContentSize().width/2, unit->getContentSize().height/2);
    Vec2 offset = pos-m_pPan->getPosition()-Vec2(0, 10);
    if (powf(offset.x, 2)+powf(offset.y, 2)>powf(radius, 2)) {
        return;
    }
    if (std::strcmp(m_sCurTypePath.c_str(), "meat")==0 || std::strcmp(m_sCurTypePath.c_str(), "fruit")==0 || std::strcmp(m_sCurTypePath.c_str(), "vegetable")==0){
        m_pGuideLayer->removeGuide();
        int angle = 20*(arc4random()%10);
        unit->setRotation(angle);
        unit->setPosition(m_pDecoration->convertToNodeSpace(pos));
        m_pDecoration->addChild(unit);
        
        std::stringstream ostrBaked;
        ostrBaked<<"content/category/afterbake/"<<m_sCurTypePath<<"/"<<m_nIndex<<".png";
        Sprite* pBakedUnit = Sprite::create(ostrBaked.str());
        pBakedUnit->setRotation(angle);
        pBakedUnit->setPosition(unit->getPosition());
        m_pDecorationRipe->addChild(pBakedUnit);
        
        AudioHelp::getInstance()->playSelectedEffect();
        _playEffectNice();
    }
    
}

#pragma mark - initData
void MixFlourScene::_initData()
{
    setExPath("content/make/donut/");
    
    m_nNextSceneTag = GameUIEvent::eSceneTagCutDonut;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagCutDonut;
}

#pragma mark ==== Mix UI =====
void MixFlourScene::_createMixUI()
{
    m_pBowl = Sprite::create(localPath("make1/bowl_down.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 210+110-visibleSize.width/2, 350);
    
    m_pBowlFront = Sprite::create(localPath("make1/bowl_on.png"));
    this->addChildToContentLayer(m_pBowlFront);
    CMVisibleRect::setPositionAdapted(m_pBowlFront, 210+110-visibleSize.width/2, 350);
    
    m_pMixture = Node::create();
    this->addChildToContentLayer(m_pMixture);
    CMVisibleRect::setPositionAdapted(m_pMixture, 210+110, 350);
    m_pMixture->setLocalZOrder(5);
    
    m_pBowl->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
    m_pBowlFront->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
    m_pBowlFront->setLocalZOrder(8);
    
//    AudioHelp::getInstance()->playEffect("vo_add_ingredients.mp3");
}

void MixFlourScene::_showMixIngredient(int index)
{
    AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
    if (index==0) {
        m_pEgg0_0 = Sprite::create(localPath("make1/egg2.png"));
        this->addChildToContentLayer(m_pEgg0_0);
        CMVisibleRect::setPositionAdapted(m_pEgg0_0, 310+visibleSize.width, 720);
        
        m_pEgg1_0 = Sprite::create(localPath("make1/egg1.png"));
        this->addChildToContentLayer(m_pEgg1_0);
        CMVisibleRect::setPositionAdapted(m_pEgg1_0, 310+visibleSize.width, 720);
        
        m_pEgg0 = _createDrageNode(localPath("make1/egg_1.png"),false);
        this->addChildToContentLayer(m_pEgg0);
        CMVisibleRect::setPositionAdapted(m_pEgg0, 310-20+visibleSize.width, 720);
        m_pEgg0->setTag(IngredientTag::eTagEgg0);
        m_pEgg0->setContentSize(Size(200,170));
        
        m_pEgg1 = _createDrageNode(localPath("make1/egg_1.png"),false);
        this->addChildToContentLayer(m_pEgg1);
        CMVisibleRect::setPositionAdapted(m_pEgg1, 310+20+visibleSize.width, 730);
        m_pEgg1->setTag(IngredientTag::eTagEgg1);
        m_pEgg1->setContentSize(Size(200,170));
        
        m_pSugar = _createDrageNode(localPath("make1/sugar_down.png"),false);
        this->addChildToContentLayer(m_pSugar);
        CMVisibleRect::setPositionAdapted(m_pSugar, 440+visibleSize.width, 730);
        m_pSugar->setActionPoint(Vec2(7, 90));
        m_pSugar->setTag(IngredientTag::eTagSugar);
//
        m_pWater = _createDrageNode(localPath("make1/kettle1_down.png"),false);
        this->addChildToContentLayer(m_pWater);
        CMVisibleRect::setPositionAdapted(m_pWater, 590+visibleSize.width, 760);
        m_pWater->setActionPoint(Vec2(28, 231));
        m_pWater->setTag(IngredientTag::eTagWater);
//
        ContainerNode* pSugar = ContainerNode::create(m_pSugar);
        pSugar->addShadow(localPath("make1/sugar_shadow.png"));
        pSugar->addFront(localPath("make1/sugar_on.png"));
        pSugar->addFood(localPath("make1/sugar.png"),localPath("make1/sugar_down.png"));
        m_pSugar->setUserData(pSugar);
//
        KettleNode* pKettle = KettleNode::create(m_pWater);
        pKettle->addShadow(localPath("make1/kettle_shadow.png"));
        pKettle->addWater(localPath("make1/kettle1_water.png"), localPath("make1/kettle_mask.png"));
        pKettle->addFront(localPath("make1/kettle1_on.png"));
        pKettle->addPourWater(localPath("make1/kettle2_waiter2.png"));
        m_pWater->setUserData(pKettle);
        
        m_pEgg0->setVisible(false);
        m_pEgg1->setVisible(false);
        m_pEgg0_0->setScale(0.6);
        m_pEgg1_0->setScale(0.6);
        m_pEgg0->setScale(0.6);
        m_pEgg1->setScale(0.6);
        m_pSugar->setScale(0.6);
        m_pWater->setScale(0.6);
        auto action = Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                       CallFuncN::create([=](Node* pNode){
            DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
            if (pIngredient) {
                pIngredient->setOrgPositionDefault();
                pIngredient->setTouchEnabled(true);
            }
        }), NULL);
        
        m_pEgg0->runAction(action);
        m_pEgg1->runAction(action->clone());
        m_pEgg0_0->runAction(action->clone());
        m_pEgg1_0->runAction(action->clone());
        m_pSugar->runAction(action->clone());
        m_pWater->runAction(action->clone());
    }else if (index==1){
        m_pButter = _createDrageNode(localPath("make1/butter.png"));
        this->addChildToContentLayer(m_pButter);
        CMVisibleRect::setPositionAdapted(m_pButter, 350+visibleSize.width, 720);
        m_pButter->setTag(IngredientTag::eTagButter);
        
        m_pMilk = _createDrageNode(localPath("make1/milk_1.png"),false);
        this->addChildToContentLayer(m_pMilk);
        CMVisibleRect::setPositionAdapted(m_pMilk, 530+visibleSize.width, 750);
        m_pMilk->setActionPoint(Vec2(12, 144));
        m_pMilk->setTag(IngredientTag::eTagMilk);
        
        ContainerNode* pButter = ContainerNode::create(m_pButter);
        pButter->addShadow(localPath("make1/butter_shadow.png"));
        pButter->addFood(localPath("make1/butter_1.png"));
        m_pButter->setUserData(pButter);
        
        KettleNode* pKettle = KettleNode::create(m_pMilk);
        pKettle->addShadow(localPath("make1/milk_shadow.png"));
        pKettle->addWater(localPath("make1/milk_2.png"), localPath("make1/milk_2.png"));
        m_pMilk->setUserData(pKettle);
        
        m_pButter->setScale(0.6);
        m_pMilk->setScale(0.6);
        auto action = Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                       CallFuncN::create([=](Node* pNode){
            DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
            if (pIngredient) {
                pIngredient->setOrgPositionDefault();
                pIngredient->setTouchEnabled(true);
            }
        }), NULL);
        
        m_pButter->runAction(action);
        m_pMilk->runAction(action->clone());
    }else if (index==2) {
        m_pFlour = _createDrageNode(localPath("make1/flour2.png"),false);
        this->addChildToContentLayer(m_pFlour);
        CMVisibleRect::setPositionAdapted(m_pFlour, 600+visibleSize.width, 770);
        m_pFlour->setActionPoint(Vec2(62, 277));
        m_pFlour->setTag(IngredientTag::eTagFlour);
        
        m_pSalt = _createDrageNode(localPath("make1/salt.png"));
        this->addChildToContentLayer(m_pSalt);
        CMVisibleRect::setPositionAdapted(m_pSalt, 310+visibleSize.width, 750);
        m_pSalt->setTag(IngredientTag::eTagSalt);
        m_pSalt->setActionPoint(Vec2(69, 168));
        
        m_pYeast = _createDrageNode(localPath("make1/baking_powder1.png"),false);
        this->addChildToContentLayer(m_pYeast);
        CMVisibleRect::setPositionAdapted(m_pYeast, 450+visibleSize.width, 760);
        m_pYeast->setActionPoint(Vec2(5, 186));
        m_pYeast->setTag(IngredientTag::eTagYeast);
        
        
        ContainerNode* pSalt = ContainerNode::create(m_pSalt);
        pSalt->addShadow(localPath("make1/salt_shadow.png"));
        m_pSalt->setUserData(pSalt);
        
        
        ContainerNode* pFlour = ContainerNode::create(m_pFlour);
        pFlour->addShadow(localPath("make1/flour2_shadow.png"));
        m_pFlour->setUserData(pFlour);
        
        ContainerNode* pPowder = ContainerNode::create(m_pYeast);
        pPowder->addShadow(localPath("make1/baking_powder3.png"));
        pPowder->addFood(localPath("make1/baking_powder2.png"));
        m_pYeast->setUserData(pPowder);
        
        m_pFlour->setScale(0.6);
        m_pSalt->setScale(0.6);
        m_pYeast->setScale(0.6);
        auto action = Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                       CallFuncN::create([=](Node* pNode){
            DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
            if (pIngredient) {
                pIngredient->setOrgPositionDefault();
                pIngredient->setTouchEnabled(true);
            }
        }), NULL);
        m_pFlour->runAction(action);
        m_pYeast->runAction(action->clone());
        m_pSalt->runAction(action->clone());
    }
}

void MixFlourScene::_moveDownMixIngredient(int tag)
{
//    不要这个效果了
    return;
    DragNode*pNode = dynamic_cast<DragNode*>(m_pContentLayer->getChildByTag(tag));
    if (!pNode) {
        return;
    }
    int index = tag - IngredientTag::eTagBegin-1;
    static Vec2 sEndPos[]={
        Vec2(510, 308),//egg
        Vec2(510, 308),
        Vec2(510, 450), //sugar
        Vec2(530, 395), //water
        Vec2(520, 280), //butter
        Vec2(530, 450), //milk
        Vec2(450, 300), //salt
        Vec2(540, 390), //powder
        Vec2(470, 570), //flour
    
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
        case IngredientTag::eTagEgg0:
        {
            m_pEgg0_0->runAction(action);
            m_pEgg0->runAction(action->clone());
            m_pEgg0_0->setLocalZOrder(IngredientTag::eTagEnd-m_pEgg0_0->getTag());
            m_pEgg0->setLocalZOrder(IngredientTag::eTagEnd-m_pEgg0->getTag());
        }
            break;
        case IngredientTag::eTagEgg1:
        {
            m_pEgg1_0->runAction(action);
            m_pEgg1->runAction(action->clone());
            m_pEgg1_0->setLocalZOrder(IngredientTag::eTagEnd-m_pEgg1_0->getTag());
            m_pEgg1->setLocalZOrder(IngredientTag::eTagEnd-m_pEgg1->getTag());
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
void MixFlourScene::_showWhisk()
{
    AudioHelp::getInstance()->playEffect("vo_mix_ingredients.mp3");
    m_pBowl->runAction(MoveBy::create(0.5, Vec2(-110, 0)));
    m_pBowlFront->runAction(MoveBy::create(0.5, Vec2(-110, 0)));
    m_pMixture->runAction(MoveBy::create(0.5, Vec2(-110, 0)));
    m_bWhriskIsReady = false;
    m_pWhrisk = _createDrageNode(localPath("make1/blender.png"));
    this->addChildToContentLayer(m_pWhrisk);
    CMVisibleRect::setPositionAdapted(m_pWhrisk, 550+visibleSize.width, 360);
    m_pWhrisk->setActionPoint(Vec2(45, 60));
    m_pWhrisk->setLocalZOrder(10);
    m_pWhrisk->setTag(IngredientTag::eTagWhrisk);
    
    Sprite* pSprite = Sprite::create(localPath("make1/blender_1.png"));
    m_pWhrisk->addChild(pSprite,-1);
    pSprite->setName("stick");
    pSprite->setVisible(false);
    
    Sprite* pShadow = Sprite::create(localPath("make1/blender_shadow.png"));
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

void MixFlourScene::_removeWhisk()
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

#pragma mark ==== roll UI =====
void MixFlourScene::_createRollUI()
{
    m_pFlourSprinkle = Sprite::create(localPath("make1/powder.png"));
    this->addChildToContentLayer(m_pFlourSprinkle);
    CMVisibleRect::setPositionAdapted(m_pFlourSprinkle, 480, 320);
    
    m_pDoughGrid = NodeGrid::create();
    this->addChildToContentLayer(m_pDoughGrid);
    CMVisibleRect::setPositionAdapted(m_pDoughGrid, 480, 320);
    
    m_pDough = Sprite::create(localPath("make1/dough0.png"));
    m_pDoughGrid->addChild(m_pDough);
    
    m_pDoughPan = Sprite::create(localPath("make1/dough2.png"));
    m_pDoughGrid->addChild(m_pDoughPan);
}

void MixFlourScene::_pourDough()
{
    m_pBowl->setLocalZOrder(5);
    m_pMixture->setLocalZOrder(6);
    m_pDough->setLocalZOrder(7);
    m_pBowl->runAction(Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width-100, 50)),
                                        RotateBy::create(0.3, Vec3(0, 40, 0)),
                                        DelayTime::create(0.3),
                                        MoveBy::create(0.3, Vec2(-200, 0)),
                                        RotateBy::create(0.3, Vec3(0, -40, 0)),
                                        MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                        CallFunc::create([=](){
        m_pBowl->removeFromParent();
        m_pBowl = nullptr;
    }), NULL));
    m_pMixture->runAction(Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width-100, 50)),
                                           RotateBy::create(0.3, Vec3(0, 40, 0)),
                                           MoveBy::create(0.5, Vec2(50, -30)),
                                           CallFunc::create([=](){
        m_pDough->setOpacity(255);
        m_pMixture->removeFromParent();
        m_pMixture = nullptr;
    }), NULL));
}

void MixFlourScene::_showSpreadFlour()
{
    Sprite* pFlourBag = Sprite::create(localPath("make1/flour_h.png"));
    this->addChildToContentLayer(pFlourBag);
    pFlourBag->setLocalZOrder(10);
    CMVisibleRect::setPositionAdapted(pFlourBag, 480+visibleSize.width, 320);
    ccBezierConfig cfg;
    cfg.controlPoint_1 = CMVisibleRect::getPosition(200, 200);
    cfg.controlPoint_2 = CMVisibleRect::getPosition(900, 400);
    cfg.endPosition = CMVisibleRect::getPosition(800, 900);
    
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/flour2.plist");
    pFlourBag->addChild(pParticle);
    pParticle->setPosition(Vec2(62, 277));
    pParticle->setVisible(false);
    
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
        Sprite* pFlour = Sprite::create(localPath("make1/powder0.png"));
        this->addChildToContentLayer(pFlour);
        pFlour->setLocalZOrder(5);
        Vec2 pos = Vec2(200.0+10.0*(arc4random()%50), 100.0+10.0*(arc4random()%50));
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

void MixFlourScene::_showRollTool()
{
    AudioHelp::getInstance()->playEffect("vo_roll_dough.mp3");
    m_pRollTool = _createDrageNode(localPath("make1/rolling.png"));
    this->addChildToContentLayer(m_pRollTool);
    m_pRollTool->setLocalZOrder(10);
    CMVisibleRect::setPositionAdapted(m_pRollTool, 480+visibleSize.width, 320);
    m_pRollTool->setTouchEnabled(false);
    m_pRollTool->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                            CallFunc::create([=](){
        m_pRollTool->setOrgPositionDefault();
        m_pRollTool->setTouchEnabled(true);
    }), NULL));
}

void MixFlourScene::_removeRollTool()
{
    
}

#pragma mark ======== deel ========================
void MixFlourScene::_back(DragNode* pIngredient)
{
    pIngredient->setTouchEnabled(false);
    pIngredient->back(0,[=](){
        ContainerNode* pContainer = (ContainerNode*)pIngredient->getUserData();
        if (pContainer) {
            pContainer->showShadow();
        }
        int tag = pIngredient->getTag();
        if (IngredientTag::eTagEgg0 == tag) {
            m_pEgg0_0->setVisible(true);
            pIngredient->setVisible(false);
        }else if (IngredientTag::eTagEgg1 == tag){
            m_pEgg1_0->setVisible(true);
            pIngredient->setVisible(false);
        }
        pIngredient->setTouchEnabled(true);
    });
    pIngredient->runAction(ScaleTo::create(0.5, 0.6));
}

void MixFlourScene::_addIngredient(DragNode* pIngredient)
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
            lNameStr = "make1/bowl_sugar.png";
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
                if (m_nAddCount==4) {
                    _finishAdd(1);
                }
                
                m_bReadyAdd = true;
                pIngredient->removeFromParent();
            }),
                                                    NULL));
        }
            break;
#pragma mark === add Egg ===
        case IngredientTag::eTagEgg0:
        {
            lNameStr = "make1/egg_4.png";
            posAdd = Vec2(50,10);
            pIngredient->setTouchEnabled(false);
            pIngredient->removeFromParent();
            Sprite* pEgg = Sprite::create(localPath("make1/egg_1.png"));
            this->addChildToContentLayer(pEgg);
            pEgg->setPosition(m_pBowl->getPosition()+Vec2(50, 150));
            AudioHelp::getInstance()->playAddEggEffect();
            pEgg->runAction(Sequence::create(AnimationHelp::createAnimate(localPath("make1/egg_"), 1, 2,false),
                                             CallFunc::create([=](){
                Sprite* pEgg2 = Sprite::create(localPath("make1/egg_3.png"));
                this->addChildToContentLayer(pEgg2);
                pEgg2->setLocalZOrder(6);
                pEgg2->setPosition(m_pBowl->getPosition()+Vec2(50, 120));
                pEgg2->runAction(Sequence::create(DelayTime::create(1),
                                                  CallFunc::create([=](){
                    m_nAddCount++;
                    if (m_nAddCount==4) {
                        _finishAdd(1);
                    }
                    m_bReadyAdd = true;
                    pEgg2->removeFromParent();
                }), NULL));
                pEgg->removeFromParent();
            }), NULL));
        }
            break;
        case IngredientTag::eTagEgg1:
        {
            lNameStr = "make1/egg_4.png";
            pIngredient->setTouchEnabled(false);
            pIngredient->removeFromParent();
            Sprite* pEgg = Sprite::create(localPath("make1/egg_1.png"));
            this->addChildToContentLayer(pEgg);
            pEgg->setPosition(m_pBowl->getPosition()+Vec2(-20, 150));
            AudioHelp::getInstance()->playAddEggEffect();
            pEgg->runAction(Sequence::create(AnimationHelp::createAnimate(localPath("make1/egg_"), 1, 2,false),
                                             CallFunc::create([=](){
                Sprite* pEgg2 = Sprite::create(localPath("make1/egg_3.png"));
                this->addChildToContentLayer(pEgg2);
                pEgg2->setLocalZOrder(6);
                pEgg2->setPosition(m_pBowl->getPosition()+Vec2(0, 120));
                pEgg2->runAction(Sequence::create(DelayTime::create(1),
                                                  CallFunc::create([=](){
                    m_nAddCount++;
                    if (m_nAddCount==4) {
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
            lNameStr = "make1/bowl_water.png";
            pIngredient->setTouchEnabled(false);
            
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
                if (m_nAddCount==4) {
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
            lNameStr = "make1/bowl_butter.png";
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
                    Sprite* pButter = Sprite::create(localPath("make1/butter_2.png"));
                    this->addChildToContentLayer(pButter);
                    pButter->setPosition(pIngredient->getPosition());
                    pButter->setLocalZOrder(10);
                    pButter->setRotation(i*20);
                    pButter->runAction(Sequence::create(DelayTime::create(0.1+0.1*i),
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
                if (m_nAddCount==6) {
                    _finishAdd(2);
                }
                m_bReadyAdd = true;
            }),NULL));
        }
            break;
#pragma mark === add Milk ===
        case IngredientTag::eTagMilk:
        {
            lNameStr = "make1/bowl_milk.png";
            pIngredient->setTouchEnabled(false);
            
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+70, m_pBowl->getPositionY()+300));
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
                    _finishAdd(2);
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
            lNameStr = "make1/bowl_flour.png";
            pIngredient->setTouchEnabled(false);
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+180, m_pBowl->getPositionY()+300));
            pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -80),
                                                 CallFunc::create([=](){
                AudioHelp::getInstance()->playPourPowderEffect();
                
                Sprite* pBag = Sprite::create(localPath("make1/flour1_down.png"));
                this->addChildToContentLayer(pBag);
                pBag->setLocalZOrder(8);
                
                Sprite* pBagFront = Sprite::create(localPath("make1/flour1_on.png"));
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
                    if (m_nAddCount==9) {
                        _finishAdd(3);
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
            lNameStr = "make1/bowl_baking_powder.png";
            pIngredient->setTouchEnabled(false);
            
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+130, m_pBowl->getPositionY()+250));
            pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -100),
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
                                                 ScaleTo::create(0.5, 1.1,0.9),
                                                 DelayTime::create(1),
                                                 ScaleTo::create(0.5, 1),
                                                 RotateTo::create(0.3, 0),
                                                 MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                 CallFunc::create([=](){
                m_nAddCount++;
                if (m_nAddCount==9) {
                    _finishAdd(3);
                }
                m_bReadyAdd = true;
                pIngredient->removeFromParent();
            }),
                                                 NULL));
        }
            break;
#pragma mark === add Salt ===
        case IngredientTag::eTagSalt:
        {
            lNameStr = "make1/bowl_slat.png";
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
                    _finishAdd(3);
                }
                
                m_bReadyAdd = true;
                pIngredient->removeFromParent();
            }),
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

void MixFlourScene::_mixBatter()
{
    if (m_bWhriskIsReady){
        m_nMixCount++;
        
        if (m_nMixCount%80==0) {
            int mixtureStep = m_nMixCount/80;
            int mixTotal = 4;
            if (2==m_nStep) {
                mixTotal = 5;
            }
            if (mixtureStep<=mixTotal){
                std::stringstream ostr;
                ostr<<"make1/stir"<<m_nStep+1<<"_"<<mixtureStep<<".png";
                Sprite* pMixture = Sprite::create(localPath(ostr.str()));
                m_pMixture->removeAllChildren();
                m_pMixture->addChild(pMixture);
            }
            if (mixtureStep>=mixTotal){
                //mix finish
                m_pMixture->stopAllActions();
                Sprite *stick = (Sprite*)m_pWhrisk->getChildByName("stick");
                if (stick) {
                    stick->stopAllActions();
                    stick->setVisible(false);
                }
                m_pWhrisk->setDragSprite(Sprite::create(localPath("make1/blender.png")));
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
        if (m_nStep==2) {
            AudioHelp::getInstance()->playLoopEffect("stir_machine.mp3");
        }else{
            AudioHelp::getInstance()->playLoopEffect("machine_stir.mp3");
        }
        m_pWhrisk->setDragSprite(Sprite::create(localPath("make1/blender_0.png")));
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
                stick->runAction(RepeatForever::create(AnimationHelp::createAnimate(localPath("make1/blender_"), 1, 3)));
            }
            Rect rect = m_pBowl->getBoundingBox();
            Rect limitRect = Rect(rect.origin.x+190, rect.origin.y+250, rect.size.width-200, rect.size.height*0.4);
            m_pWhrisk->setLimitRect(limitRect);
            m_pWhrisk->setTouchEnabled(true);
            
            m_pWhrisk->setTouchEnabled(true);
        }), NULL));
//        m_pBowl->runAction(MoveBy::create(0.5, Vec2(260, 0)));
//        m_pMixture->runAction(MoveBy::create(0.5, Vec2(260, 0)));
        
        
//        m_pBowl->runAction(MoveTo::create(0.5, CMVisibleRect::getPositionAdapted(Vec2(320, 350))));
//        m_pBowlFront->runAction(MoveTo::create(0.5, CMVisibleRect::getPositionAdapted(Vec2(320, 350))));
//        m_pMixture->runAction(MoveTo::create(0.5, CMVisibleRect::getPositionAdapted(Vec2(320, 350))));
        m_pBowl->runAction(MoveBy::create(0.5, Vec2(110, 0)));
        m_pBowlFront->runAction(MoveBy::create(0.5, Vec2(110, 0)));
        m_pMixture->runAction(MoveBy::create(0.5, Vec2(110, 0)));
    }
}

void MixFlourScene::_rollDough()
{
    if (m_bRollToolReady){
        m_nRollCount++;
        float scale = 1.0+1.0*m_nRollCount/300;
        m_pDough->setScale(scale);
        log("====scale === %f",scale);
        m_pFlourSprinkle->setOpacity(255-m_nRollCount);
        if (scale==1.5) {
            m_pDough->setVisible(false);
            m_pDoughPan->setVisible(true);
            m_pDoughPan->setScale(0.7);
        }else if (scale>1.5){
            float scale2 = 0.7+1.0*(m_nRollCount-150)/300;
            m_pDoughPan->setScale(scale2);
            log("====scale2 === %f",scale2);
            if (scale2>=1) {
                m_pDoughPan->setScale(1);
                m_pRollTool->ignoreMoving = true;
                m_pRollTool->setTouchEnabled(false);
                m_pFlourSprinkle->removeFromParent();
                m_pRollTool->runAction(Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width, 0)), NULL));
                _finishRoll();
            }
        }
    }else{
        m_bRollToolReady = true;
//        AudioHelp::getInstance()->playLoopEffect("");
        m_pRollTool->setTouchEnabled(false);
        m_pRollTool->setLocalZOrder(5);
        m_pRollTool->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.1,1, 0.95),
                                                                    ScaleTo::create(0.1,1),
                                                                    NULL)));
        m_pRollTool->setTouchEnabled(true);
        m_pDoughGrid->runAction(RepeatForever::create(Waves::create(3, Size(10, 10), 1, 5, true, true)));
//        m_pDough->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.2),
//                                                                   ScaleBy::create(0.3, 0.1),
//                                                                   DelayTime::create(0.2),
//                                                                   ScaleBy::create(0.3, -0.08),
//                                                                     NULL)));
        
    }

}

void MixFlourScene::_addDoughPan()
{
    m_pDragDough->setTouchEnabled(false);
    m_pDragDough->setVisible(false);
    m_pDoughGrid->setPosition(Vec2(m_pPan->getPositionX(), m_pPan->getPositionY()+150));
    m_pDoughPan->setScale(0.7);
    m_pDoughGrid->setVisible(true);
    m_pPan->setLocalZOrder(-1);
    
    m_pDoughGrid->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, -160)),
                                             Spawn::create(Waves::create(0.5, Size(10, 10), 1, 5, true, true),
                                                           MoveBy::create(0.3, Vec2(0, 10)), NULL),
                                             CallFunc::create([=](){
//        _createSauceUI();
    }), NULL));
//    m_pDoughGrid->runAction(MoveTo::create(0.5, CMVisibleRect::getPosition(480, 320)));
//    m_pDoughPan->runAction(ScaleTo::create(0.5, 1));
//    m_pPan->runAction(Sequence::create(MoveTo::create(0.5, CMVisibleRect::getPosition(480, 320)),
//                                       ScaleTo::create(0.5, 1), NULL));
    m_pDoughPan->runAction(ScaleTo::create(0.5, 1));
    m_pPan->runAction(ScaleTo::create(0.5, 1));
    
}


void MixFlourScene::_playEffectNice(float)
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
void MixFlourScene::_finishAdd(int index)
{
    switch (index) {
        case 0:
        {
            _moveDownMixIngredient(IngredientTag::eTagWater);
        }
            break;
        case 1:
        {
            _showWhisk();
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

void MixFlourScene::_finishMix()
{
    _removeWhisk();
//    210
    if (2!=m_nStep){
//        m_pBowl->runAction(MoveBy::create(0.5, Vec2(-110, 0)));
//        m_pBowlFront->runAction(MoveBy::create(0.5, Vec2(-110, 0)));
//        m_pMixture->runAction(MoveBy::create(0.5, Vec2(-110, 0)));
    }
    if (0==m_nStep) {
        _showMixIngredient(m_nStep+1);
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=]{
            _moveDownMixIngredient(IngredientTag::eTagButter);
            _moveDownMixIngredient(IngredientTag::eTagMilk);
            AudioHelp::getInstance()->playEffect("vo_add_ingredients.mp3");
        }), NULL));
    }else if (1==m_nStep) {
        _showMixIngredient(m_nStep+1);
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=]{
            _moveDownMixIngredient(IngredientTag::eTagFlour);
            _moveDownMixIngredient(IngredientTag::eTagYeast);
            _moveDownMixIngredient(IngredientTag::eTagSalt);
            AudioHelp::getInstance()->playEffect("vo_add_ingredients.mp3");
        }), NULL));
    }else if (2==m_nStep) {
        AudioHelp::getInstance()->playEffect("done.mp3");
        AudioHelp::getInstance()->playEffect(((arc4random()%100)>50?"vo_nice_work.mp3":"vo_well_done.mp3"));
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
        this->addChildToUILayer(pParticle);
        pParticle->setLocalZOrder(10);
        pParticle->setPosition(visibleSize*0.5);
        m_pGameUI->showNextLayout();
    }
    m_nStep++;
}

void MixFlourScene::_finishRoll()
{
    m_pDoughGrid->stopAllActions();
    m_pPan = Sprite::create(localPath("make1/pan.png"));
    this->addChildToContentLayer(m_pPan);
    CMVisibleRect::setPosition(m_pPan, 300-visibleSize.width, 320);
    m_pPan->setScale(0.7);
    
    m_pDragDough = _createDrageNode(localPath("make1/dough2.png"));
    this->addChildToContentLayer(m_pDragDough);
    m_pDragDough->setVisible(false);
    m_pDragDough->setTouchEnabled(false);
    
    m_pDoughPan->runAction(ScaleTo::create(0.5, 0.7));
    m_pDoughGrid->runAction(Sequence::create(MoveBy::create(0.5, Vec2(300, 0)),
                                             CallFunc::create([=](){
        m_pDoughGrid->setVisible(false);
        m_pDragDough->setPosition(m_pDoughGrid->getPosition());
        m_pDragDough->setScale(0.7);
        m_pDragDough->setVisible(true);
        m_pDragDough->setOrgPositionDefault();
        m_pDragDough->setTouchEnabled(true);
    }), NULL));
    
    m_pPan->runAction(Sequence::create(DelayTime::create(0.3),
                                       MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                       CallFunc::create([=](){
        m_pGuideLayer->showGuideMove(m_pDragDough->getPosition(), m_pPan->getPosition());
    }), NULL));
}
