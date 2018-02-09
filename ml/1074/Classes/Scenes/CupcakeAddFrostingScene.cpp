
#include "CupcakeAddFrostingScene.h"
#include "SceneManager.h"

CupcakeAddFrostingScene::CupcakeAddFrostingScene()
{
    m_nMoveCount = 0;
    m_nFrostingIndex = -1;
    m_nAddCount = 0;
    m_pFrostingBag = nullptr;
}

CupcakeAddFrostingScene::~CupcakeAddFrostingScene()
{
    
}
bool CupcakeAddFrostingScene::init()
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
    
    _showTray();
    m_pGameUI->showNormalLayout();
    return true;
}

void CupcakeAddFrostingScene::onEnter()
{
    ExtensionScene::onEnter();
}

void CupcakeAddFrostingScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void CupcakeAddFrostingScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if (name == "frosting_bag"){
        if(pDragNode->index == 0){
            pDragNode->index = 1;
            Sprite* pColor = Sprite::create(localRootPath("dec/cream/"+std::to_string(m_nFrostingIndex+1)+".png"));
            pColor->setPosition(pColor->getContentSize()*0.5);
            
            m_pFrostingScribble = ScribbleNode::create(pColor->getContentSize());
//            this->addChildToContentLayer(m_pFrostingScribble);
//            m_pFrostingScribble->setPosition(m_pCurCupcake->getPosition()+Vec2(0, 130));
            m_pFrostingScribble->useBrush("content/brush.png",Scribble::BrushType::eBrush);
//            m_pFrostingScribble->setLocalZOrder(5);
            m_pDecNode->addChild(m_pFrostingScribble);
            m_pFrostingScribble->antiAliasing();
            
            m_pFrostingScribble->useTarget(pColor);
            m_pScrollView->setTouchEnabled(false);
            m_bItemEnable = false;
            m_pScrollView->runAction(Sequence::create(MoveBy::create(1, Vec2(visibleSize.width-m_pScrollView->getInnerContainer()->getPositionX(), 0)),
                                                      CallFunc::create([=](){
                m_pScrollView->removeFromParent();
                m_pScrollView = nullptr;
            }), NULL));
            AudioHelp::getInstance()->playEffect("drag_corn.mp3");
        }else{
            Rect rect = m_pCurCupcake->getContentSprite()->getBoundingBox();
            rect.origin = m_pCurCupcake->convertToWorldSpace(rect.origin);
            rect.origin = rect.origin+Vec2(0, 160);
            if (rect.containsPoint(worldPoint)) {
                m_pFrostingScribble->paint(worldPoint);
                Director::getInstance()->getRenderer()->render();
                m_nMoveCount = 0;
            }
        }
        
    }
}

void CupcakeAddFrostingScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if (name == "frosting_bag") {
        if(pDragNode->index == 1){
            Rect rect = m_pCurCupcake->getContentSprite()->getBoundingBox();
            rect.origin = m_pCurCupcake->convertToWorldSpace(rect.origin);
            rect.origin = rect.origin+Vec2(0, 160);
            if (rect.containsPoint(worldPoint)) {
                AudioHelp::getInstance()->playLoopEffect("pouring_sauce.mp3");
                m_pFrostingScribble->paint(worldPoint+pDragNode->getOffset(),worldPoint);
                Director::getInstance()->getRenderer()->render();
            }
            m_nMoveCount++;
            if (m_nMoveCount%5==0) {
//                Sprite* pDot = Sprite::create(localRootPath("dec/cream/"+std::to_string(m_nFrostingIndex+1)+".png"));
//                this->addChildToUILayer(pDot);
//                pDot->setScale(0.2);
//                pDot->setPosition(worldPoint);
//                pDot->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5, 0.3),
//                                                               FadeOut::create(0.5), NULL),
//                                                 CallFuncN::create([=](Node* pNode){
//                    pNode->removeFromParent();
//                }), NULL));
            }
        }
    }
}

void CupcakeAddFrostingScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
    AudioHelp::getInstance()->stopLoopEffect();
    std::string name = pDragNode->getName();
    if (name == "frosting_bag"){
        if(pDragNode->index == 1){
            int percent = m_pFrostingScribble->getCanvas()->getPercentageTransparent();
            log("====percent===%d",percent);
            if (percent <= 40) {
                m_pGuideLayer->removeGuide();
                m_pFrostingScribble->resetCanvas();
                m_pFrostingScribble->paint(localRootPath("dec/cream/"+std::to_string(m_nFrostingIndex+1)+".png"));
                pDragNode->setTouchEnabled(false);
                pDragNode->runAction(MoveTo::create(1, Vec2(visibleSize.width+300, pDragNode->getPositionY())));
                _showCandies();
            }
        }
    }else if (name == "candy"){
        m_pGuideLayer->removeGuide();
        Rect rect = m_pCurCupcake->getContentSprite()->getBoundingBox();
        rect.origin = m_pCurCupcake->convertToWorldSpace(rect.origin);
        rect.origin = rect.origin+Vec2(0, 160);
        if (rect.containsPoint(worldPoint)) {
            pDragNode->setTouchEnabled(false);
            pDragNode->setVisible(false);
            int tag = pDragNode->getTag();
            std::string path = "";
            Vec2 pos = Vec2(-75, 50);
            if (tag <2) {
                pos = Vec2(0, 68);
                path = "marshmallow"+std::to_string(tag+1)+".png";
                MenuLayer::addIngredient(MenuLayer::ePackageCupcake, MenuLayer::eCupcake_marshmallow);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
            }else{
                pos = Vec2(0, 78);
                path = "topping_rainbow.png";
                MenuLayer::addIngredient(MenuLayer::ePackageCupcake, MenuLayer::eCupcake_rainbow);
                MenuLayer::showAddIngredientAnimation(m_pGameUI->getButton(GameUILayoutLayer::eUIButtonTagMenu));
            }
            AudioHelp::getInstance()->playEffect("drag_corn.mp3");
            AudioHelp::getInstance()->_playEffectLaugh();
            Sprite* pAdd = Sprite::create(localRootPath("dec/"+path));
            pAdd->setPosition(pos);
            m_pDecNode->addChild(pAdd);
            pAdd->setLocalZOrder(10-tag);
            
            if (!m_pCottonCandy0->isVisible() && !m_pCottonCandy1->isVisible()) {
                m_pCottonCandyPlate->setLocalZOrder(10);
                m_pCottonCandyPlate->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, 0)));
            }
            if (!m_pRainbowCandy->isVisible()) {
                m_pRainbowCandyPlate->setLocalZOrder(10);
                m_pRainbowCandyPlate->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, 0)));
            }
            m_nAddCount++;
            if (m_nAddCount==3) {
                _finish();
            }
        }else{
            pDragNode->setTouchEnabled(false);
            pDragNode->back(1,[=](){                
                pDragNode->setTouchEnabled(true);
            });
        }
    }
}

void CupcakeAddFrostingScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void CupcakeAddFrostingScene::_initData()
{
    setExPath("content/make/cupcake/make2/");
}

