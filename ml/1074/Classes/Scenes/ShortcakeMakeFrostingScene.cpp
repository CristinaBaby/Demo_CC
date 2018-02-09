
#include "ShortcakeMakeFrostingScene.h"
#include "SceneManager.h"

ShortcakeMakeFrostingScene::ShortcakeMakeFrostingScene()
{
    m_nStep = 0;
    m_nFlavorIndex = -1;
    m_bItemEnable = false;
    m_pSauceBottle = nullptr;
    
}

ShortcakeMakeFrostingScene::~ShortcakeMakeFrostingScene()
{
    
}
bool ShortcakeMakeFrostingScene::init()
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
        AudioHelp::getInstance()->playEffect("vo_add_ingredients.mp3");
    }),
                                     DelayTime::create(3),
                                     CallFunc::create([=](){
        //        m_pGuideLayer->showGuideMove(m_pFlour->getPosition(), m_pBowl->getPosition());
    }), NULL));
    m_pGameUI->showNormalLayout();
    auto eventAcc = EventListenerAcceleration::create(CC_CALLBACK_2(ShortcakeMakeFrostingScene::onAcceleration, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventAcc, this);
    return true;
}

void ShortcakeMakeFrostingScene::onEnter()
{
    ExtensionScene::onEnter();
}

void ShortcakeMakeFrostingScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void ShortcakeMakeFrostingScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if (name == "ingredient"){
        int tag = pDragNode->getTag();
        IngredientManager::getInstance()->showShadow(tag,false);
    }
}

void ShortcakeMakeFrostingScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setScale(1);
    std::string name = pDragNode->getName();
    
}

void ShortcakeMakeFrostingScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
    m_pGuideLayer->removeGuide();
    std::string name = pDragNode->getName();
    if (name == "ingredient") {
        int tag = pDragNode->getTag();
        Rect rectBowl = m_pBowl->getBoundingBox();
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
        }
        
    }else if (name == "sauce_bottle"){
        _pourSauce();
    }else if (name == "blender_switch"){
        m_pBlenderSwitch->setTouchEnabled(false);
        m_pBlenderSwitch->getDragSprite()->setTexture(localPath("launch2.png"));
        m_pBlenderStick->runAction(RepeatForever::create(Sequence::create(AnimationHelp::createAnimate(localPath("mixer"), 2, 4,false),
                                                                     CallFunc::create([=](){
            
//            MixManager::getInstance()->mix();
        }),  NULL)));
        AudioHelp::getInstance()->playEffect("press_button.mp3");
        AudioHelp::getInstance()->playLoopEffect("stir_spoon.mp3");
        this->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.02),
                                                               CallFunc::create([=](){
            
            MixManager::getInstance()->mix();
        }),  NULL)));
        _showProgressStar();
    }
}

void ShortcakeMakeFrostingScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    dragNodeTouchEnded(pDragNode,worldPoint);
    
}

void ShortcakeMakeFrostingScene::onAcceleration(Acceleration* acc, Event* event)
{
    if (acc->x<0 ){
        m_pSauceBottle->setRotation(100*acc->x);
    }
    if (acc->x<-0.5 ) {
        m_pSauceBottle->setTouchEnabled(false);
        Device::setAccelerometerEnabled(false);
        _pourSauce();
    }
}

#pragma mark - initData
void ShortcakeMakeFrostingScene::_initData()
{
    setExPath("content/make/shortcake/make2/");
}

