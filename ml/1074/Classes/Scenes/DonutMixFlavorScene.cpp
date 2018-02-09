
#include "DonutMixFlavorScene.h"
#include "SceneManager.h"

DonutMixFlavorScene::DonutMixFlavorScene()
{
    m_pBottle = nullptr;
    m_pBowl = nullptr;
    m_pMixTool = nullptr;
    m_nFinishCount = 0;
    m_nFlavorIndex = -1;
}

DonutMixFlavorScene::~DonutMixFlavorScene()
{
    
}
bool DonutMixFlavorScene::init()
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
    
    _showBowls();
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        _showBowl(m_nFinishCount);
        _showFlavorScrollView();
        _setMix();
    }), NULL));
    
    auto eventAcc = EventListenerAcceleration::create(CC_CALLBACK_2(DonutMixFlavorScene::onAcceleration, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventAcc, this);
    m_pGameUI->showNormalLayout();
    return true;
}

void DonutMixFlavorScene::onEnter()
{
    ExtensionScene::onEnter();
}

void DonutMixFlavorScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void DonutMixFlavorScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
}

void DonutMixFlavorScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name == "tool_normal"){
        pDragNode->setLocalZOrder(8);
        Rect bowlRect = m_pBowl->getBoundingBox();
        if (bowlRect.containsPoint(worldPoint)) {
            AudioHelp::getInstance()->playLoopEffect("stir_spoon.mp3");
            MixManager::getInstance()->mix();
        }else{
            AudioHelp::getInstance()->stopLoopEffect();
            m_pBowl->stopAllActions();
        }
    }
}

void DonutMixFlavorScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    std::string name = pDragNode->getName();
    if (name == "tool_normal"){
        AudioHelp::getInstance()->stopLoopEffect();
        m_pBowl->stopAllActions();
    }else if (name == "bottle") {
        pDragNode->setTouchEnabled(false);
        _addFlavor();
//        m_pBottle->setDragSprite(Sprite::create("content/make/common/flavor/bottle/color"+std::to_string(m_nFlavorIndex+1)+"_1.png"));
//        Sprite* pCap = Sprite::create("content/make/common/flavor/bottle/color"+std::to_string(m_nFlavorIndex+1)+"_2.png");
//        pCap->setAnchorPoint(Vec2(0.5, 0));
//        pCap->setPosition(pDragNode->getPosition()+Vec2(0, 100));
//        pCap->runAction(Sequence::create(MoveBy::create(1, visibleSize),
//                                         CallFunc::create([=](){
//            pCap->removeFromParent();
//        }), NULL));
    }
}

void DonutMixFlavorScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    dragNodeTouchEnded(pDragNode,worldPoint);
}

void DonutMixFlavorScene::onAcceleration(Acceleration* acc, Event* event){
    
    log("=================%f,%f,%f",acc->x,acc->y,acc->z);
    if (acc->x<0 ){
        m_pBottle->setRotation(100*acc->x);
    }
    if (acc->x<-0.5 ) {
        _addFlavor();
    }
}
#pragma mark - initData
void DonutMixFlavorScene::_initData()
{
    setExPath("content/make/donut/make2/");
}

void DonutMixFlavorScene::_showBowls()
{
    for (int i = 0; i<5; i++) {
        Node* pBowl = Node::create();
        this->addChildToContentLayer(pBowl);
        CMVisibleRect::setPositionAdapted(pBowl, 88+120*i+visibleSize.width, 730);
        pBowl->setScale(0.3);
        
        ContainerNode* pContainner = ContainerNode::create(pBowl);
        pContainner->addBody(localPath("bowl_down.png"));
        pContainner->addShadow(localPath("bowl_shadow.png"));
        pContainner->addFront(localPath("bowl_on.png"));
        pContainner->addFood(localPath("bowl_batter.png"));
        pBowl->setUserData(pContainner);
        
        m_smallBowls.push_back(pBowl);
        pBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
    }
    AudioHelp::getInstance()->playEffect("slide_sight.mp3");
//    this->runAction(Sequence::create(DelayTime::create(1),
//                                     CallFunc::create([=](){
//        AudioHelp::getInstance()->playEffect("vo_pour_mixture_bowl.mp3");
//    }), NULL));
    
}