void CupcakeAddFrostingScene::_showTray()
{
    m_pTray = Sprite::create(localPath("plate.png"));
    this->addChildToContentLayer(m_pTray);
    CMVisibleRect::setPositionAdapted(m_pTray, 330, 350-visibleSize.height);
    m_pTray->setLocalZOrder(1);
    
    Sprite* pShadow = Sprite::create(localPath("plate_shadow.png"));;
    this->addChildToContentLayer(pShadow);
    CMVisibleRect::setPositionAdapted(pShadow, 330, 350);
    pShadow->setVisible(false);
    
    m_pTray->runAction(Sequence::create(MoveBy::create(1, Vec2(0, visibleSize.height)),
                                        DelayTime::create(0.5),
                                        CallFunc::create([=](){
        pShadow->setVisible(true);
        _showChooseCupcakes();
        m_pTray->removeAllChildren();
    }),
                                        JumpBy::create(1, Vec2::ZERO, 50, 2),
                                        CallFunc::create([=](){
        pShadow->setVisible(false);
    }),
                                        MoveBy::create(1, Vec2(0, -visibleSize.height)), NULL));
    
    int cupIndex = GameDataManager::getInstance()->m_nShapeIndex;
    std::vector<std::string> flavorVector = StringHelp::split(GameDataManager::getInstance()->m_sFlavorStr, ",");
    int count = 2;
    for (int i = 0; i<4; i++) {
        Sprite* pCup = Sprite::create(localPath("cup/"+std::to_string(cupIndex+1)+".png"));
        m_pTray->addChild(pCup);
        if (i/2>0) {
            pCup->setPosition(Vec2(222+224*(i%count), 120+100*(i/count)));
            pCup->setScale(0.83);
        }else{
            pCup->setPosition(Vec2(120+222*(i%count), 120+100*(i/count)));
        }
        pCup->setTag(i);
        
        ClippingNode* pClipping = ClippingNode::create(Sprite::create(localPath("batter/batter_mask.png")));
        pClipping->setAlphaThreshold(0.5);
        m_pTray->addChild(pClipping,5);
        if (i/2>0) {
            pClipping->setPosition(Vec2(222+224*(i%count), 120+100*(i/count)));
            pClipping->setScale(0.83);
        }else{
            pClipping->setPosition(Vec2(120+222*(i%count), 120+100*(i/count)));
        }
        for (int k = 0; k<3; k++) {
            std::string flavorStr = flavorVector.at(i*3+k);
            int flavorIndex = atoi(flavorStr.c_str());
            Sprite* pBatter = Sprite::create(localRootPath("dec/cake/"+std::to_string(flavorIndex+1)+".png"));
            pClipping->addChild(pBatter);
            pBatter->setPosition(Vec2(32, -34-40+20*k));
            pBatter->setOpacity(0);
            pBatter->runAction(FadeTo::create(2, 255));
        }
    }
}
void CupcakeAddFrostingScene::_showChooseCupcakes()
{
    int cupIndex = GameDataManager::getInstance()->m_nShapeIndex;
    std::vector<std::string> flavorVector = StringHelp::split(GameDataManager::getInstance()->m_sFlavorStr, ",");
    int count = 2;
    for (int i = 0; i<4; i++) {
        LockItem* pItem = LockItem::create();
        pItem->initWithName(localRootPath("dec/cup_down/"+std::to_string(cupIndex+1)+".png"), "");
        this->addChildToContentLayer(pItem);
//        pItem->setPosition(Vec2(125+186*(i%3), 120+100*(i/3))+m_pTray->getBoundingBox().origin);
        if (i/2>0) {
            pItem->setPosition(Vec2(222+224*(i%count), 120+100*(i/count))+m_pTray->getBoundingBox().origin);
        }else{
            pItem->setPosition(Vec2(120+222*(i%count), 120+100*(i/count))+m_pTray->getBoundingBox().origin);
        }
        pItem->setTag(i);
        pItem->setLocalZOrder(2);
        pItem->setScale(0.4);
        
        Sprite* pCup = Sprite::create(localRootPath("dec/cup_on/"+std::to_string(cupIndex+1)+".png"));
        pItem->addChild(pCup,10);
        pCup->setOpacity(200);
        
        Node* pFood = Node::create();
        pItem->addChild(pFood);
        pFood->setName("food");
        for (int k = 2; k>=0; k--) {
            std::string flavorStr = flavorVector.at(i*3+k);
            int flavorIndex = atoi(flavorStr.c_str());
            _addBatter(pFood,flavorIndex,k);
        }
        pItem->onItemClicked = CC_CALLBACK_1(CupcakeAddFrostingScene::_onCupcakeSelectCallback, this);
        pItem->setItemTouchEnable(false);
        m_cakeVector.push_back(pItem);
        pItem->runAction(Sequence::create(JumpTo::create(1, CMVisibleRect::getPositionAdapted(Vec2(200+240*(i%2), 400+150*(i/2))), 150, 1),
                                          CallFunc::create([=](){
            pItem->setItemTouchEnable();
        }), NULL));
    }
}
void CupcakeAddFrostingScene::_addBatter(Node* pParent,int flavorIndex,int maskIndex)
{
    Sprite* pStencil = Sprite::create(localRootPath("dec/mask"+std::to_string(maskIndex+1)+".png"));
    Sprite* pSprite = Sprite::create(localRootPath("dec/cake/"+std::to_string(flavorIndex+1)+".png"));
    
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
    
    pParent->addChild(pRT);
}
void CupcakeAddFrostingScene::_showCupcake()
{
    
}