void ShortcakeMakeFrostingScene::_showBowl()
{
    m_pBowl = Sprite::create(localPath("bowl2_0.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 320-visibleSize.width, 380);
    m_pBowl->runAction(MoveBy::create(1.0,Vec2(visibleSize.width,0)));
    
    m_pBowlFront = Sprite::create(localPath("bowl2_1.png"));
    this->addChildToContentLayer(m_pBowlFront);
    m_pBowlFront->setLocalZOrder(10);
    m_pBowlFront->setPosition(m_pBowl->getPosition());
    m_pBowlFront->runAction(MoveBy::create(1.0,Vec2(visibleSize.width,0)));
    
    m_pBatterClipping = ClippingNode::create(Sprite::create(localPath("bowl2_mask.png")));
    m_pBowl->addChild(m_pBatterClipping);
    m_pBatterClipping->setAlphaThreshold(0.5);
    m_pBatterClipping->setPosition(Vec2(m_pBowl->getContentSize().width*0.5,m_pBowl->getContentSize().height*0.5));
    
    m_pBatter = Node::create();
    m_pBatterClipping->addChild(m_pBatter);
}

void ShortcakeMakeFrostingScene::_showMixBlender()
{
    m_pBlender = Sprite::create(localPath("mixer.png"));
    this->addChildToContentLayer(m_pBlender);
    CMVisibleRect::setPositionAdapted(m_pBlender, 250-visibleSize.width, 480);
    
    m_pBlenderStick = Sprite::create(localPath("mixer1.png"));
    this->addChildToContentLayer(m_pBlenderStick);
    CMVisibleRect::setPositionAdapted(m_pBlenderStick, 250-visibleSize.width, 480);
    
    m_pBlenderSwitch = _createDrageNode(localPath("launch1.png"));
    m_pBlender->addChild(m_pBlenderSwitch);
    m_pBlenderSwitch->setPosition(Vec2(430, 500));
    m_pBlenderSwitch->setName("blender_switch");
    m_pBlenderSwitch->setMovable(false);
    m_pBlenderSwitch->setTouchEnabled(false);
    
    m_pBlenderStick->setLocalZOrder(5);
    m_pBlenderStick->runAction(MoveBy::create(1.0,Vec2(visibleSize.width,0)));
    m_pBlender->runAction(Sequence::create(MoveBy::create(1.0,Vec2(visibleSize.width,0)),
                                           DelayTime::create(0.5),
                                           CallFunc::create([=](){
        m_pBlenderSwitch->setTouchEnabled(true);
        m_pGuideLayer->showGuideTap(m_pBlender->convertToWorldSpace(m_pBlenderSwitch->getPosition()));
    }), NULL));
}

void ShortcakeMakeFrostingScene::_showIngredientsStep(int step)
{
    if(step==0) {
        _showIngredient(0);
        _showIngredient(1);
        _showIngredient(2);
        AudioHelp::getInstance()->playEffect("present_in.mp3");
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("vo_fresh_cream_filling.mp3");
        }),
                                         DelayTime::create(4.5),
                                         CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("vo_icing_sugar_vanilla.mp3");
        }), NULL));
    }else if (step == 1) {
        _showTypeItemScroll();
    }else if(step == 2){
        _showMixBlender();
        auto action = Sequence::create(MoveBy::create(0.5, Vec2(0, -200)),
                                       DelayTime::create(0.5),
                                       MoveBy::create(0.5, Vec2(0, 200)), NULL);
        m_pBowl->setLocalZOrder(2);
        m_pBowl->runAction(action);
        m_pBowlFront->runAction(action->clone());
        m_pGuideLayer->showGuideTap(m_pBlenderSwitch->getParent()->convertToWorldSpace(m_pBlenderSwitch->getPosition()));
        MixManager::getInstance()->reset();
        MixManager::getInstance()->_mixtureStartIndex = -1;
        MixManager::getInstance()->_mixCount = 4;
        MixManager::getInstance()->_mixSpeed = 200;
        MixManager::getInstance()->_mixRotateSpeed = 0;
        MixManager::getInstance()->_mixturePathEX = localPath("cream"+std::to_string(m_nFlavorIndex+1))+"_";
        MixManager::getInstance()->_mixtureNode = m_pBatter;
        MixManager::getInstance()->_mixtureSize = m_pBowl->getContentSize();
        MixManager::getInstance()->onMixFinish = CC_CALLBACK_0(ShortcakeMakeFrostingScene::_finishMix, this);
        MixManager::getInstance()->onMixCallback = CC_CALLBACK_1(ShortcakeMakeFrostingScene::_doMix, this);
        MixManager::getInstance()->onScheduleCallback = [=](int index){
            m_pProgressStar->addStar();
        };
    }else if(step == 3){
    }else if(step == 4){
    }
    
}

