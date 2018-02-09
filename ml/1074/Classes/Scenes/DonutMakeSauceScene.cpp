
#include "DonutMakeSauceScene.h"
#include "SceneManager.h"

DonutMakeSauceScene::DonutMakeSauceScene()
{
    m_pSauceBottle = nullptr;
    m_bItemEnable = false;
    m_nAddCount = 0;
    m_nStep = 0;
    m_nShakeCount = 0;
}

DonutMakeSauceScene::~DonutMakeSauceScene()
{
    
}
bool DonutMakeSauceScene::init()
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
    
    _showBowl();
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        _showTypeItemScroll();
//        _finishMix();
//        _showDonut();
        
    }), NULL));
    
    auto eventAcc = EventListenerAcceleration::create(CC_CALLBACK_2(DonutMakeSauceScene::onAcceleration, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventAcc, this);
    
    m_pTouchLayer = TouchLayer::create();
    this->addChildToUILayer(m_pTouchLayer);
    m_pTouchLayer->onTouchEnded = CC_CALLBACK_2(DonutMakeSauceScene::TouchEnded, this);
    m_pTouchLayer->setEnable(false);
    m_pGameUI->showNormalLayout();
    return true;
}

void DonutMakeSauceScene::onEnter()
{
    ExtensionScene::onEnter();
}

void DonutMakeSauceScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    if (IAPManager::getInstance()->getItemIsBought(2)) {
        for_each(m_typeItems.begin(), m_typeItems.end(), [=](LockItem* pItem){
            pItem->unlock();
        });
    }
}

void DonutMakeSauceScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
}

void DonutMakeSauceScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name == "spoon"){
        if (pDragNode->index == 0) {
            Rect rect = m_pSauceBottle->getDragSprite()->getBoundingBox();
            rect.origin = m_pSauceBottle->convertToWorldSpace(rect.origin);
            if (rect.containsPoint(worldPoint)) {
                pDragNode->setTouchEnabled(false);
                m_pGuideLayer->removeGuide();
                _FullSpoonSauce(m_nSauceIndex);
            }
        }else{
            Rect rect = m_pBowl->getBoundingBox();
            if (rect.containsPoint(worldPoint)){
                pDragNode->setTouchEnabled(false);
                m_pGuideLayer->removeGuide();
                _pourSauce(m_nSauceIndex);
            }
        }
    }else if (name == "sugar"){
        Rect rect = m_pSieve->getWorldRect();
        if (rect.containsPoint(worldPoint)){
            pDragNode->setTouchEnabled(false);
            m_pGuideLayer->removeGuide();
            _pourIcingSugar();
        }
        
    }else if (name == "tool_normal"){
        pDragNode->setLocalZOrder(8);
        Rect bowlRect = m_pBowl->getBoundingBox();
        if (bowlRect.containsPoint(worldPoint)) {
            AudioHelp::getInstance()->playLoopEffect("stir_spoon.mp3");
            MixManager::getInstance()->mix();
        }else{
            AudioHelp::getInstance()->stopLoopEffect();
            m_pBowl->stopAllActions();
        }
    }else if (name == "donut"){
        Rect rect = m_pBowl->getBoundingBox();
        if (rect.containsPoint(worldPoint)){
            pDragNode->setTouchEnabled(false);
            m_pGuideLayer->removeGuide();
            _addSauce2Donut();
        }
        
    }
}

void DonutMakeSauceScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    AudioHelp::getInstance()->stopLoopEffect();
    std::string name = pDragNode->getName();
    if (name == "sauce_bottle") {
        pDragNode->setTouchEnabled(false);
        m_bItemEnable = false;
        m_pScrollView->runAction(Sequence::create(MoveBy::create(1, Vec2(0, -500)), NULL));
        m_pBanner->runAction(Sequence::create(MoveBy::create(1, Vec2(0, -500)),
                                              CallFunc::create([=](){
            m_pBanner->removeFromParent();
            m_pBanner = nullptr;
        }), NULL));
        m_pGuideLayer->removeGuide();
        int tag = pDragNode->getTag();
        if (tag<5) {
//            jam
            Node* pLid = pDragNode->getChildByName("cap");
            if(pLid){
                pLid->runAction(Sequence::create(JumpBy::create(1, Vec2(100, 600), visibleSize.height, 1),
                                                 CallFunc::create([=](){
                    _showSpoon();
                    pLid->setVisible(false);
                }), NULL));
            }
        }else{
            pDragNode->setTouchEnabled(false);
            Device::setAccelerometerEnabled(false);
            _pourSauce(tag);
        }
    }
}

void DonutMakeSauceScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    dragNodeTouchEnded(pDragNode,worldPoint);
}