void CupcakeAddFrostingScene::_showFrostingScrollview()
{
    m_pScrollView = ExpandScrollView::create();
    m_pScrollView->setContentSize(Size(visibleSize.width,200));
    this->addChildToContentLayer(m_pScrollView);
    CMVisibleRect::setPositionAdapted(m_pScrollView, visibleSize.width, 630,kBorderLeft);
    m_pScrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    m_pScrollView->setLocalZOrder(5);
    m_pScrollView->setScrollBarEnabled(false);
    m_pScrollView->setClippingEnabled(false);
    
    int typeCount = 10;
    m_pScrollView->getInnerContainer()->setContentSize(Size((typeCount+1)*200, 200));
    for (int i = 0; i<typeCount; i++) {
        LockItem* pItem = LockItem::create();
        
        bool bLock = false;
        if (!gNoneIap && !IAPManager::getInstance()->getItemIsBought(2) && i%2==1){
            if (!ConfigManager::getInstance()->getVideoUnLocked("cupcake_frosting", i)) {
                LockUIManager::getInstance()->registerLock("cupcake_frosting", i, pItem);
                LockUIManager::getInstance()->setLockOriginPos(Vec2(40, -60));
                pItem->lockType = 1;
                bLock = true;
            }
        }
        pItem->initWithName(localRootPath("dec/cream_bag/bag.png"), "content/common/video.png",bLock);
        pItem->setScale(0.5);
        pItem->setRotation(30);
        Sprite* pLock = pItem->getLockSprite();
        if (pLock) {
            pLock->setScale(2);
            pLock->setRotation(-30);
        }
        
        KettleNode* pContainer = KettleNode::create(pItem);
        pContainer->addFront(localRootPath("dec/cream_bag/bag.png"));
        pContainer->addShadow(localRootPath("dec/cream_bag/shadow.png"));
        pContainer->addWater(localRootPath("dec/cream_bag/"+std::to_string(i+1)+".png"), localRootPath("dec/cream_bag/1.png"));
        pItem->setUserData(pContainer);
        
        Sprite* Lock = pItem->getLockSprite();
        if (Lock) {
            Lock->setTag(TAG_REWARD_LOCK);
        }
        pItem->setTag(i);
        m_pScrollView->getInnerContainer()->addChild(pItem);
        pItem->setPosition(Vec2(100+i*200,100));
        //        pItem->setScale(0.5);
        
        pItem->onItemClicked = CC_CALLBACK_1(CupcakeAddFrostingScene::_onFrostingClicked, this);
        m_typeItems.pushBack(pItem);
    }
    m_pScrollView->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                              CallFunc::create([=](){
        m_pGuideLayer->removeGuide();
        m_pGuideLayer->showGuideTap(m_pScrollView->getPosition()+Vec2(450, 100));
        m_bItemEnable = true;
        AudioHelp::getInstance()->playEffect("vo_so_many_flavors.mp3");
    }), NULL));
    
    AudioHelp::getInstance()->playEffect("present_in.mp3");
}
void CupcakeAddFrostingScene::_showFrostingBag(int index)
{
    
    m_pGuideLayer->removeGuide();
    //        show sauce
    if(m_nFrostingIndex>=0){
        
    }else{
        
    }
    LockItem* pItem = m_typeItems.at(index);
    m_pScrollView->setTouchEnabled(false);
    
    pItem->setVisible(false);
    if(m_pFrostingBag){
        DragNode* pCurbag = m_pFrostingBag;
        LockItem* pPreFlavor = m_typeItems.at(m_nFrostingIndex);
        pCurbag->setTouchEnabled(false);
        pCurbag->runAction(Sequence::create(MoveTo::create(1, pPreFlavor->getParent()->convertToWorldSpace(pPreFlavor->getPosition())),
                                                   CallFunc::create([=](){
            pPreFlavor->setVisible(true);
            pCurbag->removeFromParent();
        }), NULL));
        
    }
    m_pFrostingBag = _createDrageNode(localRootPath("dec/cream_bag/bag.png"));
    m_pFrostingBag->setPosition(pItem->getParent()->convertToWorldSpace(pItem->getPosition()));
    
    KettleNode* pContainer = KettleNode::create(m_pFrostingBag);
    pContainer->addFront(localRootPath("dec/cream_bag/bag.png"));
    pContainer->addWater(localRootPath("dec/cream_bag/"+std::to_string(index+1)+".png"), localRootPath("dec/cream_bag/1.png"));
    m_pFrostingBag->setUserData(pContainer);
    
    this->addChildToContentLayer(m_pFrostingBag);
    m_pFrostingBag->setName("frosting_bag");
    
    m_pFrostingBag->setUserData(pContainer);
    m_pFrostingBag->setTouchEnabled(false);
    m_pFrostingBag->runAction(Sequence::create(MoveTo::create(1, m_pCurCupcake->getPosition()+Vec2(100, 110)),
                                               CallFunc::create([=](){
        m_pFrostingBag->setTouchEnabled(true);
        m_pGuideLayer->removeGuide();
        m_pGuideLayer->showGuide("content/common/tip/cream_line.png", m_pDecNode->getPosition());
        //            m_pGuideLayer->showGuideMove(m_pFrostingBag->getPosition(),m_pCurCupcake->getPosition());
        m_pScrollView->setTouchEnabled(true);
        m_bItemEnable = true;
    }), NULL));
    
    m_pFrostingBag->setLocalZOrder(5);
    m_pFrostingBag->setActionPoint(Vec2(5, 10));
    
    m_nFrostingIndex = index;
    m_pFrostingBag->setTag(index);
//    if(!m_pFrostingBag){
//        LockItem* pPreFlavor = m_typeItems.at(m_nFrostingIndex);
//        m_pFrostingBag->runAction(Sequence::create(MoveTo::create(1, pPreFlavor->getParent()->convertToWorldSpace(pPreFlavor->getPosition())),
//                                                   CallFunc::create([=](){
//            pPreFlavor->setVisible(true);
//            pItem->setVisible(false);
//            KettleNode* pContainer = (KettleNode*)m_pFrostingBag->getUserData();
//            if (pContainer) {
//                pContainer->addWater(localRootPath("dec/cream_bag/"+std::to_string(index+1)+".png"), localRootPath("dec/cream_bag/1.png"));
//            }
//            pItem->setVisible(false);
//            m_pFrostingBag->setPosition(pItem->getParent()->convertToWorldSpace(pItem->getPosition()));
//        }),
//                                                   MoveTo::create(1, m_pCurCupcake->getPosition()+Vec2(100, 150)),
//                                                   CallFunc::create([=](){
//            m_pFrostingBag->setTouchEnabled(true);
//            m_pGuideLayer->removeGuide();
//            m_pGuideLayer->showGuide("content/common/tip/cream_line.png", m_pDecNode->getPosition());
//            //            m_pGuideLayer->showGuideMove(m_pFrostingBag->getPosition(),m_pCurCupcake->getPosition());
//            m_pScrollView->setTouchEnabled(true);
//            m_bItemEnable = true;
//        }), NULL));
//    }else{
//        LockItem* pPreFlavor = m_typeItems.at(m_nFrostingIndex);
//        m_pFrostingBag->runAction(Sequence::create(MoveTo::create(1, pPreFlavor->getParent()->convertToWorldSpace(pPreFlavor->getPosition())),
//                                                   CallFunc::create([=](){
//            pPreFlavor->setVisible(true);
//            pItem->setVisible(false);
//            KettleNode* pContainer = (KettleNode*)m_pFrostingBag->getUserData();
//            if (pContainer) {
//                pContainer->addWater(localRootPath("dec/cream_bag/"+std::to_string(index+1)+".png"), localRootPath("dec/cream_bag/1.png"));
//            }
//            pItem->setVisible(false);
//            m_pFrostingBag->setPosition(pItem->getParent()->convertToWorldSpace(pItem->getPosition()));
//        }),
//                                                   MoveTo::create(1, m_pCurCupcake->getPosition()+Vec2(100, 150)),
//                                                   CallFunc::create([=](){
//            m_pFrostingBag->setTouchEnabled(true);
//            m_pGuideLayer->removeGuide();
//            m_pGuideLayer->showGuide("content/common/tip/cream_line.png", m_pDecNode->getPosition());
////            m_pGuideLayer->showGuideMove(m_pFrostingBag->getPosition(),m_pCurCupcake->getPosition());
//            m_pScrollView->setTouchEnabled(true);
//            m_bItemEnable = true;
//        }), NULL));
//    }
}

