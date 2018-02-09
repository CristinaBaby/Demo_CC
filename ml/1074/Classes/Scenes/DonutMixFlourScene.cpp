
#include "DonutMixFlourScene.h"
#include "SceneManager.h"

DonutMixFlourScene::DonutMixFlourScene()
{
    m_nStep = 0;
    m_nFlourAdd = 0;
    m_nShakeCount = 0;
    m_nShakeAdd = 0;
    m_nEggCount = 0;
}

DonutMixFlourScene::~DonutMixFlourScene()
{
    
}
bool DonutMixFlourScene::init()
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
    m_pTouchLayer->onTouchEnded = CC_CALLBACK_2(DonutMixFlourScene::TouchEnded, this);
    m_pTouchLayer->setEnable(false);
    
    m_pGameUI->showNormalLayout();
    return true;
}

void DonutMixFlourScene::onEnter()
{
    ExtensionScene::onEnter();
}

void DonutMixFlourScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void DonutMixFlourScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
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
                                                                MoveTo::create(1, Vec2(visibleSize.width/2, m_pBowl->getPositionY()+100)), NULL),
                                                  CallFunc::create([=](){
                pDragNode->setRotation(0);
                pDragNode->setDragSprite(Sprite::create("content/make/common/mix/agitator1_2.png"));
                
                Rect rect = m_pBowl->getBoundingBox();
                rect.origin.x += 60;
                rect.origin.y += 80;
                rect.size = Size(250, 100);
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
                if (!ConfigManager::getInstance()->getVideoUnLocked("donut_classic_mixtool", 1)){
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
                        RewardManager::getInstance()->showRewardAds("donut_classic_mixtool", 1);
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
                rect.origin.x += 60;
                rect.origin.y += 80;
                rect.size = Size(250, 100);
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

void DonutMixFlourScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
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

void DonutMixFlourScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    AudioHelp::getInstance()->stopLoopEffect();
    std::string name = pDragNode->getName();
    if (name == "ingredient") {
        int tag = pDragNode->getTag();
        Rect rectBowl = m_pBowl->getBoundingBox();
        if (tag == 8) {//flour
            rectBowl = m_pSieve->getWorldRect();
        }
        Rect rectIngredient = pDragNode->getDragSprite()->getBoundingBox();
        rectIngredient.origin = pDragNode->convertToWorldSpace(rectIngredient.origin);
        if (rectBowl.intersectsRect(rectIngredient)){
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
    }
}

void DonutMixFlourScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

void DonutMixFlourScene::TouchEnded(Touch *pTouch, Event *pEvent)
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

void DonutMixFlourScene::onAcceleration(Acceleration* acc, Event* event){
    
    log("=================%f,%f,%f",acc->x,acc->y,acc->z);
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
}
#pragma mark - initData
void DonutMixFlourScene::_initData()
{
    setExPath("content/make/donut/make1/");
}

void DonutMixFlourScene::_showBowl()
{
    m_pBowl = Sprite::create(localPath("bowl_shadow.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 320-visibleSize.width, 380);
    m_pBowl->runAction(MoveBy::create(1.0,Vec2(visibleSize.width,0)));
    
    Sprite* pBowl = Sprite::create(localPath("bowl.png"));
    m_pBowl->addChild(pBowl);
    pBowl->setPosition(m_pBowl->getContentSize()*0.5);
    
    m_pBowlFront = Sprite::create(localPath("bowl_on.png"));
    this->addChildToContentLayer(m_pBowlFront);
    m_pBowlFront->setLocalZOrder(10);
    m_pBowlFront->setPosition(m_pBowl->getPosition());
    m_pBowlFront->runAction(MoveBy::create(1.0,Vec2(visibleSize.width,0)));
    
    m_pBatter = Node::create();
    m_pBowl->addChild(m_pBatter);
    m_pBatter->setPosition(m_pBowl->getContentSize()*0.5);
    
    MixManager::getInstance()->reset();
    MixManager::getInstance()->_mixtureStartIndex = 0;
    MixManager::getInstance()->_mixCount = 5;
    MixManager::getInstance()->_mixSpeed = 100;
    MixManager::getInstance()->_mixRotateSpeed = 0;
    MixManager::getInstance()->_mixturePathEX = localPath("stir");
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
    MixManager::getInstance()->onMixFinish = CC_CALLBACK_0(DonutMixFlourScene::_finishMix, this);
}

void DonutMixFlourScene::_showMixTools()
{
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
        if (!ConfigManager::getInstance()->getVideoUnLocked("donut_classic_mixtool", 1)){
            LockUIManager::getInstance()->registerLock("donut_classic_mixtool", 1, pMixTool_electric);
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
                if (ConfigManager::getInstance()->getVideoUnLocked("donut_classic_mixtool", 1)) {
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

void DonutMixFlourScene::_showIngredientsStep(int step)
{
    if(step==0) {
        _showIngredient(0);
        _showIngredient(1);
        AudioHelp::getInstance()->playEffect("present_in.mp3");
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("vo_yeast_salt.mp3");
        }), NULL));
    }else if (step == 1) {
        _showIngredient(2);
        _showIngredient(3);
        _showIngredient(4);
        AudioHelp::getInstance()->playEffect("present_in.mp3");
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("vo_butter_egg.mp3");
        }), NULL));
    }else if(step == 2){
        _showIngredient(5);
        _showIngredient(6);
        _showIngredient(7);
        AudioHelp::getInstance()->playEffect("present_in.mp3");
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("vo_vanilla.mp3");
        }), DelayTime::create(3),
                                         CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("vo_sugar_milk.mp3");
                                         }), NULL));
    }else if(step == 3){
        _showSieve();
        _showIngredient(8);
        AudioHelp::getInstance()->playEffect("present_in.mp3");
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("vo_using_sieve.mp3");
        }), NULL));
    }else if(step == 4){
        _showMixTools();
        _finishAdd();
    }
    
}

