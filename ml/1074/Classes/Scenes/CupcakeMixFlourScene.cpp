
#include "CupcakeMixFlourScene.h"
#include "SceneManager.h"

CupcakeMixFlourScene::CupcakeMixFlourScene()
{
    m_nStep = 0;
    m_nFlourAdd = 0;
    m_nShakeCount = 0;
    m_nShakeAdd = 0;
    m_nFlavorIndex = -1;
    m_nMixStep = 0;
    m_bItemEnable = false;
    m_pBottle = nullptr;
    m_nEggCount = 0;
//    m_pDecorationScrollView = nullptr;
}

CupcakeMixFlourScene::~CupcakeMixFlourScene()
{
    
}
bool CupcakeMixFlourScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("common/bg.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        _showBowl();
        _showIngredientsStep(0);
//        _showMixTools();
        AudioHelp::getInstance()->playEffect("vo_add_ingredients.mp3");
    }),
                                     DelayTime::create(3),
                                     CallFunc::create([=](){
        //        m_pGuideLayer->showGuideMove(m_pFlour->getPosition(), m_pBowl->getPosition());
        //        m_pGuideLayer->showGuideRotateLeft(m_pFlour->getPosition()+Vec2(-100, -100), 0.2);
    }), NULL));
    
    m_pTouchLayer = TouchLayer::create();
    this->addChildToUILayer(m_pTouchLayer);
    m_pTouchLayer->onTouchEnded = CC_CALLBACK_2(CupcakeMixFlourScene::TouchEnded, this);
    m_pTouchLayer->setEnable(false);
    
    m_pGameUI->showNormalLayout();
    return true;
}

void CupcakeMixFlourScene::onEnter()
{
    ExtensionScene::onEnter();
}

void CupcakeMixFlourScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void CupcakeMixFlourScene::onButtonCallback(Button* btn)
{
    int tag = btn->getTag();
    if (tag == GameUILayoutLayer::eUIButtonTagNext) {
//        btn->setTouchEnabled(false);
//        btn->setVisible(false);
        if(!m_bItemEnable)
        {
            return;
        }
        m_pGameUI->hideNext();
        
        GameDataManager::getInstance()->m_sFlavorStr = GameDataManager::getInstance()->m_sFlavorStr+","+std::to_string(0);
        m_pGuideLayer->removeGuide();
        if(m_pBottle){
            m_pBottle->setTouchEnabled(false);
            Device::setAccelerometerEnabled(false);
            DragNode* pPreBottle = m_pBottle;
            pPreBottle->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                   CallFunc::create([=](){
                pPreBottle->removeFromParent();
            }), NULL));
            m_pBottle = nullptr;
        }
        
        m_pScrollView->runAction(Sequence::create(MoveBy::create(1, Vec2(0, -500)),
                                                  CallFunc::create([=](){
            m_pScrollView->removeFromParent();
            m_pScrollView = nullptr;
        }), NULL));
        m_pBanner->runAction(Sequence::create(MoveBy::create(1, Vec2(0, -500)),
                                              CallFunc::create([=](){
            m_pBanner->removeFromParent();
            m_pBanner = nullptr;
        }), NULL));
        
        if (m_nMixStep<3) {
            m_pBowlFront->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                     CallFunc::create([=](){
                CMVisibleRect::setPositionAdapted(m_pBowlFront, 320-visibleSize.width, 380);
            }),
                                                     MoveBy::create(1.0,Vec2(visibleSize.width,0)), NULL));
            m_pBowl->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                CallFunc::create([=](){
                CMVisibleRect::setPositionAdapted(m_pBowl, 320-visibleSize.width, 380);
                m_pBatter->removeAllChildren();
                Sprite* pAdd = Sprite::create(localPath("stir_5.png"));
                if (pAdd) {
                    m_pBatter->addChild(pAdd);
                }
            }),
                                                MoveBy::create(1.0,Vec2(visibleSize.width,0)),
                                                CallFunc::create([=](){
                _showFlavorScrollView();
            }), NULL));
            m_nMixStep++;
        }else{
            _finish();
        }
        return;
    }
    ExtensionScene::onButtonCallback(btn);
}
void CupcakeMixFlourScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if (name == "ingredient"){
        int tag = pDragNode->getTag();
        IngredientManager::getInstance()->showShadow(tag,false);
        if (tag == 8) {
            pDragNode->setDragSprite(Sprite::create(localPath("flour_spoon.png")));
            ContainerNode* pNode = (ContainerNode*)pDragNode->getUserData();
            if (!pNode) {
                pNode = ContainerNode::create(pDragNode);
            }
            pNode->addFood(localPath("flour_spoon1.png"));
            pDragNode->setUserData(pNode);
        }
    }else if (name == "tool_normal"){
        int index = pDragNode->index;
        if (index == 0) {
            pDragNode->index = 1;
            m_pCurTool = pDragNode;
            
            AudioHelp::getInstance()->playEffect("drag_corn.mp3");
            
            pDragNode->setTouchEnabled(false);
            pDragNode->runAction(Sequence::create(Spawn::create(RotateBy::create(1, -90),
                                                                MoveTo::create(1, Vec2(visibleSize.width/2, m_pBowl->getPositionY()+50)), NULL),
                                                  CallFunc::create([=](){
                pDragNode->setRotation(0);
                pDragNode->setDragSprite(Sprite::create("content/make/common/mix/agitator1_2.png"));
                
                Rect rect = m_pBowl->getBoundingBox();
                rect.origin.x += 100;
                rect.origin.y += 100;
                rect.size = Size(200, 100);
                pDragNode->setLimitRect(rect);
                
                pDragNode->setTouchEnabled(true);
                AudioHelp::getInstance()->_playEffectGoodChooice();
                _showProgressStar();
            }), NULL));
            pDragNode->setActionPoint(Vec2(61, 72));
            m_pMixTool_electric->setTouchEnabled(false);
            m_pMixTool_electric->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
        }else{
            pDragNode->setLocalZOrder(8);
        }
    }else if (name == "tool_electric"){
        int index = pDragNode->index;
        if (index == 0){
            if (!IAPManager::getInstance()->getItemIsBought(2)) {
                if (!ConfigManager::getInstance()->getVideoUnLocked("cupcake_classic_mixtool", 1)){
                    ShopMiniLayer* pLayer = ShopMiniLayer::create(false);
                    this->addChildToUILayer(pLayer);
                    pLayer->setLocalZOrder(100);
                    pDragNode->setTouchEnabled(false);
                    pLayer->shopDismissed = [=](){
                        if (IAPManager::getInstance()->getItemIsBought(2)) {
                            Node* pLock = pDragNode->getChildByName(LOCK_NAME_REWARD);
                            if (pLock) {
                                pLock->setVisible(false);
                            }
                        }
                        
                        pDragNode->setTouchEnabled(true);
                    };
                    pLayer->shopGetFree = [=](){
                        pDragNode->setTouchEnabled(true);
                        LockUIManager::getInstance()->onRewardSuccess = [=](){
                            AudioHelp::getInstance()->playEffect("bling.mp3");
                            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/goodParticle.plist");
                            pDragNode->addChild(pParticle,100);
                            pParticle->setPosition(Vec2::ZERO);
                        };
                        RewardManager::getInstance()->showRewardAds("cupcake_classic_mixtool", 1);
                        pLayer->removeFromParent();
                    };
                    return;
                }
            }
            pDragNode->index = 1;
            m_pCurTool = pDragNode;
            AudioHelp::getInstance()->playEffect("drag_corn.mp3");
            
            pDragNode->setTouchEnabled(false);
            pDragNode->runAction(Sequence::create(Spawn::create(RotateBy::create(1, -30),
                                                                MoveTo::create(1, Vec2(visibleSize.width/2+50, m_pBowl->getPositionY()+150)), NULL),
                                                  CallFunc::create([=](){
                pDragNode->setRotation(0);
                pDragNode->setDragSprite(Sprite::create("content/make/common/mix/agitator2_1.png"));
                
                Sprite* pStick = Sprite::create("content/make/common/mix/agitator2_2.png");
                pDragNode->addChild(pStick,-1);
                pStick->setName("stick");
                
                Rect rect = m_pBowl->getBoundingBox();
                rect.origin.x += 100;
                rect.origin.y += 100;
                rect.size = Size(200, 100);
                pDragNode->setLimitRect(rect);
                
                pDragNode->setTouchEnabled(true);
                
                AudioHelp::getInstance()->_playEffectGoodChooice();
                _showProgressStar();
                _toolElectricWork();
            }), NULL));
            pDragNode->setActionPoint(Vec2(59, 39));
            
            m_pMixTool_normal->setTouchEnabled(false);
            m_pMixTool_normal->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
        }else{
            pDragNode->setLocalZOrder(8);
            _toolElectricWork();
        }
    }
}

void CupcakeMixFlourScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setScale(1);
    std::string name = pDragNode->getName();
    if (name == "tool_normal" || name == "tool_electric"){
        int index = pDragNode->index;
        if(index == 1){
            pDragNode->setLocalZOrder(8);
            Rect bowlRect = m_pBowl->getBoundingBox();
            if (bowlRect.containsPoint(worldPoint)) {
                if (name == "tool_electric"){
                    AudioHelp::getInstance()->playLoopEffect("stir_machine1.mp3");
                }else{
                    AudioHelp::getInstance()->playLoopEffect("stir_spoon.mp3");
                }
                MixManager::getInstance()->mix();
            }else{
                AudioHelp::getInstance()->stopLoopEffect();
                m_pBowl->stopAllActions();
            }
        }
    }
    
}

void CupcakeMixFlourScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    std::string name = pDragNode->getName();
    if (name == "ingredient") {
        int tag = pDragNode->getTag();
        Rect rectBowl = m_pBowl->getBoundingBox();
        if (tag == 8) {//flour
            rectBowl = m_pSieve->getWorldRect();
        }
        Rect rectIngredient = pDragNode->getDragSprite()->getBoundingBox();
        rectIngredient.origin = pDragNode->convertToWorldSpace(rectIngredient.origin);
        if (rectBowl.intersectsRect(rectIngredient)) {
            m_pGuideLayer->removeGuide();
            _addIngredient(pDragNode);
        }else{
            pDragNode->setTouchEnabled(false);
            pDragNode->back(pDragNode->getTag(),[=](){
                IngredientManager::getInstance()->showShadow(pDragNode->getTag(),true);
                pDragNode->setTouchEnabled(true);
            });
            if (tag == 8) {//flour
                pDragNode->runAction(ScaleTo::create(0.5, 0.6));
            }
        }
        
    }else if (name == "tool_normal" || name == "tool_electric"){
        int index = pDragNode->index;
        if(index == 1){
            AudioHelp::getInstance()->stopLoopEffect();
            m_pBowl->stopAllActions();
        }
        _toolElectricWork(false);
    }else if (name == "bottle") {
        pDragNode->setTouchEnabled(false);
        _addFlavor();
    }
}

void CupcakeMixFlourScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    dragNodeTouchEnded(pDragNode,worldPoint);
}

void CupcakeMixFlourScene::TouchEnded(Touch *pTouch, Event *pEvent)
{
    Rect rectBowl = m_pSieve->getWorldRect();
    if (!rectBowl.containsPoint(pTouch->getLocation())){
        return;
    }
    
    Device::setAccelerometerEnabled(false);
    m_pTouchLayer->setEnable(false);
    m_pSieve->separatePowder();
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        _separatePowder();
        m_nShakeCount = m_nShakeAdd*100;
        m_pSieve->stopSeparate();
        
        if (m_nShakeAdd<4){
            Device::setAccelerometerEnabled(true);
            m_pTouchLayer->setEnable(true);
        }
    }), NULL));
}

void CupcakeMixFlourScene::onAcceleration(Acceleration* acc, Event* event){
    
    log("=================%f,%f,%f",acc->x,acc->y,acc->z);
    if(m_nStep==8){
        if (abs(acc->x)>0.3 ) {
            m_pSieve->separatePowder();
            m_nShakeCount++;
            if(m_nShakeCount%10==0){
                Device::setAccelerometerEnabled(false);
                m_pTouchLayer->setEnable(false);
            }
            this->runAction(Sequence::create(DelayTime::create(1),
                                             CallFunc::create([=](){
                m_pSieve->stopSeparate();
                if (m_nShakeAdd<4){
                    Device::setAccelerometerEnabled(true);
                    m_pTouchLayer->setEnable(true);
                }
            }), NULL));
            if(m_nShakeCount%40==0){
                _separatePowder();
            }
        }
        
    }else if(m_nStep>8){
        if (acc->x<0 ){
            m_pBottle->setRotation(100*acc->x);
        }
        if (acc->x<-0.5 ) {
            m_pBottle->setTouchEnabled(false);
            Device::setAccelerometerEnabled(false);
            _addFlavor();
        }
    }
}
#pragma mark - initData
void CupcakeMixFlourScene::_initData()
{
    setExPath("content/make/cupcake/make1/");
}

