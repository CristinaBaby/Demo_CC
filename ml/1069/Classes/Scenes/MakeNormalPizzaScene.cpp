
#include "MakeNormalPizzaScene.h"
#include "SceneManager.h"

MakeNormalPizzaScene::MakeNormalPizzaScene()
{
    m_pBowl = nullptr;
    m_pWhrisk = nullptr;
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
    
    hasPermission = false;
//    AudioHelp::getInstance()->registerEffectScene(ClassString(MakeNormalPizzaScene));
}

MakeNormalPizzaScene::~MakeNormalPizzaScene()
{
    CC_SAFE_RELEASE(m_pDecorationRipe);
//    AudioHelp::getInstance()->removeEffectScene(ClassString(MakeNormalPizzaScene));
}
bool MakeNormalPizzaScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    Sprite* pBg = Sprite::create(localPath("make/make_bg.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        _createMixUI();
    }), NULL));
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    if (!UserDefault::getInstance() -> getBoolForKey("removeAds")) {
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeInterstitialAds);
    }
    
    m_pGameUI->showNormalLayout();
    AudioHelp::getInstance()->playBackGroundMusic("make_bg.mp3");
    return true;
}

void MakeNormalPizzaScene::onEnter()
{
    ExtensionScene::onEnter();
//    FlurryEventManager::getInstance()->logCurrentModuleEnterEvent(Flurry_EVENT_MAKE_NORMAL);
    Analytics::getInstance()->sendScreenEvent(Flurry_EVENT_MAKE_NORMAL);
}

void MakeNormalPizzaScene::onExit()
{
//    FlurryEventManager::getInstance()->logCurrentModuleExitEvent(Flurry_EVENT_MAKE_NORMAL);
    ExtensionScene::onExit();
}

void MakeNormalPizzaScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    m_pDecorationScrollView->decorationData = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
    m_pDecorationScrollView->reloadData();
}

void MakeNormalPizzaScene::onButtonCallback(Button* btn)
{
    int tag = btn->getTag();
    if (GameUILayoutLayer::eUIButtonTagNext==tag){
//        btn->setTouchEnabled(false);
//        m_pGameUI->hideNext();
//        _finishDecorate();

        m_btn = btn;
#if __cplusplus > 201100L
        RuntimePermissionManager::getInstance()->onPermissionGrantedResult = [&](int           requestcode,bool bgranted){
            onPermissionGrantedResult(requestcode, bgranted);
        };
#else
        RuntimePermissionManager::getInstance()->mRuntimePermissionDelegate = this;
#endif
        
        //调用申请权限接口的标识，会在你的回调方法中用到，可以是任何值
        int requestCode = 1;
        //调用权限申请的方法,根据需要申请敏感权限
        RuntimePermissionManager::getInstance()->requestRuntimePermissions(requestCode, PERMISSION::kWriteExternalStorage);
        
        return;
    }
    ExtensionScene::onButtonCallback(btn);
}

void MakeNormalPizzaScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    if (pDragNode==m_pSholve) {
        pDragNode->setLocalZOrder(-2);
    }else{
        pDragNode->setLocalZOrder(10);
    }
    if (pDragNode==m_pWhrisk) {
        Sprite *stick = (Sprite*)pDragNode->getChildByName("stick");
        
        if (stick){
            stick->runAction(RepeatForever::create(AnimationHelp::createAnimate(localPath("make/agitato"), 2, 4)));
        }
        
        if (m_bWhriskIsReady){
            AudioHelp::getInstance()->playLoopEffect("machine_stir.mp3");
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
            m_pMixture->runAction(RepeatForever::create(RotateBy::create(1, 360)));
//            m_pMixture->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.2),
//                                                                         CallFunc::create([=]()
//                                                                                          {
//                                                                                              m_pMixture->setScaleX(-1);
//                                                                                          }),
//                                                                         DelayTime::create(0.2),
//                                                                         CallFunc::create([=]()
//                                                                                          {
//                                                                                              m_pMixture->setScaleX(1);
//                                                                                          }),
//                                                                         NULL)));
            
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

void MakeNormalPizzaScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
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
    }else if (pDragNode==m_pOilBrush) {
        if (m_bOilReady) {
            _brushOil();
        }else{
            Rect rect = m_pOil->getBoundingBox();
            rect.origin = m_pOil->getParent()->convertToWorldSpace(rect.origin);
            if (rect.containsPoint(worldPoint)){
                _brushOil();
            }
        }
    }else if (pDragNode==m_pSauceSpoon) {
        if (m_bSauceReady) {
            _brushSauce();
        }else{
            Rect rect = m_pSauce->getBoundingBox();
            rect.origin = m_pSauce->getParent()->convertToWorldSpace(rect.origin);
            if (rect.containsPoint(worldPoint)){
                _brushSauce();
            }
        }
    }else if (pDragNode==m_pCheese) {
        Rect rect = pDragNode->getBoundingBox();
        if (rect.containsPoint(m_pGrater->getPosition())) {
            if (m_bGraterReady) {
//                _sliceCheese();
                
                if(hasPermission) {
                    _sliceCheese();
                } else {
#if __cplusplus > 201100L
                    RuntimePermissionManager::getInstance()->onPermissionGrantedResult = [&](int           requestcode,bool bgranted){
                        onPermissionGrantedResult(requestcode, bgranted);
                    };
#else
                    RuntimePermissionManager::getInstance()->mRuntimePermissionDelegate = this;
#endif
                    
                    //调用申请权限接口的标识，会在你的回调方法中用到，可以是任何值
                    int requestCode = 2;
                    //调用权限申请的方法,根据需要申请敏感权限
                    RuntimePermissionManager::getInstance()->requestRuntimePermissions(requestCode, PERMISSION::kWriteExternalStorage);
                }
                
                

                
                
                
            }else{
                m_bGraterReady = true;
                Vec2 location = m_pPan->getPosition();
                Vec2 pos = Vec2(location.x, location.y+80);
                m_pGrater->runAction(RepeatForever::create(Sequence::create(CircleMove::create(20, pos, 150, 360*6),
                                                                            NULL)));
            }
        }
        
    }
}

void MakeNormalPizzaScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    m_pGuideLayer->removeGuide();
    AudioHelp::getInstance()->stopLoopEffect();
    pDragNode->stopAllActions();
    //用sholve的时候 不能执行m_pDoughGrid 不然会有意想不到的情况  不晓得啥原因
    if (pDragNode==m_pSholve){
        _getSholvePizza();
        return;
    }
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
    }else if (pDragNode==m_pOilBrush) {
//        m_pOilBrush->back();
    }else if (pDragNode==m_pSauceSpoon) {
//        m_pSauceSpoon->back();
    }else if (pDragNode==m_pGrater) {
        m_pGrater->setTouchEnabled(false);
//        m_pGrater->runAction(CircleMove::create(3, m_pPan->getPosition(), 0.0f, 1500));
        
        Vec2 location = m_pPan->getPosition();
        Vec2 pos = Vec2(location.x, location.y+80);
        m_pGrater->setPosition(Vec2(location.x+150, location.y+80));
//        m_pGrater->runAction(RepeatForever::create(Sequence::create(CircleMove::create(20, pos, 150, 360*6),
//                                                                    NULL)));
    }else if (pDragNode==m_pCheese) {
        if (!m_bGraterReady) {
            pDragNode->back();
        }
    }else if (pDragNode==m_pSholve) {
        _getSholvePizza();
    }else{
        Rect rect = m_pBowl->getBoundingBox();
        if (rect.containsPoint(pDragNode->getPosition()) && m_bReadyAdd) {
            pDragNode->setTouchEnabled(false);
            m_bReadyAdd = false;
            _addIngredient(pDragNode);
        }else{
            pDragNode->back(0,[=](){
            });
        }
    }
}

void MakeNormalPizzaScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->stopAllActions();
    if (m_pDoughGrid) {
        m_pDoughGrid->stopAllActions();
    }else if (m_pDragDough==pDragNode) {
        m_pDragDough->runAction(EaseBackOut::create(ScaleTo::create(0.3, 0.7)));
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

}
void MakeNormalPizzaScene::TouchBegan(Touch *pTouch, Event *pEvent)
{
    
}
void MakeNormalPizzaScene::TouchEnded(Touch *pTouch, Event *pEvent)
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
void MakeNormalPizzaScene::_initData()
{
    setExPath("content/make/normal/");
}

#pragma mark ==== Mix UI =====
void MakeNormalPizzaScene::_createMixUI()
{
    m_pBowl = Sprite::create(localPath("make/bowl.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 300-visibleSize.width/2, 350);
    
    m_pMixture = Node::create();
    this->addChildToContentLayer(m_pMixture);
    CMVisibleRect::setPositionAdapted(m_pMixture, 300, 350);
    m_pMixture->setLocalZOrder(5);
    
    m_pBowl->runAction(MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)));
    
    _showMixIngredient(0);
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=]{
        m_pGuideLayer->showGuideMove(m_pFlour->getPosition(), m_pBowl->getPosition());
    }), NULL));
    AudioHelp::getInstance()->playEffect("vo_add_ingredients.mp3");
}

void MakeNormalPizzaScene::_showMixIngredient(int index)
{
    if (index==0) {
        m_pFlour = _createDrageNode(localPath("make/flour.png"),false);
        this->addChildToContentLayer(m_pFlour);
        CMVisibleRect::setPositionAdapted(m_pFlour, 735+visibleSize.width, 350);
        m_pFlour->setActionPoint(Vec2(62, 277));
        m_pFlour->setTag(0);
        m_pFlour->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                             CallFuncN::create([=](Node* pNode){
            DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
            if (pIngredient) {
                pIngredient->setOrgPositionDefault();
                pIngredient->setTouchEnabled(true);
            }
        }), NULL));
    }else if (index==1) {
        m_pYeast = _createDrageNode(localPath("make/yeast.png"),false);
        this->addChildToContentLayer(m_pYeast);
        CMVisibleRect::setPositionAdapted(m_pYeast, 640+visibleSize.width, 480);
        m_pYeast->setActionPoint(Vec2(46, 166));
        m_pYeast->setTag(1);
        
        m_pSalt = _createDrageNode(localPath("make/salt.png"),false);
        this->addChildToContentLayer(m_pSalt);
        CMVisibleRect::setPositionAdapted(m_pSalt, 840+visibleSize.width, 480);
        m_pSalt->setActionPoint(Vec2(61, 170));
        m_pSalt->setTag(2);
        
        m_pSugar = _createDrageNode(localPath("make/sugar.png"),false);
        this->addChildToContentLayer(m_pSugar);
        CMVisibleRect::setPositionAdapted(m_pSugar, 610+visibleSize.width, 230);
        m_pSugar->setActionPoint(Vec2(64, 170));
        m_pSugar->setTag(3);
        
        m_pWater = _createDrageNode(localPath("make/water0.png"),false);
        this->addChildToContentLayer(m_pWater);
        CMVisibleRect::setPositionAdapted(m_pWater, 800+visibleSize.width, 250);
        m_pWater->setActionPoint(Vec2(28, 231));
        m_pWater->setTag(4);
        
        auto action = Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                       CallFuncN::create([=](Node* pNode){
            DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
            if (pIngredient) {
                pIngredient->setOrgPositionDefault();
                pIngredient->setTouchEnabled(true);
            }
        }), NULL);
        m_pYeast->runAction(action);
        m_pSalt->runAction(action->clone());
        m_pSugar->runAction(action->clone());
        m_pWater->runAction(action->clone());
    }
}