void DonutMakeSauceScene::TouchEnded(Touch *pTouch, Event *pEvent)
{
    Rect rectBowl = m_pSieve->getWorldRect();
    if (!rectBowl.containsPoint(pTouch->getLocation())){
        return;
    }
    
    Device::setAccelerometerEnabled(false);
    m_pTouchLayer->setEnable(false);
    m_pSieve->separatePowder();
    _separatePowder();
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        m_pSieve->stopSeparate();
    }), NULL));
}
void DonutMakeSauceScene::onAcceleration(Acceleration* acc, Event* event){
    
    log("=================%f,%f,%f",acc->x,acc->y,acc->z);
    if(m_nStep==0){
        if (acc->x<0 ){
            m_pSauceBottle->setRotation(100*acc->x);
        }
        if (acc->x<-0.5 ) {
            m_pSauceBottle->setTouchEnabled(false);
            Device::setAccelerometerEnabled(false);
            _pourSauce(m_nSauceIndex);
        }
    }else if(m_nStep==1){
        if (abs(acc->x)>0.3 ) {
            m_pSieve->separatePowder();
            m_nShakeCount++;
            if(m_nShakeCount%10==0){
                Device::setAccelerometerEnabled(false);
                m_pTouchLayer->setEnable(false);
            }
            this->runAction(Sequence::create(DelayTime::create(1),
                                             CallFunc::create([=](){
                Device::setAccelerometerEnabled(true);
                m_pTouchLayer->setEnable(true);
                m_pSieve->stopSeparate();
            }), NULL));
            if(m_nShakeCount%40==0){
                _separatePowder();
            }
        }
    }
}
#pragma mark - initData
void DonutMakeSauceScene::_initData()
{
    setExPath("content/make/donut/make3/");
    m_nShapeIndex = GameDataManager::getInstance()->m_nShapeIndex;
}

void DonutMakeSauceScene::_showBowl()
{
    m_pBowl = Sprite::create(localPath("bowl_down.png"));
    this->addChildToContentLayer(m_pBowl);
    CMVisibleRect::setPositionAdapted(m_pBowl, 210-visibleSize.width, 380);
    m_pBowl->runAction(MoveBy::create(1.0,Vec2(visibleSize.width,0)));
    
    m_pBowlFront = Sprite::create(localPath("bowl_on.png"));
    this->addChildToContentLayer(m_pBowlFront);
    m_pBowlFront->setLocalZOrder(10);
    m_pBowlFront->setPosition(m_pBowl->getPosition());
    m_pBowlFront->runAction(MoveBy::create(1.0,Vec2(visibleSize.width,0)));
    
    m_pBatter = Node::create();
    m_pBowl->addChild(m_pBatter);
    m_pBatter->setPosition(Vec2(m_pBowl->getContentSize().width*0.5,m_pBowl->getContentSize().height*0.5));
    
    m_pBatterClipping = ClippingNode::create(Sprite::create(localPath("bowl_mask.png")));
    m_pBowl->addChild(m_pBatterClipping);
    m_pBatterClipping->setAlphaThreshold(0.5);
    m_pBatterClipping->setPosition(Vec2(m_pBowl->getContentSize().width*0.5,m_pBowl->getContentSize().height*0.5));
    
    m_pBatter = Node::create();
    m_pBatterClipping->addChild(m_pBatter);
//    m_pBatter->setPosition(Vec2(m_pBowl->getContentSize().width*0.5,m_pBowl->getContentSize().height*0.5));
}

void DonutMakeSauceScene::_showTypeItemScroll()
{
    m_pScrollView = ExpandScrollView::create();
    m_pScrollView->setContentSize(Size(visibleSize.width,200));
    this->addChildToUILayer(m_pScrollView);
    CMVisibleRect::setPositionAdapted(m_pScrollView, 0, 120-600,kBorderLeft,kBorderBottom);
    m_pScrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    m_pScrollView->setLocalZOrder(5);
    m_pScrollView->setScrollBarEnabled(false);
    m_pScrollView->setClippingEnabled(false);
    
    m_pBanner = Sprite::create("content/category/icon/fruit_banner.png");
    this->addChildToUILayer(m_pBanner);
    m_pBanner->setPosition(m_pScrollView->getPosition()+m_pBanner->getAnchorPointInPoints());
    
    
    int typeCount = 10;
    m_pScrollView->getInnerContainer()->setContentSize(Size((typeCount+1)*150, 200));
    for (int i = 0; i<typeCount; i++) {
        LockItem* pItem = LockItem::create();
        
        bool bLock = false;
        if (!gNoneIap && !IAPManager::getInstance()->getItemIsBought(2) && i%2==1){
            if (!ConfigManager::getInstance()->getVideoUnLocked("donut_sauce", i)) {
                LockUIManager::getInstance()->registerLock("donut_sauce", i, pItem);
                LockUIManager::getInstance()->setLockOriginPos(Vec2(40, -60));
                pItem->lockType = 1;
                bLock = true;
            }
        }
        if (i<5) {
            pItem->initWithName(localPath("jam/bottle_down.png"), "content/common/video.png",bLock);
        }else{
            pItem->initWithName(localPath("sauce/"+std::to_string(i-4)+".png"), "content/common/video.png",bLock);
        }
        Sprite* Lock = pItem->getLockSprite();
        if (Lock) {
            Lock->setTag(TAG_REWARD_LOCK);
            Lock->setLocalZOrder(30);
            Lock->setScale(2);
        }
        
        KettleNode* pContainer = KettleNode::create(pItem);
        pItem->setUserData(pContainer);
        _setContainer(pContainer, i);
        
        pItem->setTag(i);
        m_pScrollView->getInnerContainer()->addChild(pItem);
        pItem->setPosition(Vec2(100+i*150,100));
        pItem->setScale(0.5);
        
        pItem->onItemClicked = CC_CALLBACK_1(DonutMakeSauceScene::_onTypeClicked, this);
        m_typeItems.pushBack(pItem);
    }
    m_pBanner->runAction(MoveBy::create(1, Vec2(0, 600)));
    m_pScrollView->runAction(Sequence::create(MoveBy::create(1, Vec2(0, 600)),
                                              CallFunc::create([=](){
        m_bItemEnable = true;
        AudioHelp::getInstance()->playEffect("vo_make_donut_frosty.mp3");
    }), NULL));
    AudioHelp::getInstance()->playEffect("show_bar.mp3");

}