void DonutMixFlavorScene::_showBowl(int index)
{
    AudioHelp::getInstance()->playEffect("loading_3.mp3");
    if (m_nFinishCount == 0 && !m_pBowl) {
        m_pBowl = Sprite::create(localPath("bowl_shadow.png"));
        this->addChildToContentLayer(m_pBowl);
        CMVisibleRect::setPositionAdapted(m_pBowl, 320, 380);
        
        Sprite* pBowl = Sprite::create(localPath("bowl_down.png"));
        m_pBowl->addChild(pBowl);
        pBowl->setPosition(m_pBowl->getContentSize()*0.5);
        
        m_pBowlFront = Sprite::create(localPath("bowl_on.png"));
        this->addChildToContentLayer(m_pBowlFront);
        m_pBowlFront->setLocalZOrder(10);
        m_pBowlFront->setPosition(m_pBowl->getPosition());
        
        m_pBatter = Node::create();
        m_pBowl->addChild(m_pBatter);
        m_pBatter->setPosition(m_pBowl->getContentSize()*0.5);
        
    }
    m_pBatter->removeAllChildren();
    Sprite* pBatter = Sprite::create(localPath("bowl_batter.png"));
    m_pBatter->addChild(pBatter);
    
    m_pBowl->setVisible(false);
    m_pBowlFront->setVisible(false);
    
    Node* pNode = m_smallBowls.at(index);
    pNode->runAction(Sequence::create(Spawn::create(MoveTo::create(1, m_pBowl->getPosition()),
                                                    ScaleTo::create(1, 1), NULL),
                                      Hide::create(),
                                      CallFunc::create([=](){
        m_pBowl->setVisible(true);
        m_pBowlFront->setVisible(true);
    }), NULL));
}
void DonutMixFlavorScene::_showMixTool()
{
    if (!m_pMixTool) {
        m_pMixTool = _createDrageNode(localPath("spoon1.png"));
        this->addChildToContentLayer(m_pMixTool);
        m_pMixTool->setName("tool_normal");
    }else{
        m_pMixTool->setDragSprite(Sprite::create(localPath("spoon1.png")));
    }
    m_pMixTool->setTouchEnabled(false);
    CMVisibleRect::setPositionAdapted(m_pMixTool, 420+visibleSize.width, 500);
    
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    m_pMixTool->setTag(0);
    m_pMixTool->setActionPoint(Vec2(23, 0));
    Rect rect = m_pBowl->getBoundingBox();
    rect.origin.x += 53;
    rect.origin.y += 74;
    rect.size = Size(210, 110);
    m_pMixTool->setLimitRect(rect);
    
    auto action = Sequence::create(JumpBy::create(1, Vec2(-visibleSize.width, 0),200,1),
                                   CallFuncN::create([=](Node* pNode){
        m_pMixTool->setDragSprite(Sprite::create(localPath("spoon2.png")));
        DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
        if (pIngredient) {
            pIngredient->setOrgPositionDefault();
            pIngredient->setTouchEnabled(true);
        }
    }), NULL);
    m_pMixTool->runAction(action);
}

void DonutMixFlavorScene::_showFlavorScrollView()
{
    m_typeItems.clear();
    m_nFlavorIndex = -1;
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
        
        pItem->onItemClicked = CC_CALLBACK_1(DonutMixFlavorScene::_onTypeClicked, this);
        m_typeItems.pushBack(pItem);
    }
    AudioHelp::getInstance()->playEffect("show_bar.mp3");
    m_pScrollView->runAction(Sequence::create(MoveBy::create(1, Vec2(0, 600)),
                                              CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("kids_laughing2.mp3");
        m_bItemEnable = true;
    }), NULL));
    m_pBanner->runAction(MoveBy::create(1, Vec2(0, 600)));
}

void DonutMixFlavorScene::_showBottle(int index)
{
    m_pGuideLayer->removeGuide();
    m_nFlavorIndex = index;
    if(m_pBottle){
        DragNode* pPreBottle = m_pBottle;
        pPreBottle->setTouchEnabled(false);
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
    AudioHelp::getInstance()->playEffect("loading_3.mp3");
    
    KettleNode* pContainer = KettleNode::create(m_pBottle);
    pContainer->addFront("content/make/common/flavor/bottle/color"+std::to_string(index+1)+"_2.png","front",Vec2(0, 120));
    m_pBottle->setUserData(pContainer);
    
    m_pBottle->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                          CallFunc::create([=](){
        m_pGuideLayer->showGuideRotateLeft(m_pBottle->getPosition());
        m_pBottle->setTouchEnabled(true);
        Device::setAccelerometerEnabled(true);
        m_bItemEnable = true;
        AudioHelp::getInstance()->playEffect("vo_food_coloring_bowl.mp3");
    }), NULL));
    m_pBottle->setLocalZOrder(20);
}


void DonutMixFlavorScene::_showProgressStar()
{
    m_pProgressStar = ProgressStar::create();
    this->addChildToUILayer(m_pProgressStar);
    CMVisibleRect::setPositionAdapted(m_pProgressStar, 320, 850+visibleSize.height);
    m_pProgressStar->runAction(EaseBackOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height))));
}

void DonutMixFlavorScene::_setMix()
{
    MixManager::getInstance()->reset();
    MixManager::getInstance()->_mixtureStartIndex = 0;
    MixManager::getInstance()->_mixCount = 4;
    MixManager::getInstance()->_mixSpeed = 80;
    MixManager::getInstance()->_mixRotateSpeed = 0;
    MixManager::getInstance()->_mixturePathEX = localPath("mixture/");
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
    MixManager::getInstance()->onMixFinish = CC_CALLBACK_0(DonutMixFlavorScene::_finishMix, this);
}