void MakeNormalPizzaScene::_showWhisk()
{
    m_pWhrisk = _createDrageNode(localPath("make/agitato.png"));
    this->addChildToContentLayer(m_pWhrisk);
    CMVisibleRect::setPositionAdapted(m_pWhrisk, 740+visibleSize.width, 360);
    m_pWhrisk->setActionPoint(Vec2(45, 60));
    m_pWhrisk->setLocalZOrder(10);
    
    Sprite* pSprite = Sprite::create(localPath("make/agitato2.png"));
    m_pWhrisk->addChild(pSprite,-1);
    pSprite->setName("stick");
    
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

void MakeNormalPizzaScene::_removeWhisk()
{
    m_pWhrisk->setTouchEnabled(false);
    m_pWhrisk->runAction(Sequence::create(DelayTime::create(0.5),
                                          EaseBackOut::create(MoveBy::create(1, Vec2(visibleSize.width, 0))),
                                          CallFunc::create([=]()
                                                           {
                                                           }),
                                          NULL));
}

#pragma mark ==== roll UI =====
void MakeNormalPizzaScene::_createRollUI()
{
    m_pFlourSprinkle = Sprite::create(localPath("make/powder.png"));
    this->addChildToContentLayer(m_pFlourSprinkle);
    CMVisibleRect::setPositionAdapted(m_pFlourSprinkle, 480, 320);
    
    m_pDoughGrid = NodeGrid::create();
    this->addChildToContentLayer(m_pDoughGrid);
    CMVisibleRect::setPositionAdapted(m_pDoughGrid, 480, 320);
    
    m_pDough = Sprite::create(localPath("make/dough0.png"));
    m_pDoughGrid->addChild(m_pDough);
    
    m_pDoughPan = Sprite::create(localPath("make/dough2.png"));
    m_pDoughGrid->addChild(m_pDoughPan);
}

void MakeNormalPizzaScene::_pourDough()
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

void MakeNormalPizzaScene::_showSpreadFlour()
{
    Sprite* pFlourBag = Sprite::create(localPath("make/flour_h.png"));
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
        Sprite* pFlour = Sprite::create(localPath("make/powder0.png"));
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

void MakeNormalPizzaScene::_showRollTool()
{
    AudioHelp::getInstance()->playEffect("vo_roll_dough.mp3");
    m_pRollTool = _createDrageNode(localPath("make/rolling.png"));
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

void MakeNormalPizzaScene::_removeRollTool()
{
    
}

#pragma mark ===== sauce UI ====
void MakeNormalPizzaScene::_createSauceUI()
{
    AudioHelp::getInstance()->playEffect("vo_brush_oil.mp3");
    _showSauceIngredient();
    m_pOilScribble = ScribbleNode::create(m_pDoughPan->getContentSize());
    this->addChildToContentLayer(m_pOilScribble);
    m_pOilScribble->setPosition(m_pDoughGrid->getPosition());
    m_pOilScribble->useBrush("content/make/brush.png");
    m_pOilScribble->useTarget(localPath("make/oil2.png"));
    
    m_pSauceScribble = ScribbleNode::create(m_pDoughPan->getContentSize());
    this->addChildToContentLayer(m_pSauceScribble);
    m_pSauceScribble->setPosition(m_pDoughGrid->getPosition());
    m_pSauceScribble->useBrush("content/make/brush.png");
    m_pSauceScribble->useTarget(localPath("make/sauce2.png"));
    
    MyRenderTexture* pOilTempRT = MyRenderTexture::create(m_pDoughPan->getContentSize().width, m_pDoughPan->getContentSize().height);
    Sprite* pOilTemp = Sprite::create(localPath("make/oil2.png"));
    pOilTemp->setAnchorPoint(Vec2::ZERO);
    pOilTempRT->begin();
    pOilTemp->visit();
    pOilTempRT->end();
    
    MyRenderTexture* pSauceTempRT = MyRenderTexture::create(m_pDoughPan->getContentSize().width, m_pDoughPan->getContentSize().height);
    Sprite* pSauceTemp = Sprite::create(localPath("make/oil2.png"));
    pSauceTemp->setAnchorPoint(Vec2::ZERO);
    pSauceTempRT->begin();
    pSauceTemp->visit();
    pSauceTempRT->end();
    
    Director::getInstance()->getRenderer()->render();
    
    m_nOilPercent = 100-pOilTempRT->getPercentageTransparent();
    m_nSaucePercent = 100-pSauceTempRT->getPercentageTransparent();
    
    m_pOilBrush = _createDrageNode(localPath("make/oil_brush.png"));
    this->addChildToContentLayer(m_pOilBrush);
    CMVisibleRect::setPositionAdapted(m_pOilBrush, 720+visibleSize.width, 500);
    m_pOilBrush->setActionPoint(Vec2(34, 148));
    
    m_pSauceSpoon = _createDrageNode(localPath("make/sauce_spoon.png"));
    this->addChildToContentLayer(m_pSauceSpoon);
    CMVisibleRect::setPositionAdapted(m_pSauceSpoon, 720+visibleSize.width, 255);
    m_pSauceSpoon->setActionPoint(Vec2(29, 174));
    
    m_pOilBrush->setTouchEnabled(false);
    m_pOilBrush->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                            CallFunc::create([=](){
        m_pOilBrush->setOrgPositionDefault();
        m_pOilBrush->setTouchEnabled(true);
    }), NULL));
    m_pSauceSpoon->setTouchEnabled(false);
    m_pSauceSpoon->setMovable(false);
    m_pSauceSpoon->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                            CallFunc::create([=](){
        m_pSauceSpoon->setOrgPositionDefault();
        m_pSauceSpoon->setTouchEnabled(true);
    }), NULL));
}

void MakeNormalPizzaScene::_showSauceIngredient()
{
    m_pOilPlate = Sprite::create(localPath("make/oil_bowl.png"));
    this->addChildToContentLayer(m_pOilPlate);
    CMVisibleRect::setPositionAdapted(m_pOilPlate, 720, 500);
    
    m_pSaucePlate = Sprite::create(localPath("make/sauce_plate.png"));
    this->addChildToContentLayer(m_pSaucePlate);
    CMVisibleRect::setPositionAdapted(m_pSaucePlate, 720, 255);
    
    m_pOilGrid = NodeGrid::create();
    m_pOilPlate->addChild(m_pOilGrid);
    m_pOilGrid->setPosition(Vec2(0, 10)+m_pOilPlate->getContentSize()*0.5);
    
    m_pOil = Sprite::create(localPath("make/oil0.png"));
    m_pOilGrid->addChild(m_pOil);
    
    m_pSauceGrid = NodeGrid::create();
    m_pSaucePlate->addChild(m_pSauceGrid);
    m_pSauceGrid->setPosition(Vec2(0, 10)+m_pSaucePlate->getContentSize()*0.5);
    
    m_pSauce = Sprite::create(localPath("make/sauce0.png"));
    m_pSauceGrid->addChild(m_pSauce);
}

void MakeNormalPizzaScene::_removeSauceIngredient()
{
    
}

#pragma mark ====== cheese UI ======
void MakeNormalPizzaScene::_createCheeseUI()
{
    m_pCheeseSlice = Node::create();
    this->addChildToContentLayer(m_pCheeseSlice);
    m_pCheeseSlice->setPosition(m_pPan->getPosition());
    
    _showCheeseIngredient();
}

void MakeNormalPizzaScene::_showCheeseIngredient()
{
    m_pCheese = _createDrageNode(localPath("make/cheese0.png"));
    this->addChildToContentLayer(m_pCheese);
    CMVisibleRect::setPosition(m_pCheese, 715+visibleSize.width, 480);
    
    m_pGrater = _createDrageNode(localPath("make/grater.png"));
    this->addChildToContentLayer(m_pGrater);
    CMVisibleRect::setPosition(m_pGrater, 730+visibleSize.width, 270);
    
    m_pCheese->setTouchEnabled(false);
    m_pGrater->setTouchEnabled(false);
    m_pGrater->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                          CallFunc::create([=](){
        m_pGrater->setOrgPositionDefault();
        m_pGrater->setTouchEnabled(true);
        m_pGuideLayer->showGuideMove(m_pGrater->getPosition(), m_pPan->getPosition());
    }), NULL));
    
    m_pCheese->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                          CallFunc::create([=](){
        m_pCheese->setOrgPositionDefault();
        m_pCheese->setTouchEnabled(true);
        AudioHelp::getInstance()->playEffect("vo_slice_cheese.mp3");
    }), NULL));
}