void DonutMakeSauceScene::_showBottle(int index)
{
    m_pGuideLayer->removeGuide();
    //        show sauce
    if (index>=5) {
        AudioHelp::getInstance()->playEffect("vo_chocolate_syrup.mp3");
    }
    if(m_pSauceBottle){
        m_pSauceBottle->setTouchEnabled(false);
        Device::setAccelerometerEnabled(false);
        DragNode* pPreBottle = m_pSauceBottle;
        pPreBottle->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                               CallFunc::create([=](){
            pPreBottle->removeFromParent();
        }), NULL));
        
    }
    
    if (index<5) {
        m_pSauceBottle = _createDrageNode(localPath("jam/bottle_on.png"));
        CMVisibleRect::setPositionAdapted(m_pSauceBottle, 510+visibleSize.width, 590);
    }else{
        m_pSauceBottle = _createDrageNode(localPath("sauce/"+std::to_string(index-4)+".png"));
        CMVisibleRect::setPositionAdapted(m_pSauceBottle, 460+visibleSize.width, 500);
        m_pSauceBottle->setActionPoint(Vec2(81, 318));
    }
    this->addChildToContentLayer(m_pSauceBottle);
    m_pSauceBottle->setName("sauce_bottle");
    m_pSauceBottle->setMovable(false);
    KettleNode* pContainer = KettleNode::create(m_pSauceBottle);
    _setContainer(pContainer, index);
    m_pSauceBottle->setUserData(pContainer);
    m_pSauceBottle->setTouchEnabled(false);
    m_pSauceBottle->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                               CallFunc::create([=](){
        m_pSauceBottle->setTouchEnabled(true);
        if(index>=5){
//            m_pGuideLayer->showGuideShake(m_pSauceBottle->getPosition());
            m_pGuideLayer->showGuideRotateLeft(m_pSauceBottle->getPosition());
            Device::setAccelerometerEnabled(true);
        }else{
            m_pGuideLayer->showGuideTap(m_pSauceBottle->getPosition());
        }
        m_bItemEnable = true;
    }), NULL));
    
    m_pSauceBottle->setTag(index);
    m_pSauceBottle->getDragSprite()->setVisible(false);
    m_pSauceBottle->setLocalZOrder(20);
}

void DonutMakeSauceScene::_showSpoon()
{
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    m_pSpoon = _createDrageNode(localPath("spoon_down.png"));
    this->addChildToContentLayer(m_pSpoon);
    CMVisibleRect::setPositionAdapted(m_pSpoon, 360+visibleSize.width, 650);
    m_pSpoon->setName("spoon");
    m_pSpoon->setTouchEnabled(false);
    
    KettleNode* pContainer = KettleNode::create(m_pSpoon);
    pContainer->addFront(localPath("spoon_on.png"));
    m_pSpoon->setUserData(pContainer);
    
    m_pSpoon->runAction(Sequence::create(EaseBackOut::create(JumpBy::create(1, Vec2(-visibleSize.width, 0), 200, 1)),
                                         CallFunc::create([=](){
        m_pSpoon->setOrgPositionDefault();
        m_pGuideLayer->showGuideMove(m_pSpoon->getPosition(), m_pSauceBottle->getPosition());
        m_pSpoon->setTouchEnabled(true);
        AudioHelp::getInstance()->playEffect("vo_jam.mp3");
    }), NULL));
}

void DonutMakeSauceScene::_showSieve()
{
    m_pSieve = SieveNode::create();
    this->addChildToContentLayer(m_pSieve);
    m_pSieve->setSieve("content/make/common/sleve/sieve_down.png", "content/make/common/sleve/sieve_on.png");
    CMVisibleRect::setPositionAdapted(m_pSieve, 280-visibleSize.width, 600);
    m_pSieve->setPowderRect(Rect(-170, 0, 200, 10));
    m_pSieve->runAction(MoveBy::create(1.0,Vec2(visibleSize.width,0)));
    
    auto eventAcc = EventListenerAcceleration::create(CC_CALLBACK_2(DonutMakeSauceScene::onAcceleration, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventAcc, this);
}

void DonutMakeSauceScene::_showIcingSugar()
{
    m_pIcingSugar = _createDrageNode(localPath("icing_sugar.png"));
    this->addChildToContentLayer(m_pIcingSugar);
    CMVisibleRect::setPositionAdapted(m_pIcingSugar, 500+visibleSize.width, 500);
    m_pIcingSugar->setName("sugar");
    m_pIcingSugar->setTouchEnabled(false);
    m_pIcingSugar->setActionPoint(Vec2(51, 248));
    
    KettleNode* pContainer = KettleNode::create(m_pIcingSugar);
    pContainer->addShadow(localPath("icing_sugar_shadow.png"));
    m_pIcingSugar->setUserData(pContainer);
    
    m_pIcingSugar->runAction(Sequence::create(EaseBackOut::create(JumpBy::create(1, Vec2(-visibleSize.width, 0), 200, 1)),
                                         CallFunc::create([=](){
        m_pIcingSugar->setOrgPositionDefault();
        m_pGuideLayer->showGuideMove(m_pIcingSugar->getPosition(), m_pSieve->getPosition());
        m_pIcingSugar->setTouchEnabled(true);
    }), NULL));
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_then_using_sieve.mp3");
    }),
                                     DelayTime::create(2),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_icing_sugar.mp3");
    }), NULL));
}