void CupcakeMixFlourScene::_showBowl()
{
    m_pBowl = Sprite::create(localPath("bowl_shadow.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 320-visibleSize.width, 380);
    m_pBowl->runAction(MoveBy::create(1.0,Vec2(visibleSize.width,0)));
    
    Sprite* pBowl = Sprite::create(localPath("bowl_down.png"));
    m_pBowl->addChild(pBowl);
    pBowl->setPosition(m_pBowl->getContentSize()*0.5);
    
    m_pBowlFront = Sprite::create(localPath("bowl_on.png"));
    this->addChildToContentLayer(m_pBowlFront);
    m_pBowlFront->setLocalZOrder(10);
    m_pBowlFront->setPosition(m_pBowl->getPosition());
    m_pBowlFront->runAction(MoveBy::create(1.0,Vec2(visibleSize.width,0)));
    
    m_pBatterClipping = ClippingNode::create(Sprite::create(localPath("bowl_mask.png")));
    m_pBowl->addChild(m_pBatterClipping);
    m_pBatterClipping->setAlphaThreshold(0.5);
    m_pBatterClipping->setPosition(Vec2(m_pBowl->getContentSize().width*0.5,m_pBowl->getContentSize().height*0.5));
    
    m_pBatter = Node::create();
    m_pBatterClipping->addChild(m_pBatter);
    
    MixManager::getInstance()->reset();
    MixManager::getInstance()->_mixtureStartIndex = 0;
    MixManager::getInstance()->_mixCount = 5;
    MixManager::getInstance()->_mixSpeed = 200;
    MixManager::getInstance()->_mixRotateSpeed = 0;
    MixManager::getInstance()->_mixturePathEX = localPath("stir_");
    MixManager::getInstance()->_mixtureNode = m_pBatter;
    MixManager::getInstance()->_mixtureSize = m_pBowl->getContentSize();
    MixManager::getInstance()->onMixCallback = [=](int index){
        if (index==2) {
            AudioHelp::getInstance()->playEffect("vo_keep_stirring.mp3");
        }
    };
    MixManager::getInstance()->onScheduleCallback = [=](int index){
        m_pProgressStar->addStar();
    };
    MixManager::getInstance()->onMixFinish = CC_CALLBACK_0(CupcakeMixFlourScene::_finishMix, this);
}

void CupcakeMixFlourScene::_showMixTools()
{
    if (m_pContentLayer->getChildByName("tool_normal")) {
        m_pContentLayer->removeChildByName("tool_normal");
    }
    if (m_pContentLayer->getChildByName("tool_electric")) {
        m_pContentLayer->removeChildByName("tool_electric");
    }
    DragNode* pMixTool_normal = _createDrageNode("content/make/common/mix/agitator1.png");
    this->addChildToContentLayer(pMixTool_normal);
    CMVisibleRect::setPositionAdapted(pMixTool_normal, 480+visibleSize.width, 720);
    pMixTool_normal->setName("tool_normal");
    pMixTool_normal->setTouchEnabled(false);
    
    DragNode* pMixTool_electric = _createDrageNode("content/make/common/mix/agitator2.png");
    this->addChildToContentLayer(pMixTool_electric);
    CMVisibleRect::setPositionAdapted(pMixTool_electric, 210+visibleSize.width, 770);
    pMixTool_electric->setName("tool_electric");
    pMixTool_electric->setTouchEnabled(false);
    
    if (!IAPManager::getInstance()->getItemIsBought(2)) {
        if (!ConfigManager::getInstance()->getVideoUnLocked("cupcake_classic_mixtool", 1)){
            LockUIManager::getInstance()->registerLock("cupcake_classic_mixtool", 1, pMixTool_electric);
            LockUIManager::getInstance()->setLockPos(Vec2(50, -50));
        }
    }
    
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    AudioHelp::getInstance()->playEffect("vo_try_automatic.mp3");
    pMixTool_normal->setTag(0);
    pMixTool_electric->setTag(1);
    
    auto action = Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                   CallFuncN::create([=](Node* pNode){
        DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
        if (pIngredient) {
            pIngredient->setOrgPositionDefault();
            pIngredient->setTouchEnabled(true);
            if (pNode->getName() == "tool_electric"){
                if (ConfigManager::getInstance()->getVideoUnLocked("cupcake_classic_mixtool", 1)) {
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dec.plist");
                    pMixTool_electric->addChild(pParticle,20);
                    pParticle->setPosition(Vec2::ZERO);
                }
            }
        }
    }), NULL);
    pMixTool_normal->runAction(action);
    pMixTool_electric->runAction(action->clone());
    
    m_pMixTool_normal = pMixTool_normal;
    m_pMixTool_electric = pMixTool_electric;
}

void CupcakeMixFlourScene::_showIngredientsStep(int step)
{
    if(step==0) {
        _showIngredient(0);
        _showIngredient(1);
        _showIngredient(2);
        AudioHelp::getInstance()->playEffect("present_in.mp3");
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("vo_milk_salt.mp3");
        }),
                                         DelayTime::create(4.5),
                                         CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("vo_yeast.mp3");
        }), NULL));
    }else if (step == 1) {
        _showIngredient(3);
        _showIngredient(4);
        _showIngredient(5);
        _showIngredient(6);
        _showIngredient(7);
        AudioHelp::getInstance()->playEffect("present_in.mp3");
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("vo_sugar_butter_eggs.mp3");
        }), NULL));
    }else if(step == 2){
        _showSieve();
        _showIngredient(8);
        AudioHelp::getInstance()->playEffect("present_in.mp3");
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("vo_using_sieve.mp3");
        }), NULL));
    }else if(step == 3){
        _finishAdd();
    }else if(step == 4){
        _showFlavorScrollView();
    }else if(step == 5){
        AudioHelp::getInstance()->playEffect("vo_mix_ingredients.mp3");
        _showMixTools();
    }
    
}

void CupcakeMixFlourScene::_showIngredient(int index)
{
    //    back shadow front food food2
    static std::string sIngredents[][5] ={
        {"milk.png","milk_1.png"},  //milk
        {"salt.png","salt_1.png"}, //salt
        {"baking powder.png","baking powder_shadow.png"}, //powder
        {"sugar1.png","sugar1_2.png","sugar1_1.png"},  //sugar
        {"butter_plate.png","butter_2.png","","butter_1.png"}, //butter
        {"egg.png","egg_shadow.png"},  //egg
        {"egg.png","egg_shadow.png"},  //egg
        {"egg.png","egg_shadow.png"},  //egg
        {"flour_spoon2.png"},  //flour
        {"flour_down.png","","flour_on.png","flour1.png"},  //flour
        {},
    };
    
    Vec2 actionPoint[] = {
        Vec2(20, 185),
        Vec2(55, 157),
        Vec2(9, 183),
        Vec2(11, 160),
        Vec2(31, 63),
        Vec2(0, 0),
        Vec2(0, 0),
        Vec2(0, 0),
        Vec2(27, 33),
        Vec2(38, 44),
    };
    Vec2 pos[] = {
        Vec2(120, 800),
        Vec2(320, 760),
        Vec2(500, 780),
        Vec2(320, 780),
        Vec2(160, 730),
        Vec2(480, 750),
        Vec2(570, 750),
        Vec2(520, 730),
        Vec2(480, 850),
        Vec2(480, 850),
    };
    DragNode* pDragNode = _createDrageNode(localPath(sIngredents[index][0]));
    this->addChildToContentLayer(pDragNode);
    pDragNode->setTag(index);
    pDragNode->setLocalZOrder(index);
    pDragNode->setName("ingredient");
    pDragNode->setTouchEnabled(true);
    pDragNode->setActionPoint(actionPoint[index]);
    CMVisibleRect::setPositionAdapted(pDragNode, pos[index]+Vec2(visibleSize.width,0));
    
    IngredientManager::getInstance()->addIngredient(pDragNode);
    IngredientManager::getInstance()->addContainer(pDragNode,localPath(""), sIngredents[index]);
    
    auto action = Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                   CallFuncN::create([=](Node* pNode){
        DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
        if (pIngredient) {
            pIngredient->setOrgPositionDefault();
            pIngredient->setTouchEnabled(true);
        }
    }), NULL);
    pDragNode->runAction(action);
    if (index == 8) {
        m_pFlourBag = Node::create();
        this->addChildToContentLayer(m_pFlourBag);
        Sprite* pBody = Sprite::create(localPath(sIngredents[index+1][0]));
        m_pFlourBag->addChild(pBody);
        
        CMVisibleRect::setPositionAdapted(m_pFlourBag, pos[index]+Vec2(visibleSize.width,-70));
        IngredientManager::getInstance()->addContainer(m_pFlourBag,localPath(""), sIngredents[index+1]);
        m_pFlourBag->runAction(action->clone());
    }
}