void MakeNormalPizzaScene::_removeCheeseIngredient()
{
    
}

#pragma mark ======= decorate UI =====
void MakeNormalPizzaScene::_showDecorations()
{
    m_pDecoration = Node::create();
    this->addChildToContentLayer(m_pDecoration);
    m_pDecoration->setPosition(m_pPan->getPosition());
    
    m_pDecorationRipe = Node::create();
    m_pDecorationRipe->retain();
    
    m_pTypeScrollView = DecTypeScrollView::createWithSize(Size(240,480));
    this->addChildToUILayer(m_pTypeScrollView);
    CMVisibleRect::setPositionAdapted(m_pTypeScrollView,  -10-visibleSize.width/2,(640-m_pTypeScrollView->getContentSize().height)/2-50+30,kBorderLeft);
    m_pTypeScrollView->onItemCellSelected = CC_CALLBACK_3(MakeNormalPizzaScene::_onTypeCallback, this);
    m_pTypeScrollView->btnPathEX = "content/make/normal/";
    m_pTypeScrollView->boxPathEX = localPath("box.png");
    m_pTypeScrollView->setLocalZOrder(1);
    m_pTypeScrollView->setDirection(DecTypeScrollView::Direction::eDirectionV);
    m_pTypeScrollView->runAction(Sequence::create(DelayTime::create(0.5),
                                                  EaseBackIn::create(MoveBy::create(1, Vec2(visibleSize.width/2, 0))),
                                                  
                                                  CallFunc::create([=](){
        
        AudioHelp::getInstance()->playEffect("vo_dec_choose.mp3");
    }),
                                                  DelayTime::create(2),
                                                  CallFunc::create([=](){
        
        AudioHelp::getInstance()->playEffect("vo_add_food_pizza.mp3");
    }), NULL));
    m_pTypeScrollView->setSingleAsset(true);
    m_pTypeScrollView->setMargin(-20);
    m_pTypeScrollView->setSelectedAnimate(true);
    m_pTypeScrollView->loadType(ConfigManager::getInstance()->getDecorateType("normalIngrident"));
    m_pTypeScrollView->setScale(0.8);
    
    m_pTouchLayer = TouchLayer::create();
    this->addChildToUILayer(m_pTouchLayer);
    m_pTouchLayer->onTouchBegan = CC_CALLBACK_2(MakeNormalPizzaScene::TouchBegan, this);
    m_pTouchLayer->onTouchEnded = CC_CALLBACK_2(MakeNormalPizzaScene::TouchEnded, this);
    std::memset(m_bGuide, 0, 3);
}
void MakeNormalPizzaScene::_removeDecorations()
{
    
    if (m_pTypeScrollView) {
        m_pTypeScrollView->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)),
                                                      CallFunc::create([=]()
                                                                       {
                                                                           m_pTypeScrollView->removeFromParent();
                                                                           m_pTypeScrollView = nullptr;
                                                                       }), NULL));
    }
    if (m_pDecorationScrollView) {
        m_pDecorationScrollView->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width/2, 0)),
                                                            CallFunc::create([=]()
                                                                             {
                                                                                 m_pDecorationScrollView->removeFromParent();
                                                                                 m_pDecorationScrollView = nullptr;
                                                                             }), NULL));
    }
}

#pragma mark ======== Sholve ========================
void MakeNormalPizzaScene::_showSholve()
{
    m_pSholve = _createDrageNode(localPath("make/pizza_shovel0.png"));
    this->addChildToContentLayer(m_pSholve);
    CMVisibleRect::setPositionAdapted(m_pSholve, 950+visibleSize.width, 50);
    m_pSholve->setTouchEnabled(false);
    m_pSholve->setActionPoint(Vec2(265, 636));
    m_pSholve->setLimitRect(Rect(0, 0, visibleSize.width+100, visibleSize.height+100));
    m_pSholve->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-visibleSize.width, 0)),
                                          CallFunc::create([=](){
        m_pSholve->setOrgPositionDefault();
        m_pSholve->setTouchEnabled(true);
        m_pGuideLayer->showGuideMove(m_pSholve->getPosition(), m_pPan->getPosition());
    }), NULL));
}

#pragma mark ======== deel ========================

void MakeNormalPizzaScene::_addIngredient(DragNode* pIngredient)
{
    IngredientIndex index = (IngredientIndex)pIngredient->getTag();
    std::string lNameStr = "";
    float dt = 1;
    Vec2 posAdd = Vec2::ZERO;
    pIngredient->setLocalZOrder(5);
    switch (index) {
#pragma mark === add Flour ===
        case IngredientIndex::eFlour:
        {
            lNameStr = "make/flour1.png";
            pIngredient->setTouchEnabled(false);
            pIngredient->getDragSprite()->setTexture(localPath("make/flour_h.png"));
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+240, m_pBowl->getPositionY()+150));
            pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -100),
                                                 CallFunc::create([=](){
                AudioHelp::getInstance()->playPourPowderEffect();
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/flour.plist");
                this->addChildToContentLayer(pParticle);
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                pParticle->setLocalZOrder(6);
            }),
                                                 ScaleTo::create(0.5, 1.1,0.9),
                                                 DelayTime::create(1),
                                                 ScaleTo::create(0.5, 1),
                                                 RotateTo::create(0.3, 0),
                                                 MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                 CallFunc::create([=](){
                m_nAddCount++;
                _showMixIngredient(1);
                
                m_bReadyAdd = true;
                pIngredient->removeFromParent();
            }),
                                                 NULL));
        }
            break;
#pragma mark === add Yeast ===
        case IngredientIndex::eYeast:
        {
            lNameStr = "make/yeast1.png";
            pIngredient->setTouchEnabled(false);
            pIngredient->getDragSprite()->setTexture(localPath("make/yeast_h.png"));
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+200, m_pBowl->getPositionY()+150));
            pIngredient->runAction(Sequence::create(RotateBy::create(0.5, -100),
                                                 CallFunc::create([=](){
                AudioHelp::getInstance()->playPourPowderEffect();
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/yeast.plist");
                this->addChildToContentLayer(pParticle);
                pParticle->setPosition(m_pContentLayer->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                pParticle->setLocalZOrder(6);
            }),
                                                 ScaleTo::create(0.5, 1.1,0.9),
                                                 DelayTime::create(1),
                                                 ScaleTo::create(0.5, 1),
                                                 RotateTo::create(0.3, 0),
                                                 MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                 CallFunc::create([=](){
                m_nAddCount++;
                if (m_nAddCount==5) {
                    _finishAdd();
                }
                m_bReadyAdd = true;
                pIngredient->removeFromParent();
            }),
                                                 NULL));
        }
            break;