void DonutMakeSauceScene::_showMixTool()
{
    m_pMixTool = _createDrageNode("content/make/common/mix/agitator1_1.png");
    this->addChildToContentLayer(m_pMixTool);
    m_pMixTool->setName("tool_normal");
    m_pMixTool->setTouchEnabled(false);
    CMVisibleRect::setPositionAdapted(m_pMixTool, 420+visibleSize.width, 500);
    
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    m_pMixTool->setTag(0);
    m_pMixTool->setActionPoint(Vec2(66, 74));
    Rect rect = m_pBowl->getBoundingBox();
    rect.origin.x += 81;
    rect.origin.y += 85;
    rect.size = Size(200, 100);
    m_pMixTool->setLimitRect(rect);
    
    auto action = Sequence::create(JumpBy::create(1, Vec2(-visibleSize.width, 0),200,1),
                                   CallFuncN::create([=](Node* pNode){
        m_pMixTool->setDragSprite(Sprite::create("content/make/common/mix/agitator1_2.png"));
        DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
        if (pIngredient) {
            pIngredient->setOrgPositionDefault();
            pIngredient->setTouchEnabled(true);
        }
    }), NULL);
    m_pMixTool->runAction(action);
}

void DonutMakeSauceScene::_showProgressStar()
{
    m_pProgressStar = ProgressStar::create();
    this->addChildToUILayer(m_pProgressStar);
    CMVisibleRect::setPositionAdapted(m_pProgressStar, 320, 850+visibleSize.height);
    m_pProgressStar->runAction(EaseBackOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height))));
}

void DonutMakeSauceScene::_setContainer(KettleNode* pContainer,int flavorIndex)
{
    if (flavorIndex<5) {
        pContainer->addShadow(localPath("jam/bottle_shadow.png"));
        pContainer->addBody(localPath("jam/bottle_down.png"));
        pContainer->addFront(localPath("jam/bottle_on.png"));
        pContainer->addFront(localPath("jam/label"+std::to_string(flavorIndex+1)+".png"),"label",Vec2(0,-30));
        pContainer->addFront(localPath("jam/bottle_on1.png"),"cap",Vec2(0,60));
        pContainer->addWater(localPath("jam/"+std::to_string(flavorIndex+1)+".png"),localPath("jam/iam_mask.png"));
    }else{
        pContainer->addShadow(localPath("sauce/shadow.png"));
        pContainer->addBody(localPath("sauce/"+std::to_string(flavorIndex-4)+".png"));
        pContainer->addFront(localPath("sauce/"+std::to_string(flavorIndex-4)+"_1.png"));
    }
    
}
void DonutMakeSauceScene::_showDonut()
{
    AudioHelp::getInstance()->playEffect("present_in.mp3");
    m_pDonutPlate = Sprite::create(localRootPath("dec/plate.png"));
    this->addChildToContentLayer(m_pDonutPlate);
    CMVisibleRect::setPositionAdapted(m_pDonutPlate, 530+visibleSize.width, 370);
    m_pDonutPlate->setScale(0.5);
    
    m_pDonut = _createDrageNode(localRootPath("dec/"+std::to_string(m_nShapeIndex)+"/1.png"));
    this->addChildToContentLayer(m_pDonut);
    CMVisibleRect::setPositionAdapted(m_pDonut, 530+visibleSize.width, 390);
    m_pDonut->setLocalZOrder(10);
    m_pDonut->setScale(0.5);
    m_pDonut->getDragSprite()->setVisible(false);
    m_pDonut->setName("donut");
    
    std::string flavorStr = GameDataManager::getInstance()->m_sFlavorStr;
    std::vector<std::string> flavorVector = StringHelp::split(flavorStr, ",");
    for (int i = 0; i<5; i++) {
        int flavorIndex = atoi(flavorVector.at(i).c_str());
        _addBatter(flavorIndex, i);
    }
    m_pDonut->setTouchEnabled(false);
    m_pDonutPlate->runAction(Sequence::create(DelayTime::create(0.5),
                                              MoveBy::create(1, Vec2(-visibleSize.width, 0)),NULL));
    
    m_pDonut->runAction(Sequence::create(DelayTime::create(0.5),
                                         MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                         CallFunc::create([=](){
        m_pGuideLayer->showGuideMove(m_pDonut->getPosition(), m_pBowl->getPosition());
        m_pDonut->setOrgPositionDefault();
        m_pDonut->setTouchEnabled(true);
    }), NULL));
}