void ShortcakeMakeFrostingScene::_showIngredient(int index)
{
    //    back shadow front food food2
    static std::string sIngredents[][5] ={
        {"heavy_cream0.png"}, //cream
        {"vanilla0.png"},  //vanlla
        {"icing_sugar0.png"},  //sugar
        {},
    };
    
    Vec2 actionPoint[] = {
        Vec2(37, 171),
        Vec2(38, 167),
        Vec2(33, 201),
    };
    Vec2 pos[] = {
        Vec2(150, 750),
        Vec2(320, 770),
        Vec2(500, 800),
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
}

void ShortcakeMakeFrostingScene::_showTypeItemScroll()
{
    m_pScrollView = ExpandScrollView::create();
    m_pScrollView->setContentSize(Size(visibleSize.width,200));
    this->addChildToContentLayer(m_pScrollView);
    CMVisibleRect::setPositionAdapted(m_pScrollView, visibleSize.width, 700,kBorderLeft);
    m_pScrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    m_pScrollView->setLocalZOrder(5);
    m_pScrollView->setScrollBarEnabled(false);
    m_pScrollView->setClippingEnabled(false);
    
    int typeCount = 14;
    m_pScrollView->getInnerContainer()->setContentSize(Size((typeCount+1)*115, 200));
    for (int i = 0; i<typeCount; i++) {
        LockItem* pItem = LockItem::create();
        
        bool bLock = false;
        if (!gNoneIap && !IAPManager::getInstance()->getItemIsBought(2) && i%2==1){
            if (!ConfigManager::getInstance()->getVideoUnLocked("shortcake_cream_flavor", i)) {
                LockUIManager::getInstance()->registerLock("shortcake_cream_flavor", i, pItem);
                LockUIManager::getInstance()->setLockOriginPos(Vec2(40, -60));
                pItem->lockType = 1;
                bLock = true;
            }
        }
        pItem->initWithName("content/make/common/flavor/bottle/color"+std::to_string(i+1)+"_0.png", "content/common/video.png",bLock);
        
        Sprite* Lock = pItem->getLockSprite();
        if (Lock) {
            Lock->setTag(TAG_REWARD_LOCK);
        }
        pItem->setTag(i);
        m_pScrollView->getInnerContainer()->addChild(pItem);
        pItem->setPosition(Vec2(100+i*115,100));
        //        pItem->setScale(0.5);
        
        pItem->onItemClicked = CC_CALLBACK_1(ShortcakeMakeFrostingScene::_onTypeClicked, this);
        m_typeItems.pushBack(pItem);
    }
    m_pScrollView->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                              CallFunc::create([=](){
        m_bItemEnable = true;
        AudioHelp::getInstance()->playEffect("vo_food_coloring_cream.mp3");
    }), NULL));
    
    AudioHelp::getInstance()->playEffect("slide_sight.mp3");
}