#pragma mark === add Salt ===
        case IngredientIndex::eSalt:
        {
            lNameStr = "make/salt1.png";
            pIngredient->setTouchEnabled(false);
            
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+200, m_pBowl->getPositionY()+150));
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
                if (m_nAddCount==5) {
                    _finishAdd();
                }
                
                m_bReadyAdd = true;
                pIngredient->removeFromParent();
            }),
                                                NULL));
        }
            break;
            
#pragma mark === add Sugar ===
        case IngredientIndex::eSugar:
        {
            lNameStr = "make/sugar1.png";
            pIngredient->setTouchEnabled(false);
            
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+160, m_pBowl->getPositionY()+130));
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
                if (m_nAddCount==5) {
                    _finishAdd();
                }
                
                m_bReadyAdd = true;
                pIngredient->removeFromParent();
            }),
                                                    NULL));
        }
            break;
#pragma mark === add Water ===
        case IngredientIndex::eWater:
        {
            lNameStr = "make/water1.png";
            pIngredient->setTouchEnabled(false);
            
            pIngredient->setPosition(Vec2(m_pBowl->getPositionX()+100, m_pBowl->getPositionY()+200));
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
            }),
                                                    DelayTime::create(2),
                                                    RotateTo::create(0.3, 0),
                                                    MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                m_nAddCount++;
                if (m_nAddCount==5) {
                    _finishAdd();
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

void MakeNormalPizzaScene::_mixBatter()
{
    if (m_bWhriskIsReady){
        m_nMixCount++;
        
        if (m_nMixCount%80==0) {
            int mixtureStep = m_nMixCount/80;
            if (mixtureStep<=4){
                std::stringstream ostr;
                ostr<<"make/batter"<<mixtureStep<<".png";
                Sprite* pMixture = Sprite::create(localPath(ostr.str()));
                m_pMixture->removeAllChildren();
                m_pMixture->addChild(pMixture);
                
            }
            if (mixtureStep>=4){
                //mix finish
                m_pMixture->stopAllActions();
                _finishMix();
                AudioHelp::getInstance()->stopLoopEffect();
            }
        }
    }else{
        m_bWhriskIsReady = true;
        AudioHelp::getInstance()->playLoopEffect("machine_stir.mp3");
//        m_pWhrisk->setTouchEnabled(false);
        m_pWhrisk->getDragSprite()->setTexture(localPath("make/agitato1.png"));
        m_pWhrisk->setLocalZOrder(5);
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
        m_pMixture->runAction(RepeatForever::create(RotateBy::create(1, 360)));
//        m_pMixture->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.2),
//                                                                     CallFunc::create([=]()
//                                                                                      {
//                                                                                          m_pMixture->setScaleX(-1);
//                                                                                      }),
//                                                                     DelayTime::create(0.2),
//                                                                     CallFunc::create([=]()
//                                                                                      {
//                                                                                          m_pMixture->setScaleX(1);
//                                                                                      }),
//                                                                     NULL)));
        m_pWhrisk->runAction(Sequence::create(MoveTo::create(0.5, Vec2(visibleSize.width*0.5,visibleSize.height*0.5+100)),
                                              CallFunc::create([=](){
            Sprite *stick = (Sprite*)m_pWhrisk->getChildByName("stick");
            
            if (stick){
                stick->runAction(RepeatForever::create(AnimationHelp::createAnimate(localPath("make/agitato"), 2, 4)));
            }
            
            m_pWhrisk->setTouchEnabled(true);
        }), NULL));
//        m_pBowl->runAction(MoveBy::create(0.5, Vec2(260, 0)));
//        m_pMixture->runAction(MoveBy::create(0.5, Vec2(260, 0)));
        
        
        m_pBowl->runAction(MoveTo::create(0.5, CMVisibleRect::getPositionAdapted(Vec2(480, 350))));
        m_pMixture->runAction(MoveTo::create(0.5, CMVisibleRect::getPositionAdapted(Vec2(480, 350))));
    }
}

void MakeNormalPizzaScene::_rollDough()
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

void MakeNormalPizzaScene::_addDoughPan()
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
        _createSauceUI();
    }), NULL));
//    m_pDoughGrid->runAction(MoveTo::create(0.5, CMVisibleRect::getPosition(480, 320)));
//    m_pDoughPan->runAction(ScaleTo::create(0.5, 1));
//    m_pPan->runAction(Sequence::create(MoveTo::create(0.5, CMVisibleRect::getPosition(480, 320)),
//                                       ScaleTo::create(0.5, 1), NULL));
    m_pDoughPan->runAction(ScaleTo::create(0.5, 1));
    m_pPan->runAction(ScaleTo::create(0.5, 1));
    
}

void MakeNormalPizzaScene::_brushOil()
{
    if (m_bOilReady){
        Vec2 endPos = m_pOilBrush->getWorldSpaceActionPoint();
        Vec2 startPos = Vec2(m_pOilBrush->getOffset(), endPos);
        m_pOilScribble->paint(startPos, endPos);
        Sprite* pBrushOil = (Sprite*)m_pSauceSpoon->getChildByName("brush_oil");
        static int moveCount = 0;
        moveCount++;
        if (moveCount%20==0) {
            int percent = 100-m_pOilScribble->getCanvas()->getPercentageTransparent();
            if (pBrushOil) {
                pBrushOil->setOpacity(225-200.0*percent/m_nSaucePercent);
            }
            if (100.0*percent/m_nOilPercent>=96) {
                moveCount = 0;
                m_pOilScribble->paint(localPath("make/oil2.png"));
                m_pOilBrush->runAction(RotateBy::create(0.5, 100));
                _finishBrushOil();
            }
        }
    }else{
        m_bOilReady = true;
        m_pOilGrid->runAction(Repeat::create(Waves::create(0.5, Size(10, 10), 1, 5, true, true), 2));
        m_pOil->runAction(ScaleTo::create(0.5, 0.5));
        m_pOilBrush->runAction(RotateBy::create(0.5, -100));
        
        Sprite* pBrushOil = Sprite::create(localPath("make/oil_brush1.png"));
        m_pOilBrush->addChild(pBrushOil);
        pBrushOil->setName("brush_oil");
    }
}