void DonutMakeSauceScene::_addBatter(int flavorIndex,int maskIndex)
{
    
    Sprite* pStencil = Sprite::create(localRootPath("dec/"+std::to_string(m_nShapeIndex)+"/mask/"+std::to_string(maskIndex+1)+".png"));
    Sprite* pSprite = Sprite::create(localRootPath("dec/"+std::to_string(m_nShapeIndex)+"/"+std::to_string(flavorIndex+1)+".png"));
    
    pStencil->setBlendFunc((ccBlendFunc){GL_ZERO,GL_SRC_ALPHA});
    pSprite->setBlendFunc((ccBlendFunc){GL_ONE,GL_ZERO});
    pStencil->setPosition(pStencil->getContentSize()*0.5);
    pSprite->setPosition(pStencil->getContentSize()*0.5);
    
    MyRenderTexture* pRT = MyRenderTexture::create(pStencil->getContentSize().width, pStencil->getContentSize().height);
    pStencil->setPosition(pStencil->getContentSize()*0.5);
    pRT->beginWithClear(1, 1, 1, 0);
    pSprite->visit();
    pStencil->visit();
    pRT->end();
    Director::getInstance()->getRenderer()->render();
    pStencil->setPosition(Vec2::ZERO);
    
    __NotificationCenter::getInstance()->removeObserver(pRT, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(pRT, EVENT_COME_TO_FOREGROUND);
    
    pRT->setLocalZOrder(6-maskIndex);
    m_pDonut->addChild(pRT);
}

void DonutMakeSauceScene::_onTypeClicked(LockItem* item)
{
    if (!m_bItemEnable) {
        return;
    }
    if (m_pScrollView->isScrolled()) {
        return;
    }
    int tag = item->getTag();
    if (item->isLocked()){
        if(!ConfigManager::getInstance()->getVideoUnLocked("donut_sauce", tag)){
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
                RewardManager::getInstance()->showRewardAds("donut_sauce", tag);
                pLayer->removeFromParent();
            };
            return;
        }
    }
    AudioHelp::getInstance()->playSelectedEffect();
    AudioHelp::getInstance()->_playEffectGoodChooice();
    item->runAction(Sequence::create(ScaleTo::create(0.13, 1.0*0.5, 0.90*0.5),
                                     ScaleTo::create(0.11, 0.92*0.5, 1.0*0.5),
                                     ScaleTo::create(0.10, 1.0*0.5, 0.96*0.5),
                                     ScaleTo::create(0.09, 0.98*0.5, 1.0*0.5),
                                     ScaleTo::create(0.08, 1.0*0.5, 0.99*0.5),
                                     ScaleTo::create(0.07, 0.99*0.5, 1.0*0.5), NULL));
    m_nSauceIndex = tag;
    _showBottle(tag);
    m_bItemEnable = false;
}

void DonutMakeSauceScene::_FullSpoonSauce(int index)
{
    
    m_pSpoon->runAction(Sequence::create(JumpTo::create(0.5, m_pSauceBottle->getPosition()+Vec2(70, 130), 200, 1),
                                          CallFunc::create([=](){
        m_pSpoon->setVisible(false);
        Node* pTemp = Node::create();
        KettleNode* pContainer = KettleNode::create(pTemp);
        pContainer->addBody(localPath("spoon_down.png"));
        pContainer->addFront(localPath("spoon_on.png"));
        m_pSauceBottle->addChild(pTemp);
        //            pTemp->setPosition(Vec2(30, 40));
        pTemp->setPosition(m_pSauceBottle->convertToNodeSpace(m_pSpoon->getPosition()));
        pTemp->runAction(Sequence::create(Spawn::create(RotateBy::create(0.5, -80),
                                                        MoveTo::create(0.5, Vec2(30, 50)), NULL),
                                          CallFunc::create([=](){
            pContainer->addWater(localPath("jam/"+std::to_string(m_nSauceIndex+1)+".png"), localPath("spoon_mask.png"));
            pContainer->setWaterPos(Vec2(-100, 0));
            
            KettleNode* pContainerBottle = (KettleNode*)m_pSauceBottle->getUserData();
            pContainerBottle->updateWaterOnce(5*(m_nSauceIndex+1));
            AudioHelp::getInstance()->playEffect("spoon_icecream.mp3");
        }),
                                          RotateBy::create(0.5, 10),
                                          DelayTime::create(0.3),
                                          MoveTo::create(0.5, Vec2(30, 200)),
                                          RotateTo::create(0.5, 0),
                                          CallFunc::create([=](){
            KettleNode* pContainerSpoon = (KettleNode*)m_pSpoon->getUserData();
            pContainerSpoon->addWater(localPath("jam/"+std::to_string(m_nSauceIndex+1)+".png"), localPath("spoon_mask.png"));
            pContainerSpoon->setWaterPos(Vec2(-100, 0));
            m_pSpoon->setVisible(true);
            m_pSpoon->index = 1;
            m_pGuideLayer->removeGuide();
            m_pGuideLayer->showGuideMove(m_pSpoon->getPosition(), m_pBowl->getPosition());
            m_pSpoon->setTouchEnabled(true);
            pTemp->removeFromParent();
            
        }), NULL));
        
    }), NULL));
}