void DonutMixFlourScene::_showIngredient(int index)
{
    //    back shadow front food food2
    static std::string sIngredents[][5] ={
        {"salt.png","salt_1.png"}, //salt
        {"yeast_bowl_down.png","yeast_bowl_down1.png","yeast_bowl_on.png","yeast.png"}, //yeast
        {"butter_plate.png","butter_2.png","","butter_1.png"}, //butter
        {"egg.png","egg1.png"},  //egg
        {"egg.png","egg1.png"},  //egg
        {"vanilla_2.png","vanilla_3.png","vanilla_1.png"},  //vanilla
        {"sugar1.png","sugar1_2.png","sugar1_1.png"},  //sugar
        {"milk.png","milk_1.png"},  //milk
        {"flour_spoon2.png"},  //flour
        {"flour_down.png","","flour_on.png","flour1.png"},  //flour
        {},
    };
    
    Vec2 actionPoint[] = {
        Vec2(49, 157),
        Vec2(13, 90),
        Vec2(31, 63),
        Vec2(0, 0),
        Vec2(0, 0),
        Vec2(34, 144),
        Vec2(11, 160),
        Vec2(20, 183),
        Vec2(23, 26),
    };
    Vec2 pos[] = {
        Vec2(195, 760),
        Vec2(410, 740),
        Vec2(195, 720),
        Vec2(400, 740),
        Vec2(460, 726),
        Vec2(155, 760),
        Vec2(320, 780),
        Vec2(480, 800),
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

void DonutMixFlourScene::_showSieve()
{
    m_pSieve = SieveNode::create();
    this->addChildToContentLayer(m_pSieve);
    CMVisibleRect::setPositionAdapted(m_pSieve, 330-visibleSize.width, 600+50);
    m_pSieve->runAction(MoveBy::create(1.0,Vec2(visibleSize.width,0)));
    m_pSieve->setLocalZOrder(5);
    
    auto eventAcc = EventListenerAcceleration::create(CC_CALLBACK_2(DonutMixFlourScene::onAcceleration, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventAcc, this);
}

void DonutMixFlourScene::_showProgressStar()
{
    m_pProgressStar = ProgressStar::create();
    this->addChildToUILayer(m_pProgressStar);
    CMVisibleRect::setPositionAdapted(m_pProgressStar, 320, 850+visibleSize.height);
    m_pProgressStar->runAction(EaseBackOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height))));
}