void CupcakeAddFrostingScene::_showCandies()
{
    m_pCottonCandyPlate = Sprite::create(localRootPath("dec/plate1.png"));
    this->addChildToContentLayer(m_pCottonCandyPlate);
    CMVisibleRect::setPositionAdapted(m_pCottonCandyPlate, 190+visibleSize.width, 710);
    
    m_pRainbowCandyPlate = Sprite::create(localRootPath("dec/plate2.png"));
    this->addChildToContentLayer(m_pRainbowCandyPlate);
    CMVisibleRect::setPositionAdapted(m_pRainbowCandyPlate, 480+visibleSize.width, 710);
    
    m_pCottonCandy0 = _createDrageNode(localRootPath("dec/plate_marshmallow1.png"));
    this->addChildToContentLayer(m_pCottonCandy0);
    CMVisibleRect::setPositionAdapted(m_pCottonCandy0, 190+visibleSize.width, 720);
    m_pCottonCandy0->setTouchEnabled(false);
    
    m_pCottonCandy1 = _createDrageNode(localRootPath("dec/plate_marshmallow2.png"));
    this->addChildToContentLayer(m_pCottonCandy1);
    CMVisibleRect::setPositionAdapted(m_pCottonCandy1, 190+visibleSize.width, 720);
    m_pCottonCandy1->setTouchEnabled(false);
    
    m_pRainbowCandy = _createDrageNode(localRootPath("dec/rainbow.png"));
    this->addChildToContentLayer(m_pRainbowCandy);
    CMVisibleRect::setPositionAdapted(m_pRainbowCandy, 480+visibleSize.width, 720);
    m_pRainbowCandy->setTouchEnabled(false);
    
    auto action = Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                   CallFuncN::create([=](Node* pNode){
        DragNode* pIngredient = dynamic_cast<DragNode*>(pNode);
        if (pIngredient) {
            pIngredient->setOrgPositionDefault();
            pIngredient->setTouchEnabled(true);
        }
    }), NULL);
    m_pCottonCandyPlate->runAction(action);
    m_pRainbowCandyPlate->runAction(action->clone());
    m_pCottonCandy0->runAction(action->clone());
    m_pCottonCandy1->runAction(action->clone());
    m_pRainbowCandy->runAction(action->clone());
    m_pCottonCandy0->setName("candy");
    m_pCottonCandy1->setName("candy");
    m_pRainbowCandy->setName("candy");
    m_pCottonCandy0->setTag(0);
    m_pCottonCandy1->setTag(1);
    m_pRainbowCandy->setTag(2);
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        m_pGuideLayer->showGuideMove(m_pCottonCandyPlate->getPosition(), m_pDecNode->getPosition());
        AudioHelp::getInstance()->playEffect("vo_make_rainbow_decorations.mp3");
    }), NULL));
    AudioHelp::getInstance()->playEffect("present_in.mp3");
}