void DonutMakeSauceScene::_pourSauce(int index)
{
    if (index<5) {
        
        m_pSpoon->runAction(Sequence::create(Spawn::create(MoveTo::create(0.5, m_pBowl->getPosition()+Vec2(60, 160)),
                                                            RotateBy::create(0.5, -50),
                                                            ScaleTo::create(0.5, 1.3), NULL),
                                              CallFunc::create([=](){
            KettleNode* pContainerSpoon = (KettleNode*)m_pSpoon->getUserData();
            pContainerSpoon->pourWater(0.5);
            AudioHelp::getInstance()->playEffect("liquid_hit.mp3");
            
            if (m_nAddCount==0) {
                ClippingNode* pNode;
                Sprite* pAdd;
                if (m_nSauceIndex<5) {
                    pNode = ClippingNode::create(Sprite::create(localPath("jam_mask.png")));
                    pAdd = Sprite::create(localPath("jam"+std::to_string(m_nSauceIndex+1)+".png"));
                }else{
                    pNode = ClippingNode::create(Sprite::create(localPath("sauce_mask.png")));
                    pAdd = Sprite::create(localPath("sauce"+std::to_string(m_nSauceIndex-4)+".png"));
                }
                pNode->setAlphaThreshold(0.5);
                m_pBatter->addChild(pNode);
                pNode->addChild(pAdd);
                pNode->setScale(0.4);
                pNode->setPosition(Vec2(0,-50));
                pNode->runAction(ScaleTo::create(0.5, 1));
            }else{
                Sprite* pAdd;
                if (m_nSauceIndex<5) {
                    pAdd = Sprite::create(localPath("jam"+std::to_string(m_nSauceIndex+1)+".png"));
                }else{
                    pAdd = Sprite::create(localPath("sauce"+std::to_string(m_nSauceIndex-4)+".png"));
                }
                
                m_pBatter->addChild(pAdd);
                pAdd->setScale(0.4);
                pAdd->setPosition(Vec2(0,-50));
                pAdd->runAction(ScaleTo::create(0.5, 1));
            }
        }),
                                              DelayTime::create(0.8),
                                              Spawn::create(ScaleTo::create(0.5, 1),
                                                            RotateTo::create(0.5, 0),
                                                            MoveTo::create(0.5, m_pBowl->getPosition()+Vec2(200, 230)), NULL),
                                              CallFunc::create([=](){
            m_nAddCount++;
            if (m_nAddCount == 2) {
                m_pSpoon->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
                m_pSauceBottle->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
                _finishAddSauce();
                MenuLayer::addIngredient(MenuLayer::ePackageDonut, MenuLayer::eDonut_jam);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
//                AudioHelp::getInstance()->playEffect("vo_sweet.mp3");
            }else{
                m_pSpoon->index = 0;
                m_pSpoon->setTouchEnabled(true);
            }
        }), NULL));
    }else{
        m_pSauceBottle->setTouchEnabled(false);
        Device::setAccelerometerEnabled(false);
        m_pScrollView->runAction(Sequence::create(MoveBy::create(1, Vec2(0, -500)), NULL));
        m_pBanner->runAction(Sequence::create(MoveBy::create(1, Vec2(0, -500)),
                                              CallFunc::create([=](){
            m_pBanner->removeFromParent();
            m_pBanner = nullptr;
        }), NULL));
        m_pGuideLayer->removeGuide();
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
            AudioHelp::getInstance()->playEffect("add_syrup.mp3");
            
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/pour_vanilla.plist");
//            pParticle->setTotalParticles(2);
            pParticle->setTexture(Sprite::create(localPath("sauce/1/"+std::to_string(index-4)+".png"))->getTexture());
            m_pSauceBottle->addChild(pParticle);
            pParticle->setPosition(m_pSauceBottle->getActionPoint());
            pParticle->setName("particle");
            pParticle->setRotation(90);
            pParticle->setLocalZOrder(20);
            pParticle->setPosition(m_pSauceBottle->convertToNodeSpace(m_pSauceBottle->getWorldSpaceActionPoint()));
            
            Sprite* pAdd;
            if (m_nSauceIndex<5) {
                pAdd = Sprite::create(localPath("jam"+std::to_string(m_nSauceIndex+1)+".png"));
            }else{
                pAdd = Sprite::create(localPath("sauce"+std::to_string(m_nSauceIndex-4)+".png"));
            }
            
            m_pBatter->addChild(pAdd);
            pAdd->setScale(0.4);
            pAdd->setPosition(Vec2(0,-50));
            pAdd->setOpacity(0);
            pAdd->runAction(Sequence::create(FadeIn::create(0.5),
                                             ScaleTo::create(1, 1), NULL));
        }),
                                                   DelayTime::create(2),
                                                   CallFunc::create([=](){
            
//            auto particle = dynamic_cast<ParticleSystem*>(m_pSauceBottle->getChildByName("particle"));
//            if (particle) {
//                particle->stopSystem();
//                particle->removeFromParent();
//            }
            
        }),
                                                   RotateTo::create(0.3, 0),
                                                   MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                                   CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("vo_yummy.mp3");
            MenuLayer::addIngredient(MenuLayer::ePackageDonut, MenuLayer::eDonut_syrup);
            MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
            _finishAddSauce();
        }), NULL));
    }
}