void DonutMixFlourScene::_addIngredient(DragNode* pIngredient)
{
    static string sBowlIn[] = {
        "bowl_salt.png",
        "bowl_yeast.png",
        "bowl_butter.png",
        "bowl_egg.png",
        "bowl_egg.png",
        "bowl_vanilla.png",
        "bowl_sugar.png",
        "bowl_milk.png",
        "",
    };
    int index = pIngredient->getTag();
    IngredientManager::getInstance()->frozeIngredients(index);
    int k = index;
    std::string lNameStr = sBowlIn[k];
    float dt = 2;
    Vec2 posAdd = Vec2::ZERO;
    pIngredient->setTouchEnabled(false);
    if (index != 4 || index != 3) {
        this->runAction(Sequence::create(DelayTime::create(0.5),
                                         CallFunc::create([=](){
            pIngredient->setLocalZOrder(5);
        }), NULL));
    }
    switch (index) {
#pragma mark =========  add salt ==========
        case 0:
        {
            posAdd = Vec2(-90, -20);
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(20, 100)),
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
                if (m_nStep == 2) {
                    _showIngredientsStep(1);
                }
                MenuLayer::addIngredient(MenuLayer::ePackageDonut, MenuLayer::eDonut_salt);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                IngredientManager::getInstance()->removeIngredient(pIngredient);
                pIngredient->removeFromParent();
            }), NULL));
        }
            break;
#pragma mark =========  add yeaset ==========
        case 1:
        {
            posAdd = Vec2(50, 0);
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
                
                AudioHelp::getInstance()->playEffect("vo_good.mp3");
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                
                m_nStep++;
                if (m_nStep == 2) {
                    _showIngredientsStep(1);
                }
                MenuLayer::addIngredient(MenuLayer::ePackageDonut, MenuLayer::eDonut_yeast);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                IngredientManager::getInstance()->removeIngredient(pIngredient);
                pIngredient->removeFromParent();
            }), NULL));
        }
            break;
            

#pragma mark =========  add Butter ==========
        case 2:
        {
            posAdd = Vec2(0, 0);
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
                AudioHelp::getInstance()->playEffect("vo_well_done.mp3");
                
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                
                m_nStep++;
                if (m_nStep == 5) {
                    _showIngredientsStep(2);
                }
                MenuLayer::addIngredient(MenuLayer::ePackageDonut, MenuLayer::eDonut_butter);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                IngredientManager::getInstance()->removeIngredient(pIngredient);
                pIngredient->removeFromParent();
            }), NULL));
        }
            break;
#pragma mark =========  add Egg ==========
        case 3:
        case 4:
        {
            dt = 4;
            posAdd = Vec2(10+20*(index-3), 10-20*(index-3));
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(m_pBowl->getContentSize().width/2-40, 100)),
                                                    JumpBy::create(1,Vec2::ZERO,20,2),
                                                    CallFunc::create([=](){
                pIngredient->setVisible(false);
                
                Sprite* pEgg = Sprite::create(localPath("egg_1.png"));
                this->addChildToContentLayer(pEgg);
                pEgg->setPosition(pIngredient->getWorldSpaceActionPoint());
                pEgg->setLocalZOrder(20);
                
                pEgg->runAction(Sequence::create(MoveTo::create(0.5, m_pBowl->getPosition()+Vec2(50*(index-3), 100)),
                                                 CallFunc::create([=](){
                    AudioHelp::getInstance()->playAddEggEffect();
                }),
                                                 DelayTime::create(1),
                                                 CallFunc::create([=](){
                    pEgg->setTexture(localPath("egg_2.png"));
                    if (m_nEggCount==1) {
                        AudioHelp::getInstance()->playEffect(arc4random()%100>0?"vo_sweet.mp3":"vo_fabulous.mp3");
                    }
                }),
                                                 DelayTime::create(1),
                                                 CallFunc::create([=](){
                    
                    IngredientManager::getInstance()->frozeIngredients(index,false);
                    
                    m_nStep++;
                    if (m_nStep == 5) {
                        _showIngredientsStep(2);
                    }
                    m_nEggCount++;
                    if (m_nEggCount==2) {
                        MenuLayer::addIngredient(MenuLayer::ePackageDonut, MenuLayer::eDonut_egg);
                        MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                    }
                    IngredientManager::getInstance()->removeIngredient(pIngredient);
                    pIngredient->removeFromParent();
                    pEgg->removeFromParent();
                }), NULL));
            }), NULL));
        }
            break;
        case 5:
