
#include "ShopLayer.h"
#include "SceneManager.h"


static Vec2 gItemPos[]={
    Vec2(480,310),
    Vec2(180,370),
    Vec2(810,350),
};
ShopLayer::ShopLayer()
{
    m_InnerPosition = Vec2::ZERO;
    m_bIsScrolling = false;
    shopDismissed = nullptr;
    m_bShowBanner = true;
    m_nPercent = 0;
}

ShopLayer::~ShopLayer()
{
    
}
bool ShopLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    _initData();
    LayerColor* color = LayerColor::create(Color4B(0, 0, 0, 160));
    this->addChild(color);
    
    //    bg
    Sprite* pBg = Sprite::create(localPath("bg.png"));
    pBg->setPosition(visibleSize*0.5);
    this->addChild(pBg);
    
    
    Button* pBackBtn = Button::create("content/common/button/btn_back.png");
    pBackBtn->addTouchEventListener([=](Ref* ref,Widget::TouchEventType type){
        if (type==Widget::TouchEventType::ENDED) {
            pBackBtn->setEnabled(false);
            _dismiss();
        }
    });
    this->addChild(pBackBtn);
    CMVisibleRect::setPosition(pBackBtn, 10, 10,kBorderLeft,kBorderTop);
    
    Button* pRestoreBtn = Button::create(localPath("restore_purchases.png"));
    pRestoreBtn->addTouchEventListener([=](Ref* ref,Widget::TouchEventType type){
        if (type==Widget::TouchEventType::ENDED) {
            pRestoreBtn->setEnabled(false);
            
            ShopAdapter::getInstance()->requestRestore();
            pRestoreBtn->setEnabled(true);
        }
    });
    this->addChild(pRestoreBtn);
    CMVisibleRect::setPosition(pRestoreBtn, 260, 220);
    
    Button* pAdsBtn = Button::create(localPath("no_ads.png"));
    pAdsBtn->addTouchEventListener([=](Ref* ref,Widget::TouchEventType type){
        if (type==Widget::TouchEventType::ENDED) {
            pAdsBtn->setEnabled(false);
            ShopAdapter::getInstance()->requestBuyItem(ConfigManager::getInstance()->getIapData(3).iapId);
            pAdsBtn->setEnabled(true);
        }
    });
    this->addChild(pAdsBtn);
    CMVisibleRect::setPosition(pAdsBtn, 500, 210);
    
    
    int count = ConfigManager::getInstance()->getIapDataVector().size();
    for (int i = 0; i<count-1; i++) {
        IAPConfigData data = ConfigManager::getInstance()->getIapData(i);
        IAPItem* pItem = IAPItem::create();
        pItem->initWithName(localPath(data.iconName), localPath("buy.png"));
        this->addChild(pItem);
        pItem->setBuyPosition(Vec2(220,0));
        //        pItem->setPosition(Vec2(320, 270*(count-i)));
        CMVisibleRect::setPosition(pItem, 256,650-160*i);
        pItem->setTag(i);
        pItem->onItemClicked = CC_CALLBACK_1(ShopLayer::onItemClicked, this);
        m_ItemVector.pushBack(pItem);
    }

    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *pTouch, Event *pEvent){
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void ShopLayer::onEnter()
{
    Layer::onEnter();
    
    if (!UserDefault::getInstance()->getBoolForKey("removeAds")){
        AdsManager::getInstance()->removeAds(kTypeBannerAds);
    }
}

void ShopLayer::onExit()
{
    if (m_bShowBanner) {
        if (!UserDefault::getInstance()->getBoolForKey("removeAds")){
            AdsManager::getInstance()->showAds(kTypeBannerAds);
            
        }
    }
    Layer::onExit();
}

#pragma mark - initData
void ShopLayer::_initData()
{
    
    setExPath("content/shop/");
}

void ShopLayer::_dismiss()
{
    if (shopDismissed) {
        shopDismissed();
    }
    this->removeFromParent();
}
void ShopLayer::onItemClicked(int index)
{
    ShopAdapter::getInstance()->requestBuyItem(ConfigManager::getInstance()->getIapData(index).iapId);
}