void MakeNormalPizzaScene::_brushSauce()
{
    if (m_bSauceReady){
        Vec2 endPos = m_pSauceSpoon->getWorldSpaceActionPoint();
        Vec2 startPos = Vec2(m_pSauceSpoon->getOffset(), endPos);
        m_pSauceScribble->paint(startPos, endPos);
        Sprite* pSpoonSauce = (Sprite*)m_pSauceSpoon->getChildByName("spoon_sauce");
        static int moveCount = 0;
        moveCount++;
        if (moveCount%20==0) {
            int percent = 100-m_pSauceScribble->getCanvas()->getPercentageTransparent();
            if (pSpoonSauce) {
                pSpoonSauce->setOpacity(225-200.0*percent/m_nSaucePercent);
            }
            if (100.0*percent/m_nSaucePercent>=96) {
                moveCount = 0;
                m_pSauceScribble->paint(localPath("make/sauce2.png"));
                m_pSauceSpoon->runAction(RotateBy::create(0.5, 90));
                _finishBrushSauce();
            }
        }
    }else{
        m_bSauceReady = true;
        m_pSauceGrid->runAction(Repeat::create(Waves::create(0.5, Size(5, 7), 1, 5, true, true), 2));
        m_pSauce->runAction(ScaleTo::create(0.5, 0.5));
        m_pSauceSpoon->runAction(RotateBy::create(0.5, -90));
        Sprite* pSpoonSauce = Sprite::create(localPath("make/sauce_spoon1.png"));
        m_pSauceSpoon->addChild(pSpoonSauce);
        pSpoonSauce->setName("spoon_sauce");
    }
}


void MakeNormalPizzaScene::_sliceCheese()
{
    m_nCheeseCount++;
    
    log("====m_pCheeseSlice=%f,%f",m_pCheeseSlice->getPositionX(),m_pCheeseSlice->getPositionY());
    for (int i = 0; i<3; i++) {
        Sprite* pSlice = Sprite::create(localPath("make/cheese_crush.png"));
        m_pCheeseSlice->addChild(pSlice);
        Vec2 pos = m_pGrater->getWorldSpaceActionPoint();
        pos = m_pCheeseSlice->convertToNodeSpace(pos);
        pSlice->setPosition(pos);
        log("=====%f,%f",pos.x,pos.y);
        float rotation = 5.0*(arc4random()%80);
        float scale = 0.5+0.1*(arc4random()%10);
        if (i<2) {
            float x = -20.0+5.0*(arc4random()%8);
            float y = -60.0-5.0*(arc4random()%8);
            Vec2 movePos = Vec2(x, y);
            pSlice->runAction(Spawn::create(RotateBy::create(0.5, rotation),
                                            ScaleTo::create(0.3, scale),
                                            MoveBy::create(0.5, movePos), NULL));
        }else{
            float x = -90.0+10.0*(arc4random()%18);
            float y = 90.0-10.0*(arc4random()%18);
            Vec2 movePos = Vec2(x, y);
            pSlice->runAction(Spawn::create(RotateBy::create(0.5, rotation),
                                            ScaleTo::create(0.3, scale),
                                            MoveTo::create(0.5, movePos), NULL));
        }
        static int count = 0;
        count++;
        log("+++++++++++slice count  %d",count);
    }
    int k = 60;
    if (m_nCheeseCount%k==0) {
        int mixtureStep = m_nCheeseCount/k;
        if (mixtureStep<=2){
            std::stringstream ostr;
            ostr<<"make/cheese"<<mixtureStep<<".png";
            m_pCheese->getDragSprite()->setTexture(localPath(ostr.str()));
        }else{
            m_pCheese->setTouchEnabled(false);
            m_pCheese->setVisible(false);
        }
        if (mixtureStep>=3){
            log("======================== _sliceCheese");
            AudioHelp::getInstance()->stopLoopEffect();
            _finishCheeseSlice();
            
        }
    }
}

void MakeNormalPizzaScene::_onTypeCallback(int index,DecorateTypeConfigData typeData,bool selected)
{
    m_sCurTypeStr = typeData.decTypeName;
    AudioHelp::getInstance()->playSelectedEffect();
    DecorateConfigData data = ConfigManager::getInstance()->getDecorateFile(m_sCurTypeStr);
    m_sCurTypePath = string(data.pathName);
    m_nIndex = -1;
    if (selected) {
        std::stringstream ostr;
        ostr.str("");
        ostr<<"content/category/"<<data.pathName<<"_icon/";
        if (!m_pDecorationScrollView) {
            m_pDecorationScrollView = ItemScrollView::createWithSize(Size(260, 425),false);
            this->addChildToUILayer(m_pDecorationScrollView);
            
            m_pDecorationScrollView->decorationData = data;
            m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_3(MakeNormalPizzaScene::_onDecorationCallback, this);
        }else{
            m_pDecorationScrollView->decorationData = data;
        }
        m_pDecorationScrollView->boxPathEX = localPath("frame.png");
        m_pDecorationScrollView->setDirection(ItemScrollView::Direction::eDirectionV);
        m_pDecorationScrollView->onItemCellSelected = CC_CALLBACK_3(MakeNormalPizzaScene::_onDecorationCallback, this);
        m_pDecorationScrollView->stopAllActions();
        m_pDecorationScrollView->setMargin(10);
        //        CMVisibleRect::setPositionAdapted(m_pDecorationScrollView, (640-m_pDecorationScrollView->getContentSize().width)*0.5,60-visibleSize.height/2);
        //        m_pDecorationScrollView->runAction(MoveBy::create(0.5, Vec2(0, visibleSize.height/2)));
        m_pDecorationScrollView->setScale(0);
        CMVisibleRect::setPositionAdapted(m_pDecorationScrollView, 160-150-30,(640-425)/2-50+30,kBorderLeft);
        m_pDecorationScrollView->runAction(MoveBy::create(0.5, Vec2(150,0)));
//        m_pDecorationScrollView->runAction(MoveBy::create(0.5, Vec2(150, 0)));
        m_pDecorationScrollView->runAction(ScaleTo::create(0.2, 0.85));
//        m_pDecorationScrollView->setScale(0.85);
        
        m_pDecorationScrollView->btnPathEX = ostr.str();
        
        m_pDecorationScrollView->bgHighlightPath = "content/category/icon/c.png";
//        m_pDecorationScrollView->boxPathEX = "content/category/icon/box_2.png";
        m_pDecorationScrollView->setMargin(30);
        m_pDecorationScrollView->reloadData();
    }else{
        m_sCurTypePath = "";
        if (m_pDecorationScrollView) {
            m_pDecorationScrollView->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-150, 0)),
                                                                CallFunc::create([=]()
                                                                                 {
                                                                                     m_pDecorationScrollView->removeFromParent();
                                                                                     m_pDecorationScrollView = nullptr;
                                                                                 }), NULL));
        }
    }

}