#pragma mark =========  add vanilla ==========
        {
            posAdd = Vec2(-50, -10);
            Node* pLid = pIngredient->getChildByName("front");
            if(pLid){
                pLid->runAction(Sequence::create(JumpBy::create(1, Vec2(100, 600), visibleSize.height, 1),
                                                 CallFunc::create([=](){
                    pLid->setVisible(false);
                }), NULL));
            }
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(50, 100)),
                                                    RotateTo::create(0.5,-90),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("drop_vanilla.mp3");
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/pour_vanilla.plist");
                pParticle->setTotalParticles(2);
                pIngredient->addChild(pParticle);
                pParticle->setPosition(pIngredient->getActionPoint());
                pParticle->setName("particle");
                pParticle->setRotation(90);
                pParticle->setLocalZOrder(20);
                pParticle->setPosition(pIngredient->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                
                
            }),
                                                    JumpBy::create(2,Vec2::ZERO,50,3),
                                                    MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(100, 100)),
                                                    CallFunc::create([=](){
//                auto particle = dynamic_cast<ParticleSystem*>(pIngredient->getChildByName("particle"));
//                if (particle) {
//                    particle->stopSystem();
//                    particle->removeFromParent();
//                }
                
            }),
                                                    RotateTo::create(1, 0),
                                                    CallFunc::create([=](){
                _ingredientOut(pIngredient);
                AudioHelp::getInstance()->playEffect("vo_talented.mp3");
                
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                
                m_nStep++;
                if (m_nStep == 8) {
                    _showIngredientsStep(3);
                }
                MenuLayer::addIngredient(MenuLayer::ePackageDonut, MenuLayer::eDonut_vanilla);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                IngredientManager::getInstance()->removeIngredient(pIngredient);
                pIngredient->removeFromParent();
            }), NULL));
            
        }
            break;
#pragma mark =========  add sugar ==========
        case 6:
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
                AudioHelp::getInstance()->playEffect("kids_laughing3.mp3");
            }),
                                                    RotateTo::create(1, 0),
                                                    CallFunc::create([=](){
                _ingredientOut(pIngredient);
                AudioHelp::getInstance()->playEffect("vo_good_job.mp3");
                
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                
                m_nStep++;
                if (m_nStep == 8) {
                    _showIngredientsStep(3);
                }
                MenuLayer::addIngredient(MenuLayer::ePackageDonut, MenuLayer::eDonut_sugar);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                IngredientManager::getInstance()->removeIngredient(pIngredient);
                pIngredient->removeFromParent();
            }), NULL));
        }
            break;
#pragma mark =========  add milk ==========
        case 7:
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
                
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                
                m_nStep++;
                if (m_nStep == 8) {
                    _showIngredientsStep(3);
                }
                MenuLayer::addIngredient(MenuLayer::ePackageDonut, MenuLayer::eDonut_milk);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                IngredientManager::getInstance()->removeIngredient(pIngredient);
                pIngredient->removeFromParent();
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
            if (index == 7) {
                pAdd->setLocalZOrder(1);
            }
            m_pGuideLayer->removeGuide();
        }), NULL));
    }
}


void DonutMixFlourScene::_ingredientOut(DragNode* pNode)
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

void DonutMixFlourScene::_toolElectricWork(bool working)
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


void DonutMixFlourScene::_separatePowder()
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
            MenuLayer::addIngredient(MenuLayer::ePackageDonut, MenuLayer::eDonut_flour);
            MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
            _finishAdd();
        }), NULL));
    }
}

void DonutMixFlourScene::_finishAdd()
{
    m_pSieve->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
    _showMixTools();
}

void DonutMixFlourScene::_finishMix()
{
    m_pProgressStar->runAction(Sequence::create(DelayTime::create(0.5),
                                                EaseBackIn::create(MoveBy::create(1, Vec2(0, visibleSize.height))),
                                                CallFunc::create([=](){
        m_pProgressStar->removeFromParent();
    }), NULL));
    AudioHelp::getInstance()->stopLoopEffect();
    AudioHelp::getInstance()->playEffect("ding.mp3");
    AudioHelp::getInstance()->playEffect("vo_well_done.mp3");
    m_pBowl->stopAllActions();
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
    
    AudioHelp::getInstance()->playEffect("done.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<DonutPourBatterScene>();
    }), NULL));
}