void ShortcakeMakeFrostingScene::_showBottle(int index)
{
    m_pGuideLayer->removeGuide();
    //        show sauce
    if(m_nFlavorIndex>=0){
        
    }else{
        
    }
    LockItem* pItem = m_typeItems.at(index);
    m_pScrollView->setTouchEnabled(false);
    
    if(m_pSauceBottle){
        DragNode* pPreBottle = m_pSauceBottle;
        LockItem* pPreFlavor = m_typeItems.at(m_nFlavorIndex);
        pPreBottle->setTouchEnabled(false);
        pPreBottle->runAction(Sequence::create(MoveTo::create(1, pPreFlavor->getParent()->convertToWorldSpace(pPreFlavor->getPosition())),
                                               CallFunc::create([=](){
            pPreFlavor->setVisible(true);
            pPreBottle->removeFromParent();
        }), NULL));
    }
    
    pItem->setVisible(false);
    AudioHelp::getInstance()->playEffect("loading_3.mp3");
    m_pSauceBottle = _createDrageNode("content/make/common/flavor/bottle/color"+std::to_string(index+1)+"_1.png");
    m_pSauceBottle->setPosition(pItem->getParent()->convertToWorldSpace(pItem->getPosition()));
    
    this->addChildToContentLayer(m_pSauceBottle);
    m_pSauceBottle->setName("sauce_bottle");
    m_pSauceBottle->setMovable(false);
    KettleNode* pContainer = KettleNode::create(m_pSauceBottle);
    pContainer->addFront("content/make/common/flavor/bottle/color"+std::to_string(index+1)+"_2.png","front",Vec2(0, 120));
    m_pSauceBottle->setUserData(pContainer);
    m_pSauceBottle->setTouchEnabled(false);
    AudioHelp::getInstance()->playEffect("loading_3.mp3");
    m_pSauceBottle->runAction(Sequence::create(MoveTo::create(1, m_pBowl->getPosition()+Vec2(100, 150)),
                                               CallFunc::create([=](){
        m_pGuideLayer->showGuideRotateLeft(m_pSauceBottle->getPosition());
        m_pSauceBottle->setTouchEnabled(true);
//        m_pGuideLayer->showGuideShake(m_pSauceBottle->getPosition());
        Device::setAccelerometerEnabled(true);
        m_pScrollView->setTouchEnabled(true);
        m_bItemEnable = true;
    }), NULL));
    
    m_pSauceBottle->setLocalZOrder(10);
    m_pSauceBottle->setActionPoint(Vec2(48, 193));
    m_nFlavorIndex = index;
    m_pSauceBottle->setTag(index);
}


void ShortcakeMakeFrostingScene::_showProgressStar()
{
    m_pProgressStar = ProgressStar::create();
    this->addChildToUILayer(m_pProgressStar);
    CMVisibleRect::setPositionAdapted(m_pProgressStar, 320, 850+visibleSize.height);
    m_pProgressStar->runAction(EaseBackOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height))));
}



void ShortcakeMakeFrostingScene::_addIngredient(DragNode* pIngredient)
{
    static string sBowlIn[] = {
        "heavy_cream2.png",
        "bowl_vanilla.png",
        "icing_sugar2.png",
        "",
        "",
    };
    int index = pIngredient->getTag();
    IngredientManager::getInstance()->frozeIngredients(index);
    int k = index;
    std::string lNameStr = sBowlIn[k];
    float dt = 2;
    Vec2 posAdd = Vec2::ZERO;
    pIngredient->setTouchEnabled(false);
    if (index < 5 || index >7) {
        this->runAction(Sequence::create(DelayTime::create(0.5),
                                         CallFunc::create([=](){
            pIngredient->setLocalZOrder(5);
        }), NULL));
    }
    switch (index) {
#pragma mark =========  add cream ==========
        case 0:
        {
            posAdd = Vec2(0, 0);
            pIngredient->getDragSprite()->setTexture(localPath("heavy_cream1.png"));
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(60, 100)),
                                                    RotateTo::create(0.5,-90),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("pouring_juice.mp3");
                
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/milk.plist");
                pIngredient->addChild(pParticle);
                pParticle->setPosition(pIngredient->getActionPoint());
                pParticle->setName("particle");
                pParticle->setRotation(90);
                pParticle->setLocalZOrder(20);
                pParticle->setScaleX(-1);
                pParticle->setPosition(pIngredient->convertToNodeSpace(pIngredient->getWorldSpaceActionPoint()));
                
            }),
                                                    CircleMove::create(2,m_pBowl->getPosition()+Vec2(80, 200),60,180),
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
                AudioHelp::getInstance()->_playEffectNiceWork();
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                
                m_nStep++;
                if (m_nStep == 3) {
                    _showIngredientsStep(1);
                }
                MenuLayer::addIngredient(MenuLayer::ePackageShortcake, MenuLayer::eShortcake_cream);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                IngredientManager::getInstance()->removeIngredient(pIngredient);
                pIngredient->removeFromParent();
            }), NULL));
        }
            break;
        case 1:
#pragma mark =========  add vanilla ==========
        {
            posAdd = Vec2(-50, -80);
            pIngredient->getDragSprite()->setTexture(localPath("vanilla1.png"));
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(50, 160)),
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
                AudioHelp::getInstance()->_playEffectNiceWork();
                
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                
                m_nStep++;
                if (m_nStep == 3) {
                    _showIngredientsStep(1);
                }
                MenuLayer::addIngredient(MenuLayer::ePackageShortcake, MenuLayer::eShortcake_vanilla);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                IngredientManager::getInstance()->removeIngredient(pIngredient);
                pIngredient->removeFromParent();
            }), NULL));
            
        }
            break;
#pragma mark =========  add sugar ==========
        case 2:
        {
            posAdd = Vec2(-40, -30);
            pIngredient->getDragSprite()->setTexture(localPath("icing_sugar1.png"));
            pIngredient->runAction(Sequence::create(MoveTo::create(0.5,m_pBowl->getPosition()+Vec2(60, 200)),
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
                
            }),
                                                    MoveBy::create(2,Vec2(80, 50)),
                                                    CallFunc::create([=](){
                AudioHelp::getInstance()->playEffect("kids_laughing3.mp3");
                auto particle = dynamic_cast<ParticleSystem*>(pIngredient->getChildByName("particle"));
                if (particle) {
                    particle->stopSystem();
                    particle->removeFromParent();
                }
            }),
                                                    RotateTo::create(1, 0),
                                                    CallFunc::create([=](){
                _ingredientOut(pIngredient);
                AudioHelp::getInstance()->_playEffectNiceWork();
                
            }),
                                                    MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                    CallFunc::create([=](){
                
                m_nStep++;
                if (m_nStep == 3) {
                    _showIngredientsStep(1);
                }
                MenuLayer::addIngredient(MenuLayer::ePackageShortcake, MenuLayer::eShortcake_icing_sugar);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
                IngredientManager::getInstance()->removeIngredient(pIngredient);
                pIngredient->removeFromParent();
            }), NULL));
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
            m_pGuideLayer->removeGuide();
        }), NULL));
    }
}


void ShortcakeMakeFrostingScene::_ingredientOut(DragNode* pNode)
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

void ShortcakeMakeFrostingScene::_onTypeClicked(LockItem* item)
{
    if (!m_bItemEnable) {
        return;
    }
    if (m_pScrollView->isScrolled()) {
        return;
    }
    int tag = item->getTag();
    if (item->isLocked()){
        if(!ConfigManager::getInstance()->getVideoUnLocked("shortcake_cream_flavor", tag)){
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
                RewardManager::getInstance()->showRewardAds("shortcake_cream_flavor", tag);
                pLayer->removeFromParent();
            };
            return;
        }
    }
    AudioHelp::getInstance()->playSelectedEffect();
    item->runAction(Sequence::create(ScaleTo::create(0.13, 1.0, 0.90),
                                     ScaleTo::create(0.11, 0.92, 1.0),
                                     ScaleTo::create(0.10, 1.0, 0.96),
                                     ScaleTo::create(0.09, 0.98, 1.0),
                                     ScaleTo::create(0.08, 1.0, 0.99),
                                     ScaleTo::create(0.07, 0.99, 1.0), NULL));
    _showBottle(tag);
    m_bItemEnable = false;
}