void DonutMakeSauceScene::_pourIcingSugar()
{
    
    m_pIcingSugar->runAction(Sequence::create(MoveTo::create(0.5,m_pSieve->getPosition()+Vec2(120, 100)),
                                            RotateTo::create(0.5,-90),
                                              CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("pour_corn_kernal.mp3");
        
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/pour_sugar.plist");
        m_pIcingSugar->addChild(pParticle);
        pParticle->setPosition(m_pIcingSugar->getActionPoint());
        pParticle->setName("particle");
        pParticle->setRotation(90);
        pParticle->setLocalZOrder(20);
//        pParticle->setScaleX(-1);
        pParticle->setPosition(m_pIcingSugar->convertToNodeSpace(m_pIcingSugar->getWorldSpaceActionPoint()));
        
        
        m_pIcingSugar->getDragSprite()->setTexture(localPath("icing_sugar1.png"));
        m_pSieve->setPowder("content/make/common/sleve/sieve_sugar.png",2);
    }),
                                            CircleMove::create(2,m_pSieve->getPosition()+Vec2(50, 150),60,180),
                                            CallFunc::create([=](){
        auto particle = dynamic_cast<ParticleSystem*>(m_pIcingSugar->getChildByName("particle"));
        if (particle) {
            particle->stopSystem();
            particle->removeFromParent();
        }
        AudioHelp::getInstance()->playEffect("kids_laughing2.mp3");
    }),
                                            RotateTo::create(1, 0),
                                            MoveBy::create(1, Vec2(visibleSize.width, 0)),
                                            CallFunc::create([=](){
        
        _finishAddIcingSugar();
        m_pIcingSugar->removeFromParent();
    }), NULL));
}

void DonutMakeSauceScene::_separatePowder()
{
    m_pGuideLayer->removeGuide();
    m_pSieve->powderFadeOut(2);
    Sprite* pAdd = Sprite::create(localPath("bowl_icing.png"));
    if (pAdd) {
        pAdd->setPosition(Vec2(0,-30));
        pAdd->setOpacity(0);
        m_pBatter->addChild(pAdd,5);
        pAdd->runAction(Sequence::create(FadeIn::create(2),
                                         CallFunc::create([=](){
        }), NULL));
    }
    
    this->stopAllActions();
    Device::setAccelerometerEnabled(false);
    m_pTouchLayer->setEnable(false);
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        m_pSieve->stopSeparate();
        AudioHelp::getInstance()->playEffect("vo_well_done.mp3");
    }),
                                     DelayTime::create(1),
                                     CallFunc::create([=](){
        m_pSieve->runAction(MoveBy::create(1.0,Vec2(visibleSize.width,0)));
        _showMixTool();
        MenuLayer::addIngredient(MenuLayer::ePackageDonut, MenuLayer::eDonut_icing_sugar);
        MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
        _showProgressStar();
//        _finishAdd();
    }), NULL));
}

void DonutMakeSauceScene::_addSauce2Donut()
{
    m_pDonut->runAction(Sequence::create(MoveTo::create(0.5, m_pBowl->getPosition()+Vec2(50, 100)),
                                         CallFunc::create([=](){        
        m_pDonut->setLocalZOrder(5);
    }),
                                         Spawn::create(JumpTo::create(1, m_pBowl->getPosition(), 150, 1),
                                                       RotateBy::create(0.8, Vec3(0, -180, 0)),
                                                       NULL),
                                         CallFunc::create([=](){
        
        m_pDonut->setRotation3D(Vec3(0, 0, 0));
        m_pDonut->setScaleX(-m_pDonut->getScaleX());
        AudioHelp::getInstance()->playEffect("liquid_hit.mp3");
        for (int i = 0; i<2; i++) {
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dip.plist");
            if (m_nSauceIndex<5) {
                pParticle->setTexture(Sprite::create(localPath("bowl_jam/"+std::to_string(m_nSauceIndex+1)+"_"+std::to_string(i+1)+".png"))->getTexture());
            }else{
                pParticle->setTexture(Sprite::create(localPath("bowl_sauce/"+std::to_string(m_nSauceIndex-4)+"_"+std::to_string(i+1)+".png"))->getTexture());
            }
            this->addChildToContentLayer(pParticle);
            pParticle->setLocalZOrder(4);
            pParticle->setPosition(m_pBowl->getPosition());
        }
    }),
                                         DelayTime::create(1),
                                         Spawn::create(JumpTo::create(2, m_pDonut->getOrgPosition(), 200, 1),
                                                       Sequence::create(RotateBy::create(1, Vec3(0, 90, 0)),
                                                                        CallFunc::create([=](){
                                                           std::ostringstream ostr;
                                                           if (m_nSauceIndex<5) {
                                                               ostr<<"dec/"<<m_nShapeIndex<<"/jam"<<m_nSauceIndex+1<<".png";
                                                           }else{
                                                               ostr<<"dec/"<<m_nShapeIndex<<"/sauce"<<m_nSauceIndex-4<<".png";
                                                           }
                                                           Sprite* pIcing = Sprite::create(localRootPath(ostr.str()));
                                                           m_pDonut->addChild(pIcing);
                                                           pIcing->setLocalZOrder(10);
                                                           pIcing->setOpacity(0);
                                                           pIcing->runAction(FadeIn::create(0.5));
                                                           pIcing->setName("icing");
                                                       }),
                                                                        RotateBy::create(1, Vec3(0, 90, 0)),NULL),
                                                       CallFunc::create([=](){
                                         }), NULL),
                                         CallFunc::create([=](){
        m_pDonut->setScaleX(-m_pDonut->getScaleX());
        AudioHelp::getInstance()->playEffect("vo_well_done.mp3");
        m_pDonut->setRotation3D(Vec3(0, 0, 0));
        m_pBowl->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
        m_pBowlFront->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
        m_pDonutPlate->runAction(Spawn::create(MoveBy::create(1, Vec2(-200, 50)),
                                               ScaleTo::create(1, 1), NULL));
        m_pDonut->runAction(Sequence::create(Spawn::create(MoveBy::create(1, Vec2(-200, 50)),
                                                           ScaleTo::create(1, 1), NULL),
                                             CallFunc::create([=](){
            _finish();
        }), NULL));
    }), NULL));
}