void CupcakeAddFrostingScene::_onCupcakeSelectCallback(LockItem* pItem)
{
    for (auto pCake:m_cakeVector) {
        pCake->setItemTouchEnable(false);
        if (pCake!=pItem) {
            Vec2 pos = pCake->getPosition();
            if (pos.x<visibleSize.width/2) {
                pCake->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
            }else{
                pCake->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
            }
        }
    }
    m_pCurCupcake = pItem;
    pItem->runAction(Sequence::create(ScaleTo::create(0.13, 1.0*0.4, 0.90*0.4),
                                      ScaleTo::create(0.11, 0.92*0.4, 1.0*0.4),
                                      ScaleTo::create(0.10, 1.0*0.4, 0.96*0.4),
                                      ScaleTo::create(0.09, 0.98*0.4, 1.0*0.4),
                                      ScaleTo::create(0.08, 1.0*0.4, 0.99*0.4),
                                      ScaleTo::create(0.07, 0.99*0.4, 1.0*0.4),
                                      Spawn::create(ScaleTo::create(1, 1),
                                                    MoveTo::create(1, CMVisibleRect::getPositionAdapted(Vec2(320, 380))), NULL),
                                      CallFunc::create([=](){
        Sprite* pShadow = Sprite::create(localRootPath("dec/cup_shadow.png"));
        this->addChildToContentLayer(pShadow);
        pShadow->setLocalZOrder(-1);
        pShadow->setPosition(pItem->getPosition()+Vec2(0, -115));
        m_pDecNode = Node::create();
        this->addChildToContentLayer(m_pDecNode);
        m_pDecNode->setLocalZOrder(5);
        m_pDecNode->setPosition(m_pCurCupcake->getPosition()+Vec2(0, 130));
        _showFrostingScrollview();
    }), NULL));
    AudioHelp::getInstance()->playSelectedEffect();
}

