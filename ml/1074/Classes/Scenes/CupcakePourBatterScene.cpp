
#include "CupcakePourBatterScene.h"
#include "SceneManager.h"

CupcakePourBatterScene::CupcakePourBatterScene()
{
    m_bItemEnable = false;
    m_nCupIndex = 0;
    m_nStep = 0;
}

CupcakePourBatterScene::~CupcakePourBatterScene()
{
    
}
bool CupcakePourBatterScene::init()
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
    
    std::memset(m_CupFlavor, -1, sizeof(m_CupFlavor));
    m_pGameUI->showNormalLayout();
    return true;
}

void CupcakePourBatterScene::onEnter()
{
    ExtensionScene::onEnter();
}
void CupcakePourBatterScene::onButtonCallback(Button* btn)
{
    int tag = btn->getTag();
    if (tag == GameUILayoutLayer::eUIButtonTagNext) {
        btn->setTouchEnabled(false);
        btn->setVisible(false);
        m_bItemEnable = false;
        m_nStep++;
        m_pScrollView->runAction(Sequence::create(MoveBy::create(0.5, Vec2(visibleSize.width, 0)),
                                                  CallFuncN::create([=](Node* pNode){
            pNode->removeFromParent();
        }), NULL));
        if (m_nStep==1) {
            GameDataManager::getInstance()->m_nShapeIndex = m_nCupIndex;
            _showBags();
        }
        return;
    }
    ExtensionScene::onButtonCallback(btn);
}

void CupcakePourBatterScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void CupcakePourBatterScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
    std::string name = pDragNode->getName();
    if ("bag"==name){
        if (pDragNode->index==0){
            KettleNode* pContainer = (KettleNode*)pDragNode->getUserData();
            if (pContainer) {
                pContainer->showShadow(false);
            }
            pDragNode->index = 1;
            pDragNode->setScale(0.67);
            pDragNode->runAction(RotateBy::create(0.5, -30));
        }
    }
}

void CupcakePourBatterScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    std::string name = pDragNode->getName();
    if ("bag"==name){
        if (pDragNode->index==1) {
            bool bAdd = false;
            for (auto pCup:m_cupItems) {
                Rect rect = pCup->getBoundingBox();
                rect.origin = pCup->getParent()->convertToWorldSpace(rect.origin);
                if (rect.containsPoint(worldPoint)) {
                    pDragNode->setTouchEnabled(false);
                    m_pCurBag = pDragNode;
                    _addBatter(pCup->getTag(),pDragNode->getTag());
                    bAdd = true;
                    break;
                }
            }
//            if (!bAdd) {
//                pDragNode->setTouchEnabled(false);
//                pDragNode->back(0,[=](){
//                    KettleNode* pContainer = (KettleNode*)pDragNode->getUserData();
//                    if (pContainer) {
//                        pContainer->showShadow(true);
//                    }
//                    pDragNode->index = 0;
//                    pDragNode->setTouchEnabled(true);
//                });
//                pDragNode->runAction(ScaleTo::create(0.5, 0.5));
//                pDragNode->runAction(RotateTo::create(0.5, 30));
//            }
        }
    }
}

void CupcakePourBatterScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
    std::string name = pDragNode->getName();
    if ("bag"==name) {
//        if (pDragNode->index==1) {
//            bool bAdd = false;
//            for (auto pCup:m_cupItems) {
//                Rect rect = pCup->getBoundingBox();
//                rect.origin = pCup->getParent()->convertToWorldSpace(rect.origin);
//                if (rect.containsPoint(worldPoint)) {
//                    pDragNode->setTouchEnabled(false);
//                    m_pCurBag = pDragNode;
//                    _addBatter(pCup->getTag(),pDragNode->getTag());
//                    bAdd = true;
//                    break;
//                }
//            }
//            if (!bAdd) {
                pDragNode->setTouchEnabled(false);
                pDragNode->back(0,[=](){
                    KettleNode* pContainer = (KettleNode*)pDragNode->getUserData();
                    if (pContainer) {
                        pContainer->showShadow(true);
                    }
                    pDragNode->index = 0;
                    pDragNode->setTouchEnabled(true);
                });
                pDragNode->runAction(ScaleTo::create(0.5, 0.5));
                pDragNode->runAction(RotateTo::create(0.5, 30));
//            }
//        }
    }
    m_pGuideLayer->removeGuide();
}

void CupcakePourBatterScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void CupcakePourBatterScene::_initData()
{
    setExPath("content/make/cupcake/make2/");
}