void DonutMixFlavorScene::_addFlavor()
{
    m_pGuideLayer->removeGuide();
    m_pBottle->setTouchEnabled(false);
    Device::setAccelerometerEnabled(false);
    Node* pLid = m_pBottle->getChildByName("front");
    if(pLid){
        AudioHelp::getInstance()->playEffect("bottle_open.mp3");
        pLid->runAction(Sequence::create(JumpBy::create(0.5, Vec2(100, 600), 100, 1),
                                         CallFunc::create([=](){
            pLid->setVisible(false);
        }), NULL));
    }
    
    if(m_nFinishCount == 0) {
        GameDataManager::getInstance()->m_sFlavorStr = std::to_string(m_nFlavorIndex);
    }else{
        GameDataManager::getInstance()->m_sFlavorStr = GameDataManager::getInstance()->m_sFlavorStr+","+std::to_string(m_nFlavorIndex);
    }
    MixManager::getInstance()->_mixturePathEX = localPath("mixture/"+std::to_string(m_nFlavorIndex+1)+"_");
    m_pBottle->runAction(Sequence::create(DelayTime::create(0.3),
                                          MoveTo::create(0.5, m_pBowl->getPosition()+Vec2(100, 150)),
                                          RotateTo::create(0.5, -80),
                                          CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("drop_vanilla.mp3");
        
//        for (int i = 0; i<8; i++) {
//            Sprite* pDrop = Sprite::create("content/make/common/flavor/drop/"+std::to_string(m_nFlavorIndex+1)+".png");
//            m_pBottle->getDragSprite()->addChild(pDrop);
//            pDrop->setPosition(m_pBottle->getActionPoint());
//            pDrop->setOpacity(0);
//            pDrop->setScale(0.1);
//            pDrop->setRotation(80);
//            pDrop->runAction(Sequence::create(DelayTime::create(0.01+i*0.1),
//                                              Spawn::create(ScaleTo::create(0.5, 0.5),
//                                                            MoveBy::create(0.6, Vec2(-100-8.0*(arc4random()%10),40-4.0*(arc4random()%5))),
//                                                            FadeIn::create(0.2), NULL),
//                                              FadeOut::create(0.3),
//                                              CallFunc::create([=](){
//                pDrop->removeFromParent();
//            }), NULL));
//        }
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/pour_vanilla.plist");
        pParticle->setTexture(Sprite::create("content/make/common/flavor/drop/"+std::to_string(m_nFlavorIndex+1)+".png")->getTexture());
        pParticle->setTotalParticles(2);
        m_pBottle->addChild(pParticle);
        pParticle->setPosition(m_pBottle->getActionPoint());
        pParticle->setName("particle");
        pParticle->setRotation(80);
        pParticle->setLocalZOrder(20);
        pParticle->setPosition(m_pBottle->convertToNodeSpace(m_pBottle->getWorldSpaceActionPoint()));
        
        
        Sprite* pAdd = Sprite::create("content/make/common/flavor/in/"+std::to_string(m_nFlavorIndex+1)+"_0.png");
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
        MenuLayer::addIngredient(MenuLayer::ePackageDonut, MenuLayer::eDonut_coloring);
        MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
        AudioHelp::getInstance()->playEffect("vo_good_job.mp3");
    }),
                                          RotateTo::create(1, 0),
                                          MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                          CallFunc::create([=](){
        _showMixTool();
        _showProgressStar();
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

void DonutMixFlavorScene::_onTypeClicked(LockItem* item)
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
}

void DonutMixFlavorScene::_finishMix()
{
    m_pProgressStar->runAction(Sequence::create(DelayTime::create(0.5),
                                                EaseBackIn::create(MoveBy::create(1, Vec2(0, visibleSize.height))),
                                                CallFunc::create([=](){
        m_pProgressStar->removeFromParent();
    }), NULL));
    AudioHelp::getInstance()->stopLoopEffect();
    AudioHelp::getInstance()->_playEffectNiceWork();
    
    m_pBowl->setVisible(false);
    m_pBowlFront->setVisible(false);
    Node* pNode = m_smallBowls.at(m_nFinishCount);
    pNode->setVisible(true);
    m_pMixTool->setTouchEnabled(false);
    m_pMixTool->runAction(JumpBy::create(1, Vec2(visibleSize.width, 0), 100, 1));
    m_pMixTool->setDragSprite(Sprite::create(localPath("spoon1.png")));
    
    ContainerNode* pContainer = (ContainerNode*) pNode->getUserData();
    if (pContainer) {
        pContainer->addFood(localPath("mixture/"+std::to_string(m_nFlavorIndex+1)+"_4.png"));
    }
    
    Vec2 pos = CMVisibleRect::getPositionAdapted(Vec2(88+120*m_nFinishCount, 730));
    m_nFinishCount++;
    pNode->runAction(Sequence::create(Spawn::create(MoveTo::create(1, pos),
                                                    ScaleTo::create(1, 0.3), NULL),
                                      CallFunc::create([=](){
        if (m_nFinishCount<5) {
            _showBowl(m_nFinishCount);
            _showFlavorScrollView();
            _setMix();
        }else{
            _finish();
        }
    }), NULL));
}

void DonutMixFlavorScene::_finish(){
    
    AudioHelp::getInstance()->stopLoopEffect();
    
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<DonutChooseShapeScene>();
    }), NULL));
}