void MakeNormalPizzaScene::_onDecorationCallback(int index,int type,DecorateConfigData decData)
{
    
    if (type==1) {
        if(!ConfigManager::getInstance()->getVideoUnLocked(decData.decTypeName, index)){
            RewardManager::getInstance()->showRewardAds(decData.decTypeName, index);
            m_pDecorationScrollView->setSelected(false);
            return;
        }
    }
    
    AudioHelp::getInstance()->playSelectedEffect();
    std::stringstream ostr;
    ostr<<"content/category/"<<decData.pathName<<"/"<<index<<".png";
    
    m_nIndex = index;
    m_bCanTouch = false;
    if (std::strcmp(decData.pathName.c_str(), "meat")==0) {
        m_bCanTouch = true;
        if (!m_bGuide[0]) {
            m_pGuideLayer->showGuideTap(Vec2(visibleSize.width*0.6, visibleSize.height*0.5));
            m_bGuide[0] = true;
        }
        return;
        
    }else if (std::strcmp(decData.pathName.c_str(), "fruit")==0) {
        m_bCanTouch = true;
        if (!m_bGuide[1]) {
            m_pGuideLayer->showGuideTap(Vec2(visibleSize.width*0.6, visibleSize.height*0.5));
            m_bGuide[1] = true;
        }
        return;
        
    }else if (std::strcmp(decData.pathName.c_str(), "vegetable")==0) {
        m_bCanTouch = true;
        if (!m_bGuide[2]) {
            m_pGuideLayer->showGuideTap(Vec2(visibleSize.width*0.6, visibleSize.height*0.5));
            m_bGuide[2] = true;
        }
        return;
        
    }
    _playEffectNice();
}

void MakeNormalPizzaScene::_playEffectNice(float)
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

RenderTexture* MakeNormalPizzaScene::_getDecorationResultRender()
{
    RenderTexture* render = RenderTexture::create(m_pPan->getContentSize().width, m_pPan->getContentSize().height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    m_pDecoration->setPosition(m_pPan->getContentSize()*0.5);
    
    render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
    m_pDecoration->cocos2d::Node::visit();
    render->end();
    
    m_pDecoration->setPosition(m_pPan->getPosition());
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    return render;

}

void MakeNormalPizzaScene::_getSholvePizza()
{
    m_pSholve->setTouchEnabled(false);
    
    Vec2 pos = m_pSholve->convertToNodeSpace(m_pSholve->getWorldSpaceActionPoint());
    m_pPan->setVisible(false);
    m_pDoughGrid->setVisible(false);
    m_pOilScribble->setVisible(false);
    m_pSauceScribble->setVisible(false);

    m_pFinishPizza = Node::create();
    m_pSholve->addChild(m_pFinishPizza);
    m_pFinishPizza->setPosition(pos);
    
    Sprite*pPan = Sprite::createWithTexture(m_pPan->getTexture());
    m_pFinishPizza->addChild(pPan);
    
    Sprite*pDough = Sprite::createWithTexture(m_pDoughPan->getTexture());
    m_pFinishPizza->addChild(pDough);
    
    Sprite*pOil = Sprite::create(localPath("make/oil2.png"));
    m_pFinishPizza->addChild(pOil);

    
    Sprite*pSauce = Sprite::create(localPath("make/sauce2.png"));
//    pSauce->setAnchorPoint(Vec2::ZERO);
    m_pFinishPizza->addChild(pSauce);
    
    
    if (m_pCheeseSlice){
        m_pCheeseSlice->retain();
        m_pCheeseSlice->removeFromParent();
        m_pFinishPizza->addChild(m_pCheeseSlice);
        m_pCheeseSlice->release();
        m_pCheeseSlice->setPosition(Vec2::ZERO);
        
    }
    
    if (m_pDecoration) {
//        m_pDecoration->retain();
        m_pDecoration->removeFromParent();
//        m_pFinishPizza->addChild(m_pDecoration);
//        m_pDecoration->release();
//        m_pDecoration->setPosition(Vec2::ZERO);
        Sprite* pDec;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        pDec = Sprite::create(SSCFileUtility::getStoragePath()+"normal_dec.png");
#else
        pDec = Sprite::create(SSCFileUtility::getStoragePath()+"/normal_dec.png");
#endif
        m_pFinishPizza->addChild(pDec);
    }
    
    _finishAll();
}

#pragma mark ======== finish ========================
void MakeNormalPizzaScene::_finishAdd()
{
    AudioHelp::getInstance()->playEffect("vo_mix.mp3");
    _showWhisk();
}

void MakeNormalPizzaScene::_finishMix()
{
    _removeWhisk();
    _createRollUI();
    m_pDough->setOpacity(0);
    m_pFlourSprinkle->setOpacity(0);
    m_pDoughPan->setVisible(false);
    m_pBowl->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width, 50)));
    m_pMixture->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width, 50)));
    
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        _showSpreadFlour();
    }),DelayTime::create(4),
                                     CallFunc::create([=](){
        _pourDough();
    }),
                                     DelayTime::create(2),
                                     CallFunc::create([=](){
        _showRollTool();
    }), NULL));
}

void MakeNormalPizzaScene::_finishRoll()
{
    m_pDoughGrid->stopAllActions();
    m_pPan = Sprite::create(localPath("make/pan.png"));
    this->addChildToContentLayer(m_pPan);
    CMVisibleRect::setPosition(m_pPan, 300-visibleSize.width, 320);
    m_pPan->setScale(0.7);
    
    m_pDragDough = _createDrageNode(localPath("make/dough2.png"));
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

void MakeNormalPizzaScene::_finishBrushOil()
{
    m_pOilBrush->setTouchEnabled(false);
    m_pOilBrush->setMovable(false);
    m_pSauceSpoon->setTouchEnabled(true);
    m_pSauceSpoon->setMovable(true);
    m_pOilBrush->back();
    
    auto action = Sequence::create(DelayTime::create(0.8),
                                   EaseBackIn::create(MoveBy::create(0.5, Vec2(visibleSize.width, 0))),
                                   CallFuncN::create([=](Node* pNode){
        pNode->removeFromParent();
    }), NULL);
    m_pOilPlate->runAction(action);
    m_pOilBrush->runAction(action->clone());
    AudioHelp::getInstance()->playEffect("vo_add_sauce.mp3");
}

void MakeNormalPizzaScene::_finishBrushSauce()
{
    m_pSauceSpoon->setTouchEnabled(false);
    m_pSauceSpoon->back();
    auto action = Sequence::create(DelayTime::create(0.8),
                                   EaseBackIn::create(MoveBy::create(0.5, Vec2(visibleSize.width, 0))),
                                   CallFuncN::create([=](Node* pNode){
        pNode->removeFromParent();
    }), NULL);
    m_pSaucePlate->runAction(action->clone());
    m_pSauceSpoon->runAction(action->clone());
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        _createCheeseUI();
        m_pOilBrush = nullptr;
        m_pSauceSpoon = nullptr;
    }), NULL));
}