void CupcakePourBatterScene::_showTray()
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
                                        CallFunc::create([=](){
        pShadow->setVisible(true);
        _showCupScrollView();
    }), NULL));
    
    int count = 2;
    for (int i = 0; i<6; i++) {
        ClippingNode* pClipping = ClippingNode::create(Sprite::create(localPath("batter/batter_mask.png")));
        pClipping->setAlphaThreshold(0.5);
        m_pTray->addChild(pClipping,5);
//        if (i/3>0) {
//            pClipping->setPosition(Vec2(143+170*(i%3), 120+100*(i/3)));
//            pClipping->setScale(0.83);
//        }else{
//            pClipping->setPosition(Vec2(125+186*(i%3), 120+100*(i/3)));
//        }
        if (i/2>0) {
            pClipping->setPosition(Vec2(222+224*(i%count), 120+100*(i/count)));
            //                pCup->setPosition(Vec2(143+170*(i%3), 120+100*(i/3)));
            pClipping->setScale(0.83);
        }else{
            pClipping->setPosition(Vec2(120+222*(i%count), 120+100*(i/count)));
            //                pCup->setPosition(Vec2(143+170*(i%3), 120+100*(i/3)));
        }
        m_batterItems.pushBack(pClipping);
    }
}

void CupcakePourBatterScene::_showCupScrollView()
{
    m_pScrollView = ExpandScrollView::create();
    m_pScrollView->setContentSize(Size(visibleSize.width,200));
    this->addChildToUILayer(m_pScrollView);
    CMVisibleRect::setPositionAdapted(m_pScrollView, visibleSize.width, 650,kBorderLeft,kBorderBottom);
    m_pScrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    m_pScrollView->setLocalZOrder(5);
    m_pScrollView->setScrollBarEnabled(false);
    m_pScrollView->setClippingEnabled(false);
    
    int typeCount = 12;
    m_pScrollView->getInnerContainer()->setContentSize(Size((typeCount+1)*170, 200));
    
    for (int i = 0; i<typeCount; i++) {
        LockItem* pItem = LockItem::create();
        
        bool bLock = false;
        if (!gNoneIap && !IAPManager::getInstance()->getItemIsBought(2) && i%2==1){
            if (!ConfigManager::getInstance()->getVideoUnLocked("cupcake_cup", i)) {
                LockUIManager::getInstance()->registerLock("cupcake_cup", i, pItem);
                LockUIManager::getInstance()->setLockOriginPos(Vec2(40, -60));
                pItem->lockType = 1;
                bLock = true;
            }
        }
        pItem->initWithName(localPath("cup_icon/"+std::to_string(i+1)+".png"), "content/common/video.png",bLock);
        Sprite* Lock = pItem->getLockSprite();
        if (Lock) {
            Lock->setTag(TAG_REWARD_LOCK);
        }
        
        pItem->setTag(i);
        m_pScrollView->getInnerContainer()->addChild(pItem);
        pItem->setPosition(Vec2(100+i*170,90));
//        pItem->setScale(0.6);
        
        pItem->onItemClicked = CC_CALLBACK_1(CupcakePourBatterScene::_onCupClicked, this);
        m_typeItems.pushBack(pItem);
    }
    AudioHelp::getInstance()->playEffect("slide_sight.mp3");
    m_pScrollView->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                              CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_choose_fancy_cups.mp3");
        m_bItemEnable = true;
    }), NULL));
}


void CupcakePourBatterScene::_showBags()
{
    int typeCount = 3;
    
    std::vector<std::string> flavorVector = StringHelp::split(GameDataManager::getInstance()->m_sFlavorStr, ",");
    for (int i = 0; i<typeCount; i++) {
        std::string flavorStr = flavorVector.at(i);
        int flavorIndex = atoi(flavorStr.c_str());
        DragNode* pBag = _createDrageNode(localPath("bag/bag.png"));
        this->addChildToContentLayer(pBag);
        CMVisibleRect::setPositionAdapted(pBag, 140+190*i+visibleSize.width, 730);
        pBag->setName("bag");
        pBag->setActionPoint(Vec2(7, 11));
        pBag->setScale(0.5);
        pBag->setRotation(30);
        pBag->setTouchEnabled(false);
        pBag->runAction(Sequence::create(MoveBy::create(1,Vec2(-visibleSize.width, 0)),
                                         CallFunc::create([=](){
            pBag->setOrgPositionDefault();
            pBag->setTouchEnabled(true);
        }), NULL));
        KettleNode* pContainer = KettleNode::create(pBag);
        pContainer->addFront(localPath("bag/bag.png"));
        pContainer->addShadow(localPath("bag/shadow.png"));
        pContainer->addWater(localPath("bag/"+std::to_string(flavorIndex+1)+".png"), localPath("bag/1.png"));
        pBag->setUserData(pContainer);
        
        pBag->setTag(flavorIndex);
        m_bagItems.pushBack(pBag);
    }
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        m_pGuideLayer->showGuideMove(Vec2(visibleSize.width/2, visibleSize.height*0.7), m_pTray->getPosition());
        AudioHelp::getInstance()->playEffect("vo_add_colored_mixture_cup.mp3");
    }), NULL));
    AudioHelp::getInstance()->playEffect("slide_sight.mp3");
}
void CupcakePourBatterScene::_onCupClicked(LockItem* item)
{
    if (!m_bItemEnable) {
        return;
    }
    if (m_pScrollView->isScrolled()) {
        return;
    }
    int tag = item->getTag();
    if (!m_bItemEnable) {
        return;
    }
    if (item->isLocked()){
        if(!ConfigManager::getInstance()->getVideoUnLocked("cupcake_cup", tag)){
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
                RewardManager::getInstance()->showRewardAds("cupcake_cup", tag);
                pLayer->removeFromParent();
            };
            return;
        }
    }
    m_bItemEnable = false;
    AudioHelp::getInstance()->playEffect("slushy_cup.mp3");
