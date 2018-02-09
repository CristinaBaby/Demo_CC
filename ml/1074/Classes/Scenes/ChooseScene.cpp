
#include "ChooseScene.h"
#include "SceneManager.h"

static std::string gsPackage[] ={
    "cupcake",
    "cake",
    "donut",
};
ChooseScene::ChooseScene()
{
    m_bItemEnable = false;
}

ChooseScene::~ChooseScene()
{
    
}
bool ChooseScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    LockUIManager::getInstance()->onRewardSuccess = nullptr;
    
    //    bg
    m_pBg = Sprite::create(localPath("bg.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    Sprite* pTable = Sprite::create(localPath("bg1.png"));
    pTable->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pTable);
    
    _showScrollView();
    AudioHelp::getInstance()->playBackGroundMusic("music.mp3");
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_choice.mp3");
    }),
                                     NULL));
    
    m_pGameUI->showNormalLayout();
    m_pGameUI->hideButton(GameUILayoutLayer::eUIButtonTagMenu);
    GameDataManager::getInstance()->m_sFlavorStr = "";
    GameDataManager::getInstance()->m_sFlavorCakeStr = "";
    GameDataManager::getInstance()->m_sFlavorCreamStr = "";
    
    return true;
}

void ChooseScene::onEnter()
{
    ExtensionScene::onEnter();
}

void ChooseScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void ChooseScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
}

void ChooseScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void ChooseScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
}

void ChooseScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void ChooseScene::_initData()
{
    setExPath("content/choose/");
}

void ChooseScene::_showScrollView()
{
    m_pScrollView = ExpandScrollView::create();
    m_pScrollView->setContentSize(Size(visibleSize.width,600));
    this->addChildToContentLayer(m_pScrollView);
    CMVisibleRect::setPositionAdapted(m_pScrollView, visibleSize.width, 240,kBorderLeft,kBorderBottom);
    m_pScrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    m_pScrollView->setLocalZOrder(5);
    m_pScrollView->setScrollBarEnabled(false);
    m_pScrollView->setClippingEnabled(false);
    
    int typeCount = 3;
    m_pScrollView->getInnerContainer()->setContentSize(Size((typeCount-1)*440+300 +visibleSize.width/2, 600));
    
    for (int i = 0; i<typeCount; i++) {
        LockItem* pItem = LockItem::create();
        
        bool bLock = false;
        if (!gNoneIap && !IAPManager::getInstance()->getItemIsBought(2) && i>0){
            if (!ConfigManager::getInstance()->getVideoUnLocked("pack", i)) {
                LockUIManager::getInstance()->registerLock("pack", i, pItem);
                LockUIManager::getInstance()->setLockOriginPos(Vec2(40, -60));
                pItem->lockType = 1;
                bLock = true;
            }
        }
        pItem->initWithName(localPath("plate.png"), "content/common/video.png",bLock);
        Sprite* Lock = pItem->getLockSprite();
        if (Lock) {
            Lock->setTag(TAG_REWARD_LOCK);
        }
        
        pItem->setTag(i);
        m_pScrollView->getInnerContainer()->addChild(pItem);
        pItem->setPosition(Vec2(300+i*440,300));
        
        ContainerNode* pContainer = ContainerNode::create(pItem);
        pContainer->addBody(localPath("down.png"));
        pContainer->addFood(localPath(gsPackage[i]+".png"));
        pContainer->addFront(localPath("on.png"),"cap");
        pContainer->addFront(localPath(gsPackage[i]+"1.png"),"label",Vec2(19, -200));
        pItem->setUserData(pContainer);
        
        pItem->onItemClicked = CC_CALLBACK_1(ChooseScene::_onPackageClicked, this);
        m_typeItems.pushBack(pItem);
    }
    m_pScrollView->runAction(Sequence::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                              CallFunc::create([=](){
        m_bItemEnable = true;
    }), NULL));
    
}