void CupcakeMixFlourScene::_showSieve()
{
    m_pSieve = SieveNode::create();
    this->addChildToContentLayer(m_pSieve);
    CMVisibleRect::setPositionAdapted(m_pSieve, 330-visibleSize.width, 630);
    m_pSieve->runAction(MoveBy::create(1.0,Vec2(visibleSize.width,0)));
    m_pSieve->setLocalZOrder(1);
    
    auto eventAcc = EventListenerAcceleration::create(CC_CALLBACK_2(CupcakeMixFlourScene::onAcceleration, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventAcc, this);
}

void CupcakeMixFlourScene::_showFlavorScrollView()
{
    m_nFlavorIndex = -1;
    m_typeItems.clear();
    m_pScrollView = ExpandScrollView::create();
    m_pScrollView->setContentSize(Size(visibleSize.width,200));
    this->addChildToUILayer(m_pScrollView);
    CMVisibleRect::setPositionAdapted(m_pScrollView, 0, 120-600,kBorderLeft,kBorderBottom);
    m_pScrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    m_pScrollView->setLocalZOrder(5);
    m_pScrollView->setScrollBarEnabled(false);
    m_pScrollView->setClippingEnabled(false);
    
    int typeCount = 14;
    m_pScrollView->getInnerContainer()->setContentSize(Size((typeCount+1)*100, 200));
    
    m_pBanner = Sprite::create("content/category/icon/fruit_banner.png");
    this->addChildToUILayer(m_pBanner);
    m_pBanner->setPosition(m_pScrollView->getPosition()+m_pBanner->getAnchorPointInPoints());
    
    for (int i = 0; i<typeCount; i++) {
        LockItem* pItem = LockItem::create();
        
        bool bLock = false;
        if (!gNoneIap && !IAPManager::getInstance()->getItemIsBought(2) && i%2==1){
            if (!ConfigManager::getInstance()->getVideoUnLocked("cupcake_flavor", i)) {
                LockUIManager::getInstance()->registerLock("cupcake_flavor", i, pItem);
                LockUIManager::getInstance()->setLockOriginPos(Vec2(40, -60));
                pItem->lockType = 1;
                bLock = true;
            }
        }
        pItem->initWithName("content/make/common/flavor/bottle/color"+std::to_string(i+1)+"_1.png", "content/common/video.png",bLock);
        Sprite* Lock = pItem->getLockSprite();
        if (Lock) {
            Lock->setTag(TAG_REWARD_LOCK);
        }
        
        Sprite* pCap = Sprite::create("content/make/common/flavor/bottle/color"+std::to_string(i+1)+"_2.png");
        pCap->setAnchorPoint(Vec2(0.5, 0));
        pCap->setPosition(Vec2(0, 0));
        pItem->addChild(pCap);
        
        pItem->setTag(i);
        m_pScrollView->getInnerContainer()->addChild(pItem);
        pItem->setPosition(Vec2(100+i*100,90));
        pItem->setScale(0.6);
        
        pItem->onItemClicked = CC_CALLBACK_1(CupcakeMixFlourScene::_onTypeClicked, this);
        m_typeItems.pushBack(pItem);
    }
    m_pScrollView->runAction(Sequence::create(MoveBy::create(1, Vec2(0, 600)),
                                              CallFunc::create([=](){
        m_bItemEnable = true;
        if (m_nMixStep==1) {
            AudioHelp::getInstance()->playEffect("vo_dye_cake_mixture_colors.mp3");
        }else{
            AudioHelp::getInstance()->playEffect("vo_more_color.mp3");
            this->runAction(Sequence::create(DelayTime::create(3),
                                             CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("vo_next_button_skip .mp3");
            }), NULL));
            m_pGameUI->showNextLayout();
        }
    }), NULL));
    AudioHelp::getInstance()->playEffect("slide_sight.mp3");
    m_pBanner->runAction(MoveBy::create(1, Vec2(0, 600)));
}


void CupcakeMixFlourScene::_showBottle(int index)
{
    m_pGuideLayer->removeGuide();
    m_nFlavorIndex = index;
    
    if(m_pBottle){
        m_pBottle->setTouchEnabled(false);
        Device::setAccelerometerEnabled(false);
        DragNode* pPreBottle = m_pBottle;
        pPreBottle->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                              CallFunc::create([=](){
            pPreBottle->removeFromParent();
        }), NULL));
    }
    m_pBottle = _createDrageNode("content/make/common/flavor/bottle/color"+std::to_string(index+1)+"_1.png");
    this->addChildToContentLayer(m_pBottle);
    CMVisibleRect::setPositionAdapted(m_pBottle, 450+visibleSize.width, 580);
    m_pBottle->setTouchEnabled(false);
    m_pBottle->setMovable(false);
    m_pBottle->setName("bottle");
    m_pBottle->setActionPoint(Vec2(49,192));
    
    KettleNode* pContainer = KettleNode::create(m_pBottle);
    pContainer->addFront("content/make/common/flavor/bottle/color"+std::to_string(index+1)+"_2.png","front",Vec2(0, 120));
    m_pBottle->setUserData(pContainer);
    
    m_pBottle->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                          CallFunc::create([=](){
        m_pGuideLayer->showGuideRotateLeft(m_pBottle->getPosition());
        m_bItemEnable = true;
        m_pBottle->setTouchEnabled(true);
        Device::setAccelerometerEnabled(true);
    }), NULL));
}

void CupcakeMixFlourScene::_showProgressStar()
{
    m_pProgressStar = ProgressStar::create();
    this->addChildToUILayer(m_pProgressStar);
    CMVisibleRect::setPositionAdapted(m_pProgressStar, 320, 850+visibleSize.height);
    m_pProgressStar->runAction(EaseBackOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height))));
}

