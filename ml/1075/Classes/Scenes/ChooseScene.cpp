
#include "ChooseScene.h"
#include "SceneManager.h"
#include "MenuLayer.h"

static std::string gsPackage[] ={
    "egg",
    "bacon",
    "cornflakes",
    "juice",
    "omelette",
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
    m_pBg = Sprite::create(localPath("choose_bg.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    _showScrollView();
    AudioHelp::getInstance()->playBackGroundMusic("music.mp3");
    
    if (GameDataManager::getInstance()->m_bFirst) {
        this->runAction(Sequence::create(DelayTime::create(0.5),
                                         CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("vo_good_morning.mp3");
        }),
                                         DelayTime::create(3.5),
                                         CallFunc::create([=](){
            AudioHelp::getInstance()->playEffect("vo_choose_page.mp3");
        }), NULL));
    }
    
    m_pGameUI->showNormalLayout();
    m_pGameUI->hideButton(GameUILayoutLayer::eUIButtonTagMenu);
    
    GameDataManager::getInstance()->m_nCupIndex = -1;
    MenuLayer::reset();
    
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
    
    int typeCount = 5;
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
        pItem->initWithName(localPath("pack_"+gsPackage[i]+".png"), "content/common/video.png",bLock);
        Sprite* Lock = pItem->getLockSprite();
        if (Lock) {
            Lock->setTag(TAG_REWARD_LOCK);
        }
        
        pItem->setTag(i);
        m_pScrollView->getInnerContainer()->addChild(pItem);
        pItem->setPosition(Vec2(300+i*440,300));
        
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
    GameDataManager::getInstance()->m_bFirst = false;
    m_pScrollView->setTouchEnabled(false);
    Vec2 pos = Vec2(visibleSize.width*0.5,visibleSize.height*0.5+20);
    pos = item->getParent()->convertToNodeSpace(pos);
    item->runAction(Sequence::create(ActionHelper::getJellyAction(),
                                     MoveTo::create(0.5, pos),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("magic_effect.mp3");
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
        this->addChildToUILayer(pParticle);
        pParticle->setLocalZOrder(10);
        pParticle->setPosition(visibleSize*0.5);
        if (tag == 0) {
            AudioHelp::getInstance()->playEffect("vo_fried_egg.mp3");
        }else if (tag == 1) {
            AudioHelp::getInstance()->playEffect("vo_fried_acon_ham.mp3");
        }else if (tag == 2) {
            AudioHelp::getInstance()->playEffect("vo_corn_flakes.mp3");
        }else if (tag == 3) {
            AudioHelp::getInstance()->playEffect("vo_juice.mp3");
        }else{
            AudioHelp::getInstance()->playEffect("vo_omelette.mp3");
        }
        if (!UserDefault::getInstance()->getBoolForKey("removeAds")){
            AdsManager::getInstance()->showAds(ADS_TYPE::kTypeInterstitialAds);
        }
    }),
                                     DelayTime::create(4),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playBackGroundMusic("music_make.mp3");
        if (tag == 0) {
            SceneManager::replaceTheScene<EggDisplayIngredientScene>();
        }else if (tag == 1) {
            SceneManager::replaceTheScene<BaconHamDisplayIngredientScene>();
        }else if (tag == 2) {
            SceneManager::replaceTheScene<CornflakeDisplayIngredientScene>();
        }else if (tag == 3) {
            SceneManager::replaceTheScene<JuiceDisplayIngredientScene>();
        }else{
            SceneManager::replaceTheScene<OmeletteDisplayIngredientScene>();
        }
        
    }), NULL));
}