void DonutMakeSauceScene::_saveDonut()
{
    
    Size cakeSize = Sprite::create(localRootPath("dec/"+std::to_string(m_nShapeIndex)+"/1.png"))->getContentSize();
    RenderTexture* render = RenderTexture::create(cakeSize.width, cakeSize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    Vec2 posDec = m_pDonut->getPosition();
    m_pDonut->setPosition(Vec2(cakeSize.width/2, cakeSize.height/2));
    Node* pIcing = m_pDonut->getChildByName("icing");
    if (pIcing) {
        pIcing->setVisible(false);
    }
    render->beginWithClear(1.0f, 1.0f, 1.0f, 0.0f);
    m_pDonut->visit();
    render->end();
    
    Director::getInstance()->getRenderer()->render();
    m_pDonut->setPosition(posDec);
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    if (pIcing) {
        pIcing->setVisible(true);
    }
    Image* pImage = render->newImage();
    
    bool issuccess;
    
    issuccess = pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+"donut.png", false);
    pImage->autorelease();
}
void DonutMakeSauceScene::_finishAddSauce()
{
    AudioHelp::getInstance()->playEffect("vo_talented.mp3");
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        _showSieve();
        _showIcingSugar();
    }), NULL));
    m_nStep = 1;
}

void DonutMakeSauceScene::_finishAddIcingSugar()
{
//    m_pGuideLayer->showGuideShake(m_pSieve->getPosition());
    m_pGuideLayer->showGuideTap(m_pSieve->getPosition());
    m_pSieve->runAction(Sequence::create(MoveBy::create(0.5, Vec2(100, 0)),
                                         CallFunc::create([=](){
        
        Device::setAccelerometerEnabled(true);
        m_pTouchLayer->setEnable();
        
    }), NULL));
    m_pBowl->runAction(MoveBy::create(1.0,Vec2(100,0)));
    m_pBowlFront->runAction(MoveBy::create(1.0,Vec2(100,0)));
    
    MixManager::getInstance()->reset();
    MixManager::getInstance()->_mixtureStartIndex = 0;
    MixManager::getInstance()->_mixCount = 4;
    MixManager::getInstance()->_mixSpeed = 100;
    MixManager::getInstance()->_mixRotateSpeed = 0;
    if (m_nSauceIndex<5) {
        MixManager::getInstance()->_mixturePathEX = localPath("stir/jam/"+std::to_string(m_nSauceIndex+1)+"_");
    }else{
        MixManager::getInstance()->_mixturePathEX = localPath("stir/sacue/"+std::to_string(m_nSauceIndex-4)+"_");
    }
    MixManager::getInstance()->_mixturePos = Vec2(0, -30);
    MixManager::getInstance()->_mixtureNode = m_pBatter;
    MixManager::getInstance()->_mixtureSize = m_pBowl->getContentSize();
    MixManager::getInstance()->onMixFinish = CC_CALLBACK_0(DonutMakeSauceScene::_finishMix, this);
    MixManager::getInstance()->onMixCallback = [=](int step){
        if (step==1) {
            m_pBatter->removeAllChildren();
        }
        if (step==2) {
            AudioHelp::getInstance()->playEffect("vo_keep_stirring_smooth.mp3");
        }
    };
    MixManager::getInstance()->onScheduleCallback = [=](int index){
        m_pProgressStar->addStar();
    };
}

void DonutMakeSauceScene::_finishMix()
{
    m_pProgressStar->runAction(Sequence::create(DelayTime::create(0.5),
                                                EaseBackIn::create(MoveBy::create(1, Vec2(0, visibleSize.height))),
                                                CallFunc::create([=](){
        m_pProgressStar->removeFromParent();
    }), NULL));
    AudioHelp::getInstance()->stopLoopEffect();
    AudioHelp::getInstance()->playEffect("ding.mp3");
    AudioHelp::getInstance()->playEffect("vo_well_done.mp3");
    m_pMixTool->setTouchEnabled(false);
    m_pMixTool->runAction(JumpBy::create(1, Vec2(visibleSize.width, 0), 100, 1));
    m_pBowl->runAction(MoveBy::create(1.0,Vec2(-100,0)));
    m_pBowlFront->runAction(MoveBy::create(1.0,Vec2(-100,0)));
    _showDonut();
}
void DonutMakeSauceScene::_finish(){
    _saveDonut();
    AudioHelp::getInstance()->stopLoopEffect();
    GameDataManager::getInstance()->m_nFlavorIndex = m_nSauceIndex;
    
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<DecorateScene>();
    }), NULL));
}