void CupcakeMixFlourScene::_addIngredient(DragNode* pIngredient)
{
    static string sBowlIn[] = {
        "bowl_milk.png",
        "bowl_salt.png",
        "bowl_baking powder.png",
        "bowl_sugar.png",
        "bowl_butter.png",
        "bowl_egg.png",
        "bowl_egg.png",
        "bowl_egg.png",
        "",
    };
    int index = pIngredient->getTag();
    IngredientManager::getInstance()->frozeIngredients(index);
    int k = index;
    std::string lNameStr = sBowlIn[k];
    float dt = 2;
    Vec2 posAdd = Vec2::ZERO;
    pIngredient->setTouchEnabled(false);
//    if (index < 1 || index >3) {
        this->runAction(Sequence::create(DelayTime::create(0.5),
                                         CallFunc::create([=](){
            pIngredient->setLocalZOrder(5);
        }), NULL));
//    }
    switch (index) {
#pragma mark =========  add milk ==========
        case 0:
        {
            posAdd = Vec2(0, 0);
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(60, 100)),
                                                    RotateTo::create(0.5,-90),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playPourMilkEffect();
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/milk.plist");
                pIngredient->addChild(pParticle);
                pParticle->setPosition(pIngredient->getActionPoint());
                pParticle->setName("particle");
                pParticle->setRotation(90);
                pParticle->setLocalZOrder(20);
                pParticle->setScaleX(-1);
                pParticle->setPosition(pIngredient->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                
                
                pIngredient->getDragSprite()->setTexture(localPath("milk_2.png"));
            }),
                                                    CircleMove::create(2,m_pBowl->getPosition()+Vec2(80, 150),60,180),
                                                    CallFunc::create([=](){
                auto particle = dynamic_cast<ParticleSystem*>(pIngredient->getChildByName("particle"));
                if (particle) {
                    particle->stopSystem();
                    particle->removeFromParent();
                }
                
            }),
                                                    RotateTo::create(1, 0),
                                                    CallFunc::create([=](){
                _ingredientOut(pIngredient);
                AudioHelp::getInstance()->playEffect("vo_good.mp3");
                
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                
                m_nStep++;
                if (m_nStep == 3) {
                    _showIngredientsStep(1);
                }
                MenuLayer::addIngredient(MenuLayer::ePackageCupcake, MenuLayer::eCupcake_milk);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                
                IngredientManager::getInstance()->removeIngredient(pIngredient);
                pIngredient->removeFromParent();
            }), NULL));
        }
            break;
#pragma mark =========  add salt ==========
        case 1:
        {
            posAdd = Vec2(-90, -20);
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(20, 120)),
                                                    RotateTo::create(0.5,-90),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("add_salt.mp3");
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/pour_sugar.plist");
                pIngredient->addChild(pParticle);
                pParticle->setPosition(pIngredient->getActionPoint());
                pParticle->setName("particle");
                pParticle->setRotation(90);
                pParticle->setLocalZOrder(20);
                pParticle->setPosition(pIngredient->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                
                
                ContainerNode* pContainer = (ContainerNode*)pIngredient->getUserData();
                if (pContainer) {
                    pContainer->pourFood();
                }
            }),
                                                    JumpBy::create(2,Vec2::ZERO,50,3),
                                                    CallFunc::create([=](){
                auto particle = dynamic_cast<ParticleSystem*>(pIngredient->getChildByName("particle"));
                if (particle) {
                    particle->stopSystem();
                    particle->removeFromParent();
                }
                
            }),
                                                    RotateTo::create(1, 0),
                                                    CallFunc::create([=](){
                _ingredientOut(pIngredient);
                AudioHelp::getInstance()->playEffect("vo_great.mp3");
                
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                
                m_nStep++;
                if (m_nStep == 3) {
                    _showIngredientsStep(1);
                }
                MenuLayer::addIngredient(MenuLayer::ePackageCupcake, MenuLayer::eCupcake_salt);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                IngredientManager::getInstance()->removeIngredient(pIngredient);
                pIngredient->removeFromParent();
            }), NULL));
        }
            break;
#pragma mark =========  add powder ==========
        case 2:
        {
            posAdd = Vec2(50, -20);
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(0, 100)),
                                                    RotateTo::create(0.5,-90),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playPourPowderEffect();
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/pour_sugar.plist");
                pIngredient->addChild(pParticle);
                pParticle->setPosition(pIngredient->getActionPoint());
                pParticle->setName("particle");
                pParticle->setRotation(90);
                pParticle->setLocalZOrder(20);
                pParticle->setPosition(pIngredient->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                
                
                ContainerNode* pContainer = (ContainerNode*)pIngredient->getUserData();
                if (pContainer) {
                    pContainer->pourFood();
                }
            }),
                                                    CircleMove::create(2,m_pBowl->getPosition()+Vec2(100, 180),60,180),
                                                    CallFunc::create([=](){
                auto particle = dynamic_cast<ParticleSystem*>(pIngredient->getChildByName("particle"));
                if (particle) {
                    particle->stopSystem();
                    particle->removeFromParent();
                }
                
            }),
                                                    RotateTo::create(1, 0),
                                                    CallFunc::create([=](){
                _ingredientOut(pIngredient);
                AudioHelp::getInstance()->playEffect("kids_laughing2.mp3");
                
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                
                m_nStep++;
                if (m_nStep == 3) {
                    _showIngredientsStep(1);
                }
                MenuLayer::addIngredient(MenuLayer::ePackageCupcake, MenuLayer::eCupcake_powder);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                IngredientManager::getInstance()->removeIngredient(pIngredient);
                pIngredient->removeFromParent();
            }), NULL));
        }
            break;
            
#pragma mark =========  add sugar ==========
        case 3:
        {
            posAdd = Vec2(40, -30);
            Node* pLid = pIngredient->getChildByName("front");
            if(pLid){
                AudioHelp::getInstance()->playEffect("bottle_open.mp3");
                pLid->runAction(Sequence::create(JumpBy::create(1, Vec2(100, 600), visibleSize.height, 1),
                                                 CallFunc::create([=](){
                    pLid->setVisible(false);
                }), NULL));
            }
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(30, 150)),
                                                    RotateTo::create(0.5,-90),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("add_sugar.mp3");
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/pour_sugar.plist");
                pIngredient->addChild(pParticle);
                pParticle->setPosition(pIngredient->getActionPoint());
                pParticle->setName("particle");
                pParticle->setRotation(90);
                pParticle->setLocalZOrder(20);
                pParticle->setPosition(pIngredient->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                
                
                pIngredient->getDragSprite()->setTexture(localPath("sugar2.png"));
            }),
                                                    MoveBy::create(2,Vec2(80, 50)),
                                                    CallFunc::create([=](){
                auto particle = dynamic_cast<ParticleSystem*>(pIngredient->getChildByName("particle"));
                if (particle) {
                    particle->stopSystem();
                    particle->removeFromParent();
                }
                
                pIngredient->getDragSprite()->setTexture(localPath("sugar1.png"));
                AudioHelp::getInstance()->playEffect("kids_laughing2.mp3");
            }),
                                                    RotateTo::create(1, 0),
                                                    CallFunc::create([=](){
                _ingredientOut(pIngredient);
                AudioHelp::getInstance()->playEffect("vo_good_job.mp3");
                
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("vo_sweet.mp3");
                
                m_nStep++;
                if (m_nStep == 8) {
                    _showIngredientsStep(2);
                }
                MenuLayer::addIngredient(MenuLayer::ePackageCupcake, MenuLayer::eCupcake_sugar);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                IngredientManager::getInstance()->removeIngredient(pIngredient);
                pIngredient->removeFromParent();
            }), NULL));
        }
            break;