void ChooseScene::_onPackageClicked(LockItem* item)
{
    if (!m_bItemEnable) {
        return;
    }
    if (m_pScrollView->isScrolled()) {
        return;
    }
    int tag = item->getTag();
    if(item->isLocked()){
        if(!ConfigManager::getInstance()->getVideoUnLocked("pack", tag)){
            ShopMiniLayer* pLayer = ShopMiniLayer::create(false);
            this->addChild(pLayer);
            pLayer->setLocalZOrder(100);
            pLayer->shopGetFree = [=](){
                LockUIManager::getInstance()->onRewardSuccess = [=](){
                    AudioHelp::getInstance()->playEffect("bling.mp3");
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/goodParticle.plist");
                    item->addChild(pParticle,100);
                    pParticle->setPosition(Vec2::ZERO);
                    item->unlock();
                };
                RewardManager::getInstance()->showRewardAds("pack", tag);
                pLayer->removeFromParent();
            };
            
            pLayer->shopDismissed = [=](){
                if (IAPManager::getInstance()->getItemIsBought(2)) {
                    for_each(m_typeItems.begin(), m_typeItems.end(), [=](LockItem* pItem){
                        pItem->unlock();
                    });
                }
            };
            return;
        }
    }
    m_bItemEnable = false;
    AudioHelp::getInstance()->playSelectedEffect();
    GameDataManager::getInstance()->m_nPackage = tag;
    for (auto pCake:m_typeItems) {
        pCake->setItemTouchEnable(false);
        if (pCake!=item) {
            Vec2 pos = pCake->getPosition();
            pos = pCake->getParent()->convertToWorldSpace(pos);
            if (pos.x<visibleSize.width/2) {
                pCake->runAction(MoveBy::create(1, Vec2(-visibleSize.width, 0)));
            }else{
                pCake->runAction(MoveBy::create(1, Vec2(visibleSize.width, 0)));
            }
        }
    }
    m_pScrollView->setTouchEnabled(false);
    Vec2 pos = Vec2(visibleSize.width*0.5,visibleSize.height*0.5+20);
    pos = item->getParent()->convertToNodeSpace(pos);
    item->runAction(Sequence::create(ActionHelper::getJellyAction(),
                                     MoveTo::create(0.5, pos),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("open_glass_cover.mp3");
        Node* pCap =  item->getChildByName("cap");
        if (pCap) {
            pCap->runAction(JumpBy::create(2, Vec2(visibleSize.width*1.2, -100), 300, 1));
            pCap->runAction(RotateBy::create(2.5, 120));
        }
        Node* pBody =  item->getChildByName("body");
        if (pBody) {
            pBody->runAction(JumpBy::create(2, Vec2(visibleSize.width*1.2, -100), 300, 1));
            pBody->runAction(RotateBy::create(2.5, 120));
        }
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
        this->addChildToUILayer(pParticle);
        pParticle->setLocalZOrder(10);
        pParticle->setPosition(visibleSize*0.5);
        if (tag == 0) {
            AudioHelp::getInstance()->playEffect("vo_cupcake.mp3");
        }else if (tag == 1) {
            AudioHelp::getInstance()->playEffect("vo_shortcake.mp3");
        }else{
            AudioHelp::getInstance()->playEffect("vo_donuts.mp3");
        }
        if (!UserDefault::getInstance()->getBoolForKey("removeAds")){
            AdsManager::getInstance()->showAds(ADS_TYPE::kTypeInterstitialAds);
        }
    }),
                                     DelayTime::create(4),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playBackGroundMusic("music_make.mp3");
        if (tag == 0) {
            SceneManager::replaceTheScene<CupcakeDisplayIngredientScene>();
        }else if (tag == 1) {
            SceneManager::replaceTheScene<ShortcakeDisplayIngredientScene>();
        }else{
            SceneManager::replaceTheScene<DonutDisplayIngredientScene>();
        }
        
    }), NULL));
}