void CupcakeAddFrostingScene::_onFrostingClicked(LockItem* item)
{
    if (!m_bItemEnable) {
        return;
    }
    if (m_pScrollView->isScrolled()) {
        return;
    }
    int tag = item->getTag();
    if (item->isLocked()){
        if(!ConfigManager::getInstance()->getVideoUnLocked("cupcake_frosting", tag)){
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
                RewardManager::getInstance()->showRewardAds("cupcake_frosting", tag);
                pLayer->removeFromParent();
            };
            return;
        }
    }
    m_bItemEnable = false;
    AudioHelp::getInstance()->playSelectedEffect();
    item->runAction(Sequence::create(ScaleTo::create(0.13, 1.0*0.5, 0.90*0.5),
                                     ScaleTo::create(0.11, 0.92*0.5, 1.0*0.5),
                                     ScaleTo::create(0.10, 1.0*0.5, 0.96*0.5),
                                     ScaleTo::create(0.09, 0.98*0.5, 1.0*0.5),
                                     ScaleTo::create(0.08, 1.0*0.5, 0.99*0.5),
                                     ScaleTo::create(0.07, 0.99*0.5, 1.0*0.5), NULL));
    _showFrostingBag(tag);
    AudioHelp::getInstance()->_playEffectGoodChooice();
}

void CupcakeAddFrostingScene::_saveCupcake()
{
//    Size cakeSize = Size(380, 400);
//    Size cakeSize = Sprite::create(localRootPath("dec/mask3.png"))->getContentSize()+Size(0,200);
    Size cakeSize = Sprite::create(localRootPath("dec/mask3.png"))->getContentSize();
    RenderTexture* render = RenderTexture::create(cakeSize.width, cakeSize.height,Texture2D::PixelFormat::RGBA8888,
                                                  GL_DEPTH24_STENCIL8_OES);//针对clippingNode 设置了深度
    Vec2 posDec = m_pDecNode->getPosition();
    Node* pFood = m_pCurCupcake->getChildByName("food");
    pFood->setPosition(Vec2(cakeSize.width/2, 162));
//    m_pDecNode->setPosition(Vec2(cakeSize.width/2, 162+130));
    render->beginWithClear(1.0f, 1.0f, 1.0f, 0.0f);
    if (pFood) {
        pFood->visit();
    }
//    m_pDecNode->visit();
    render->end();
    
    Director::getInstance()->getRenderer()->render();
//    m_pDecNode->setPosition(posDec);
    if (pFood) {
        pFood->setPosition(Vec2::ZERO);
    }
    
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_BACKGROUND);
    __NotificationCenter::getInstance()->removeObserver(render, EVENT_COME_TO_FOREGROUND);
    
    Image* pImage = render->newImage();
    
    bool issuccess;
    
    issuccess = pImage->saveToFile(FileUtils::getInstance()->getWritablePath()+"cupcake.png", false);
    pImage->autorelease();
    
}
void CupcakeAddFrostingScene::_finish()
{
    _saveCupcake();
    GameDataManager::getInstance()->m_nFlavorIndex = m_nFrostingIndex;
    AudioHelp::getInstance()->_playEffectNiceWork();
    AudioHelp::getInstance()->playEffect("done.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<DecorateScene>();
    }), NULL));
}