void ShortcakeMakeFrostingScene::_pourSauce()
{
    m_pSauceBottle->setTouchEnabled(false);
    Device::setAccelerometerEnabled(false);
    m_pScrollView->setTouchEnabled(false);
    m_bItemEnable = false;
    m_pGuideLayer->removeGuide();
    m_pScrollView->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                              CallFunc::create([=](){
        m_pScrollView->removeFromParent();
        m_pScrollView = nullptr;
    }), NULL));
    Node* pLid = m_pSauceBottle->getChildByName("front");
    if(pLid){
        AudioHelp::getInstance()->playEffect("bottle_open.mp3");
        pLid->runAction(Sequence::create(JumpBy::create(0.5, Vec2(100, 600), 100, 1),
                                         CallFunc::create([=](){
            pLid->setVisible(false);
        }), NULL));
    }
    m_pSauceBottle->runAction(Sequence::create(RotateTo::create(0.5, -90),
                                               CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("drop_vanilla.mp3");
        
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/pour_vanilla.plist");
        pParticle->setTexture(Sprite::create("content/make/common/flavor/drop/"+std::to_string(m_nFlavorIndex+1)+".png")->getTexture());
        pParticle->setTotalParticles(2);
        m_pSauceBottle->addChild(pParticle);
        pParticle->setPosition(m_pSauceBottle->getActionPoint());
        pParticle->setName("particle");
        pParticle->setRotation(90);
        pParticle->setLocalZOrder(20);
        pParticle->setPosition(m_pSauceBottle->convertToNodeSpace(m_pSauceBottle->getWorldSpaceActionPoint()));
        
        Sprite* pAdd = Sprite::create("content/make/common/flavor/in/"+std::to_string(m_nFlavorIndex+1)+"_0.png");
        
        m_pBatter->addChild(pAdd,5);
        pAdd->setScale(0.4);
        pAdd->setPosition(Vec2(0,-50));
        pAdd->setOpacity(0);
        pAdd->runAction(Sequence::create(FadeIn::create(0.5),
                                         ScaleTo::create(1, 1), NULL));
    }),
                                               DelayTime::create(2),
                                               CallFunc::create([=](){
        
//        auto particle = dynamic_cast<ParticleSystem*>(m_pSauceBottle->getChildByName("particle"));
//        if (particle) {
//            particle->stopSystem();
//            particle->removeFromParent();
//        }
        
        AudioHelp::getInstance()->playEffect("vo_fabulous.mp3");
    }),
                                               RotateTo::create(0.3, 0),
                                               MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                               CallFunc::create([=](){
        _showIngredientsStep(2);
    }), NULL));
}

void ShortcakeMakeFrostingScene::_doMix(int index)
{
    if (index==3) {
        AudioHelp::getInstance()->playEffect("vo_cream_will_ready.mp3");
    }
    
}

void ShortcakeMakeFrostingScene::_finishMix()
{
    m_pProgressStar->runAction(Sequence::create(DelayTime::create(0.5),
                                                EaseBackIn::create(MoveBy::create(1, Vec2(0, visibleSize.height))),
                                                CallFunc::create([=](){
        m_pProgressStar->removeFromParent();
    }), NULL));
    AudioHelp::getInstance()->stopLoopEffect();
    AudioHelp::getInstance()->playEffect("ding.mp3");
    AudioHelp::getInstance()->playEffect("vo_well_done.mp3");
    GameDataManager::getInstance()->m_nFlavorIndex = m_nFlavorIndex;
    m_pBlenderStick->stopAllActions();
    this->stopAllActions();
    AudioHelp::getInstance()->playEffect("present_out.mp3");
    m_pBlenderStick->runAction(JumpBy::create(1, Vec2(-visibleSize.width, 0), 300, 1));
    m_pBlender->runAction(JumpBy::create(1, Vec2(-visibleSize.width, 0), 300, 1));
    m_pBlenderSwitch->getDragSprite()->setTexture(localPath("mixer1.png"));
    m_pGuideLayer->removeGuide();
    
    AudioHelp::getInstance()->playEffect("done.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<ShortcakeAddFrostingScene>();
    }), NULL));
}
//