#pragma mark =========  add butter ==========
        case 4:
        {
            posAdd = Vec2(0, -50);
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(60, 60)),
                                                    RotateTo::create(0.5,-60),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("add_buter.mp3");
                
                ContainerNode* pContainer = (ContainerNode*)pIngredient->getUserData();
                if (pContainer) {
                    pContainer->pourFood();
                }
            }),
                                                    MoveBy::create(1,Vec2(80, 100)),
                                                    MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(100, 100)),
                                                    RotateTo::create(1, 0),
                                                    CallFunc::create([=](){
                _ingredientOut(pIngredient);
                AudioHelp::getInstance()->playEffect("vo_fabulous.mp3");
                
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                
                m_nStep++;
                if (m_nStep == 8) {
                    _showIngredientsStep(2);
                }
                MenuLayer::addIngredient(MenuLayer::ePackageCupcake, MenuLayer::eCupcake_butter);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                IngredientManager::getInstance()->removeIngredient(pIngredient);
                pIngredient->removeFromParent();
            }), NULL));
        }
            break;
#pragma mark =========  add Egg ==========
        case 5:
        case 6:
        case 7:
        {
            dt = 4;
            posAdd = Vec2(-120+50*(index-3), 20-20*(index-3));
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(m_pBowl->getContentSize().width/2-40, 100)),
                                                    JumpBy::create(1,Vec2::ZERO,20,2),
                                                    CallFunc::create([=](){
                pIngredient->setVisible(false);
                
                Sprite* pEgg = Sprite::create(localPath("egg_1.png"));
                this->addChildToContentLayer(pEgg);
                pEgg->setPosition(pIngredient->getWorldSpaceActionPoint());
                pEgg->setLocalZOrder(20);
                
                pEgg->runAction(Sequence::create(MoveTo::create(0.5, m_pBowl->getPosition()+Vec2(50*(index-3)-120, 100)),
                                                 CallFunc::create([=](){
                    AudioHelp::getInstance()->playAddEggEffect();
                }),
                                                 DelayTime::create(1),
                                                 CallFunc::create([=](){
                    pEgg->setTexture(localPath("egg_2.png"));
                    if (m_nEggCount==2) {
                        AudioHelp::getInstance()->playEffect("vo_nice.mp3");
                    }
                }),
                                                 DelayTime::create(1),
                                                 CallFunc::create([=](){
                    
                    IngredientManager::getInstance()->frozeIngredients(index,false);
                    
                    m_nStep++;
                    if (m_nStep == 8) {
                        _showIngredientsStep(2);
                    }
                    m_nEggCount++;
                    if (m_nEggCount==3) {
                        MenuLayer::addIngredient(MenuLayer::ePackageCupcake, MenuLayer::eCupcake_egg);
                        MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                    }
                    IngredientManager::getInstance()->removeIngredient(pIngredient);
                    pIngredient->removeFromParent();
                    pEgg->removeFromParent();
                }), NULL));
            }), NULL));
        }
            break;
#pragma mark =========  add flour ==========
        case 8:
        {
            m_nFlourAdd++;
            pIngredient->setTouchEnabled(false);
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pSieve->getPosition()+Vec2(-30, 140)),
                                                    RotateTo::create(0.5,-30),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playPourPowderEffect();
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/pour_flour2.plist");
                pIngredient->addChild(pParticle);
                pParticle->setPosition(pIngredient->getActionPoint());
                pParticle->setName("particle");
                pParticle->setRotation(30);
                pParticle->setLocalZOrder(20);
                pParticle->setPosition(pIngredient->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                
                
                ContainerNode* pContainer = (ContainerNode*)pIngredient->getUserData();
                if (pContainer) {
                    pContainer->pourFood();
                }
                if (m_nFlourAdd==2){
                    AudioHelp::getInstance()->playEffect("kids_laughing3.mp3");
                }
            }),
                                                    MoveBy::create(2,Vec2(100, 30)),
                                                    //                                                    MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(50, 100)),
                                                    CallFunc::create([=](){
                auto particle = dynamic_cast<ParticleSystem*>(pIngredient->getChildByName("particle"));
                if (particle) {
                    particle->stopSystem();
                    particle->removeFromParent();
                }
                m_pSieve->setPowder("content/make/common/sleve/sieve_flour"+std::to_string(3-m_nFlourAdd)+".png");
                
            }),
                                                    RotateTo::create(1, 0),
                                                    CallFunc::create([=](){
                
                if (m_nFlourAdd==2) {
                    pIngredient->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
                    m_pFlourBag->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
//                    m_pGuideLayer->showGuideShake(m_pSieve->getPosition()+Vec2(0, 100));
                    m_pGuideLayer->showGuideTap(m_pSieve->getPosition());
                    m_pTouchLayer->setEnable(true);
                    Device::setAccelerometerEnabled(true);
                }else{
                    pIngredient->back(pIngredient->getTag(),[=](){
                        IngredientManager::getInstance()->showShadow(pIngredient->getTag(),true);
                        pIngredient->setDragSprite(Sprite::create(localPath("flour_spoon.png")));
                        ContainerNode* pNode = (ContainerNode*)pIngredient->getUserData();
                        if (!pNode) {
                            pNode = ContainerNode::create(pIngredient);
                        }
                        pNode->addFood(localPath("flour_spoon1.png"));
                        pIngredient->setUserData(pNode);
                        pIngredient->setTouchEnabled(true);
                    });
                    pIngredient->runAction(ScaleTo::create(0.5, 0.6));
                }
            }), NULL));
            return;
        }
            break;
        default:
            break;
    }
    Sprite* pAdd = Sprite::create(localPath(lNameStr));
    if (pAdd) {
        pAdd->setPosition(posAdd);
        pAdd->setOpacity(0);
        m_pBatter->addChild(pAdd,5);
        pAdd->runAction(Sequence::create(DelayTime::create(dt),
                                         FadeIn::create(1),
                                         CallFunc::create([=](){
            if (index == 0) {
                pAdd->setLocalZOrder(1);
            }
            if (index == 8) {
                pAdd->setLocalZOrder(2);
            }
            m_pGuideLayer->removeGuide();
        }), NULL));
    }
}