//    AudioHelp::getInstance()->playSelectedEffect();
    
    m_pGameUI->showNextLayout();
    item->runAction(Sequence::create(ActionHelper::getJellyAction(),
                                     CallFunc::create([=](){
        m_bItemEnable = true;
    }), NULL));
    
    m_nCupIndex = tag;
    _updateCups(tag);
    AudioHelp::getInstance()->_playEffectLaugh();
}


void CupcakePourBatterScene::_updateCups(int index)
{
    if (m_cupItems.size()==0) {
        int count = 2;
        for (int i = 0; i<4; i++) {
            Sprite* pCup = Sprite::create(localPath("cup/"+std::to_string(index+1)+".png"));
            m_pTray->addChild(pCup);
            if (i/2>0) {
                pCup->setPosition(Vec2(222+224*(i%count), 120+100*(i/count)));
//                pCup->setPosition(Vec2(143+170*(i%3), 120+100*(i/3)));
                pCup->setScale(0.83);
            }else{
                pCup->setPosition(Vec2(120+222*(i%count), 120+100*(i/count)));
//                pCup->setPosition(Vec2(143+170*(i%3), 120+100*(i/3)));
            }
            pCup->setTag(i);
            m_cupItems.pushBack(pCup);
        }
    }else{
        for (auto pCup: m_cupItems) {
            pCup->setTexture(localPath("cup/"+std::to_string(index+1)+".png"));
        }
    }
}

void CupcakePourBatterScene::_addBatter(int index,int flavor)
{
    int i = 0;
    for (; i<3; i++) {
        int type = m_CupFlavor[index*3+i];
        if (type<0) {
            m_CupFlavor[index*3+i] = flavor;
            break;
        }
    }
    DragNode* pBag = m_pCurBag;
    pBag->setTouchEnabled(false);
    if(i>=3){
//        pBag->back(0,[=](){
//            KettleNode* pContainer = (KettleNode*)pBag->getUserData();
//            if (pContainer) {
//                pContainer->showShadow(true);
//            }
//            pBag->index = 0;
            pBag->setTouchEnabled(true);
//        });
//        pBag->runAction(ScaleTo::create(0.5, 0.5));
//        pBag->runAction(RotateTo::create(0.5, 30));
    }else{
        Sprite* pCup = m_cupItems.at(index);
        Vec2 pos = pCup->getPosition();
        pos = pCup->getParent()->convertToWorldSpace(pos);
        pBag->setPosition(pos+Vec2(120, 180));
        ClippingNode* pClipping = m_batterItems.at(index);
        Sprite* pBatter = Sprite::create(localPath("batter/"+std::to_string(flavor+1)+".png"));
        pClipping->addChild(pBatter);
        pBatter->setOpacity(0);
        pBatter->setPosition(Vec2(0, -40+20*i));
        AudioHelp::getInstance()->playEffect("pouring_sauce.mp3");
        pBatter->runAction(Sequence::create(FadeTo::create(1, 255),
                                            CallFunc::create([=](){
            pBag->back(0,[=](){
                KettleNode* pContainer = (KettleNode*)pBag->getUserData();
                if (pContainer) {
                    pContainer->showShadow(true);
                }
                pBag->index = 0;
                pBag->setTouchEnabled(true);
            });
            pBag->runAction(ScaleTo::create(0.5, 0.5));
            pBag->runAction(RotateTo::create(0.5, 30));
            if(i == 2){
                ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/bling.plist");
                this->addChildToUILayer(pParticle);
                pParticle->setPosition(pCup->getParent()->convertToWorldSpace(pCup->getPosition()));
                
            }
        }), NULL));
        int k = 0;
        for (; k<12; k++) {
            int type = m_CupFlavor[k];
            if (type<0) {
                break;
            }
        }
        if(k>=12){
            this->runAction(Sequence::create(DelayTime::create(1.5),
                                             CallFunc::create([=](){
                _finish();
            }), NULL));
        }
    }
}

void CupcakePourBatterScene::_finish()
{
    std::string str;
    for (int i = 0; i<12; i++) {
        if (i==0) {
            str = std::to_string(m_CupFlavor[i]);
        }else{
            str.append(",");
            str.append(std::to_string(m_CupFlavor[i]));
        }
    }
    GameDataManager::getInstance()->m_sFlavorStr = str;
    AudioHelp::getInstance()->playEffect("vo_talented.mp3");
    
    m_pGuideLayer->removeGuide();
    for(auto pBag:m_bagItems){
        pBag->setTouchEnabled(false);
        pBag->runAction(MoveTo::create(1, Vec2(visibleSize.width+300, pBag->getPositionY())));
    }
    AudioHelp::getInstance()->playEffect("done.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<CupcakeBakeScene>();
    }), NULL));
}