void MakeNormalPizzaScene::_finishCheeseSlice()
{
    m_pGrater->stopAllActions();
    m_pGrater->runAction(Sequence::create(DelayTime::create(0.5),
                                          MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                          CallFunc::create([=](){
        m_pGrater->removeFromParent();
        m_pGrater = nullptr;
    }), NULL));
    log("======================== _finishCheeseSlice");
    _savePizza();
    log("======================== _savePizza");
    auto action = Sequence::create(DelayTime::create(1.5),
                                   MoveBy::create(0.5, Vec2(350, 0)), NULL);
    m_pPan->runAction(action);
    m_pDoughGrid->runAction(action->clone());
    m_pOilScribble->runAction(action->clone());
    m_pSauceScribble->runAction(action->clone());
    m_pCheeseSlice->runAction(action->clone());
    this->runAction(Sequence::create(DelayTime::create(2.2),
                                          CallFunc::create([=](){
        _showDecorations();
    }),
                                     DelayTime::create(2),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("done.mp3");
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
        this->addChildToUILayer(pParticle);
        pParticle->setLocalZOrder(10);
        pParticle->setPosition(visibleSize*0.5);
        
        m_pGameUI->showNextLayout();
    }), NULL));
}

void MakeNormalPizzaScene::_finishDecorate()
{
    AudioHelp::getInstance()->playEffect("vo_great_combination.mp3");
    _removeDecorations();
    
    auto action = Sequence::create(DelayTime::create(1),
                                   MoveBy::create(0.5, Vec2(-380, 0)), NULL);
    m_pPan->runAction(action);
    m_pDoughGrid->runAction(action->clone());
    m_pOilScribble->runAction(action->clone());
    m_pSauceScribble->runAction(action->clone());
    m_pCheeseSlice->runAction(action->clone());
    m_pDecoration->runAction(action->clone());
    _savePizzaDecoration();
    _savePizzaDecorationRipe();
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        _showSholve();
    }), NULL));
}

void MakeNormalPizzaScene::_finishAll()
{
    m_pSholve->runAction(Sequence::create(Spawn::create(RotateBy::create(0.5, 45),
                                                        MoveBy::create(0.5, Vec2(0, -200)), NULL),
                                          CallFunc::create([=](){
        SceneManager::replaceTheScene<BakeNormalPizzaScene>(true,eMoveInR);
    }), NULL));
//    this->runAction(MoveBy::create(1,Vec2(-visibleSize.width,0)));
}

void MakeNormalPizzaScene::_savePizza()
{
    
    
    Size mixtureSize = m_pPan->getContentSize();
    RenderTexture* render = RenderTexture::create(mixtureSize.width, mixtureSize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    if (m_pCheeseSlice) {
        Vec2 posCheese = m_pCheeseSlice->getPosition();
        m_pCheeseSlice->setPosition(Vec2(mixtureSize.width/2, mixtureSize.height/2));
        
        render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
        m_pCheeseSlice->visit();
        render->end();
        m_pCheeseSlice->setPosition(posCheese);
    }
    
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    Image* pImage = render->newImage();
    
    bool issuccess;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    issuccess = pImage->saveToFile(SSCFileUtility::getStoragePath()+"normal_cheese.png", false);
#else
    issuccess = pImage->saveToFile(SSCFileUtility::getStoragePath()+"/normal_cheese.png", false);
#endif
    pImage->autorelease();

}

void MakeNormalPizzaScene::_savePizzaDecoration()
{
    Size mixtureSize = m_pPan->getContentSize();
    RenderTexture* render = RenderTexture::create(mixtureSize.width, mixtureSize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    if (m_pDecoration){
        Vec2 pos = m_pDecoration->getPosition();
        m_pDecoration->setPosition(Vec2(mixtureSize.width/2, mixtureSize.height/2));
        
        render->begin();
        m_pDecoration->visit();
        render->end();
        
        m_pDecoration->setPosition(pos);
    }
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    Image* pImage = render->newImage();
    
    bool issuccess;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    issuccess = pImage->saveToFile(SSCFileUtility::getStoragePath()+"normal_dec.png", false);
#else
    issuccess = pImage->saveToFile(SSCFileUtility::getStoragePath()+"/normal_dec.png", false);
#endif
    pImage->autorelease();
}

void MakeNormalPizzaScene::_savePizzaDecorationRipe()
{
    Size mixtureSize = m_pPan->getContentSize();
    RenderTexture* render = RenderTexture::create(mixtureSize.width, mixtureSize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    if (m_pDecorationRipe) {
        Vec2 pos = m_pDecorationRipe->getPosition();
        m_pDecorationRipe->setPosition(Vec2(mixtureSize.width/2, mixtureSize.height/2));
        
        render->begin();
        m_pDecorationRipe->visit();
        render->end();
        m_pDecorationRipe->setPosition(pos);
    }
    Director::getInstance()->getRenderer()->render();
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    Image* pImage = render->newImage();
    
    bool issuccess;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    issuccess = pImage->saveToFile(SSCFileUtility::getStoragePath()+"normal_decripe.png", false);
#else
    issuccess = pImage->saveToFile(SSCFileUtility::getStoragePath()+"/normal_decripe.png", false);
#endif
    pImage->autorelease();
}

void MakeNormalPizzaScene::saveImage2(){
//    Rect rect = _pDragNode->getBoundingBox();
//    if (rect.containsPoint(m_pGrater->getPosition())) {
//        if (m_bGraterReady) {
//            _sliceCheese();
//        }else{
//            m_bGraterReady = true;
//            Vec2 location = m_pPan->getPosition();
//            Vec2 pos = Vec2(location.x, location.y+80);
//            m_pGrater->runAction(RepeatForever::create(Sequence::create(CircleMove::create(20, pos, 150, 360*6),
//                                                                        NULL)));
//        }
//    }
    _sliceCheese();
    
    
}

void MakeNormalPizzaScene::saveImage1(){
    
    m_btn->setTouchEnabled(false);
    _finishDecorate();
    return;
    
}


void MakeNormalPizzaScene::onPermissionGrantedResult(int requestCode,bool bGranted){
    if (requestCode == 1) {
        if (bGranted) {
            this->runAction(Sequence::create(DelayTime::create(0.0f),
                                             CallFunc::create([=] {
                saveImage1();
            }),NULL))    ;
            log("-------->anroid runtime permisson was granted,requestcode = %d",requestCode);
        }else{
            //add your code....
            log("-------->anroid runtime permisson was not  granted,requestcode = %d",requestCode);
        }
    }
    if (requestCode == 2) {
        
        if (bGranted) {
            this->runAction(Sequence::create(DelayTime::create(0.0f),
                                             CallFunc::create([=] {
                hasPermission = true;
                saveImage2();
            }),NULL))    ;
            log("-------->anroid runtime permisson was granted,requestcode = %d",requestCode);
        }else{
            //add your code....
            log("-------->anroid runtime permisson was not  granted,requestcode = %d",requestCode);
        }
    }
}