void CupcakeMixFlourScene::_ingredientOut(DragNode* pNode)
{
    AudioHelp::getInstance()->playEffect("ingredients_out.mp3");
    //    for (int i = 0; i<2; i++) {
    //        m_pBowl->runAction(Sequence::create(ScaleTo::create(0.17,1.05),
    //                                            ScaleTo::create(0.17,1.0),nullptr));
    //        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/heartParticle.plist");
    //        this->addChildToContentLayer(pParticle);
    //        pParticle->setPosition(m_pBowl->getPosition()+Vec2(-50, 50));
    //        pParticle->setLocalZOrder(20);
    //        if(i == 0){
    //            pParticle->setRotation(-30);
    //            pParticle->setPosition(m_pBowl->getPosition()+Vec2(-50, 120));
    //        }
    //    }
    IngredientManager::getInstance()->frozeIngredients(pNode->getTag(),false);
}

void CupcakeMixFlourScene::_toolElectricWork(bool working)
{
    if(!m_pMixTool_electric){
        return;
    }
    if (working) {
        auto action = m_pMixTool_electric->getActionByTag(1000);
        if (!action) {
            action = RepeatForever::create(ActionHelper::createShakeAction());
            action->setTag(1000);
            m_pMixTool_electric->runAction(action);
        }
        Sprite* pStick = (Sprite*)m_pMixTool_electric->getChildByName("stick");
        if (pStick) {
            auto action = pStick->getActionByTag(1000);
            if (!action) {
                action = RepeatForever::create(AnimationHelp::createAnimate("content/make/common/mix/agitator2_", 3, 5));
                action->setTag(1000);
                pStick->runAction(action);
            }
        }
    }else{
        m_pMixTool_electric->stopAllActions();
        Sprite* pStick = (Sprite*)m_pMixTool_electric->getChildByName("stick");
        if (pStick) {
            pStick->stopAllActions();
        }
    }
}


void CupcakeMixFlourScene::_separatePowder()
{
    
    m_pGuideLayer->removeGuide();
    m_nShakeAdd++;
    
    if (m_nShakeAdd<5){
        m_pSieve->setPowder("content/make/common/sleve/sieve_flour"+std::to_string(m_nShakeAdd+1)+".png");
        Sprite* pAdd = Sprite::create("content/make/common/sleve/bowl_flour"+std::to_string(m_nShakeAdd)+".png");
        if (pAdd) {
            pAdd->setPosition(Vec2(0, 0));
            pAdd->setOpacity(0);
            m_pBatter->addChild(pAdd,5);
            m_pSieve->stopSeparate();
            if (m_nShakeAdd<5){
                Device::setAccelerometerEnabled(true);
                m_pTouchLayer->setEnable(true);
            }
            pAdd->runAction(Sequence::create(FadeIn::create(0.5),
                                             CallFunc::create([=](){
            }), NULL));
        }
    }
    if (m_nShakeAdd>=4) {
        this->stopAllActions();
        m_pSieve->stopSeparate();
        Device::setAccelerometerEnabled(false);
        m_pTouchLayer->setEnable(false);
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("vo_good_job.mp3");
        }),DelayTime::create(1),
                                         CallFunc::create([=](){
            MenuLayer::addIngredient(MenuLayer::ePackageCupcake, MenuLayer::eCupcake_flour);
            MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
            m_nStep++;
            _finishAdd();
        }), NULL));
    }
}

void CupcakeMixFlourScene::_onTypeClicked(LockItem* item)
{
    if (!m_bItemEnable) {
        return;
    }
    if (m_pScrollView->isScrolled()) {
        return;
    }
    int tag = item->getTag();
    if (m_nFlavorIndex==tag) {
        return;
    }
    if (item->isLocked()){
        if(!ConfigManager::getInstance()->getVideoUnLocked("cupcake_flavor", tag)){
            ShopMiniLayer* pLayer = ShopMiniLayer::create(false);
            this->addChildToUILayer(pLayer);
            pLayer->setLocalZOrder(100);
            pLayer->shopDismissed = [=](){
                if (IAPManager::getInstance()->getItemIsBought(2)) {
                    for_each(m_typeItems.begin(), m_typeItems.end(), [=](LockItem* pItem){
                        pItem->unlock();
                    });
                }
            };
            pLayer->shopGetFree = [=](){
                LockUIManager::getInstance()->onRewardSuccess = [=](){
                    AudioHelp::getInstance()->playEffect("bling.mp3");
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/goodParticle.plist");
                    item->addChild(pParticle,100);
                    pParticle->setPosition(Vec2::ZERO);
                    item->unlock();
                };
                RewardManager::getInstance()->showRewardAds("cupcake_flavor", tag);
                pLayer->removeFromParent();
            };
            return;
        }
    }
    m_bItemEnable = false;
    AudioHelp::getInstance()->playSelectedEffect();
    if (m_nFlavorIndex>=0) {
        Node* pPreItem = m_typeItems.at(m_nFlavorIndex);
        pPreItem->runAction(MoveBy::create(0.5, Vec2(0, -50)));
    }
    item->runAction(MoveBy::create(0.5, Vec2(0, 50)));
    m_nFlavorIndex = tag;
    _showBottle(tag);
    AudioHelp::getInstance()->_playEffectLaugh();
}

void CupcakeMixFlourScene::_addFlavor()
{
    m_pGuideLayer->removeGuide();
    m_pGameUI->hideNext();
    m_bItemEnable = false;
    m_pBottle->setTouchEnabled(false);
    Device::setAccelerometerEnabled(false);
    if(m_nMixStep == 1) {
        GameDataManager::getInstance()->m_sFlavorStr = std::to_string(m_nFlavorIndex);
    }else{
        GameDataManager::getInstance()->m_sFlavorStr = GameDataManager::getInstance()->m_sFlavorStr+","+std::to_string(m_nFlavorIndex);
    }
    MixManager::getInstance()->reset();
    MixManager::getInstance()->_mixtureStartIndex = 0;
    MixManager::getInstance()->_mixCount = 4;
    MixManager::getInstance()->_mixSpeed = 200;
    MixManager::getInstance()->_mixRotateSpeed = 0;
    MixManager::getInstance()->_mixturePathEX = localPath("color/"+std::to_string(m_nFlavorIndex+1)+"_");
    MixManager::getInstance()->_mixtureNode = m_pBatter;
    MixManager::getInstance()->_mixtureSize = m_pBowl->getContentSize();
    MixManager::getInstance()->onMixCallback = [=](int index){
        if (index==2) {
            AudioHelp::getInstance()->playEffect("vo_keep_stirring.mp3");
        }
    };
    MixManager::getInstance()->onScheduleCallback = [=](int index){
        m_pProgressStar->addStar();
    };
    MixManager::getInstance()->onMixFinish = CC_CALLBACK_0(CupcakeMixFlourScene::_finishMix, this);
    
    Node* pLid = m_pBottle->getChildByName("front");
    if(pLid){
        AudioHelp::getInstance()->playEffect("bottle_open.mp3");
        pLid->runAction(Sequence::create(JumpBy::create(0.5, Vec2(100, 600), 100, 1),
                                         CallFunc::create([=](){
            pLid->setVisible(false);
        }), NULL));
    }
    m_pBottle->runAction(Sequence::create(DelayTime::create(0.3),
                                          MoveTo::create(0.5, m_pBowl->getPosition()+Vec2(100, 150)),
                                          RotateTo::create(0.5, -80),
                                          CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("drop_vanilla.mp3");
        
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/pour_vanilla.plist");
        pParticle->setTexture(Sprite::create("content/make/common/flavor/drop/"+std::to_string(m_nFlavorIndex+1)+".png")->getTexture());
        pParticle->setTotalParticles(2);
        m_pBottle->addChild(pParticle);
        pParticle->setPosition(m_pBottle->getActionPoint());
        pParticle->setName("particle");
        pParticle->setRotation(80);
        pParticle->setLocalZOrder(20);
        pParticle->setPosition(m_pBottle->convertToNodeSpace(m_pBottle->getWorldSpaceActionPoint()));
        
        
        Sprite* pAdd = Sprite::create(localPath("color/"+std::to_string(m_nFlavorIndex+1)+".png"));
        if (pAdd) {
            pAdd->setPosition(Vec2(-20,0));
            pAdd->setOpacity(0);
            m_pBatter->addChild(pAdd,5);
            pAdd->runAction(Sequence::create(DelayTime::create(1),
                                             FadeIn::create(1),
                                             CallFunc::create([=](){
            }), NULL));
        }
    }),
                                          DelayTime::create(2),
                                          //                                          JumpBy::create(2,Vec2::ZERO,50,3),
                                          MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(100, 100)),
                                          CallFunc::create([=](){
//        auto particle = dynamic_cast<ParticleSystem*>(m_pBottle->getChildByName("particle"));
//        if (particle) {
//            particle->stopSystem();
//            particle->removeFromParent();
//        }
        MenuLayer::addIngredient(MenuLayer::ePackageCupcake, MenuLayer::eCupcake_coloring);
        MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
    }),
                                          RotateTo::create(1, 0),
                                          MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                          CallFunc::create([=](){
        m_pBottle->removeFromParent();
        m_pBottle = nullptr;
        _showMixTools();
    }), NULL));
    m_pScrollView->runAction(Sequence::create(MoveBy::create(1, Vec2(0, -500)),
                                              CallFunc::create([=](){
        m_pScrollView->removeFromParent();
        m_pScrollView = nullptr;
    }), NULL));
    m_pBanner->runAction(Sequence::create(MoveBy::create(1, Vec2(0, -500)),
                                          CallFunc::create([=](){
        m_pBanner->removeFromParent();
        m_pBanner = nullptr;
    }), NULL));
}
void CupcakeMixFlourScene::_saveMixture()
{
    Size candySize = m_pBowl->getContentSize();
    RenderTexture* render = RenderTexture::create(candySize.width, candySize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    
    m_pBatter->setPosition(Vec2(m_pBowl->getContentSize().width*0.5,m_pBowl->getContentSize().height*0.5));
    render->beginWithClear(1.0f, 1.0f, 1.0f, 0.0f);
    m_pBatter->visit();
    render->end();
    
    Director::getInstance()->getRenderer()->render();
    m_pBatter->setPosition(Vec2::ZERO);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    Image* pImage = render->newImage();
    
    bool issuccess;
    
    issuccess = pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+"mixture.png", false);
    pImage->autorelease();
}
void CupcakeMixFlourScene::_finishAdd()
{
    AudioHelp::getInstance()->playEffect("vo_mix_ingredients.mp3");
    m_pSieve->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    _showMixTools();
    
//    _saveMixture();
}

void CupcakeMixFlourScene::_finishMix()
{
    m_pProgressStar->runAction(Sequence::create(DelayTime::create(0.5),
                                                EaseBackIn::create(MoveBy::create(1, Vec2(0, visibleSize.height))),
                                                CallFunc::create([=](){
        m_pProgressStar->removeFromParent();
    }), NULL));
    m_pBowl->stopAllActions();
    AudioHelp::getInstance()->stopLoopEffect();
    AudioHelp::getInstance()->playEffect("ding.mp3");
    AudioHelp::getInstance()->playEffect("vo_well_done.mp3");
//    AudioHelp::getInstance()->playEffect("present_out.mp3");
    m_pCurTool->setTouchEnabled(false);
    m_pCurTool->stopAllActions();
    m_pCurTool->setLocalZOrder(20);
    _toolElectricWork(false);
    Sprite* pStick = (Sprite*)m_pCurTool->getChildByName("stick");
    if (pStick) {
        pStick->stopAllActions();
        pStick->setTexture("content/make/common/mix/agitator2_2.png");
    }
    if (m_pCurTool->getName() == "tool_normal") {
        m_pCurTool->setDragSprite(Sprite::create("content/make/common/mix/agitator1_1.png"));
    }
    m_pCurTool->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)), NULL));
    
    if (m_nMixStep == 0){
        _showFlavorScrollView();
        m_pBowl->runAction(MoveBy::create(1, Vec2(0, 50)));
        m_pBowlFront->runAction(MoveBy::create(1, Vec2(0, 50)));
    }else if (m_nMixStep > 0 && m_nMixStep < 3) {
        m_pBowlFront->runAction(Sequence::create(DelayTime::create(2),
                                                 MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                 CallFunc::create([=](){
            CMVisibleRect::setPositionAdapted(m_pBowlFront, 320-visibleSize.width, 380);
        }),
                                                 MoveBy::create(1.0,Vec2(visibleSize.width,0)), NULL));
        m_pBowl->runAction(Sequence::create(DelayTime::create(2),
                                            MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                            CallFunc::create([=](){
            CMVisibleRect::setPositionAdapted(m_pBowl, 320-visibleSize.width, 380);
            m_pBatter->removeAllChildren();
//            TextureCache::getInstance()->removeTextureForKey(FileUtils::getInstance()->getWritablePath()+"mixture.png");
            Sprite* pAdd = Sprite::create(localPath("stir_5.png"));
            if (pAdd) {
                m_pBatter->addChild(pAdd);
            }
        }),
                                            MoveBy::create(1.0,Vec2(visibleSize.width,0)),
                                            CallFunc::create([=](){
            _showFlavorScrollView();
        }), NULL));
    }else if (m_nMixStep >= 3){
        _finish();
    }
    m_nMixStep++;
}

void CupcakeMixFlourScene::_finish()
{
    AudioHelp::getInstance()->playEffect("done.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<CupcakePourBatterScene>();
    }), NULL));
}