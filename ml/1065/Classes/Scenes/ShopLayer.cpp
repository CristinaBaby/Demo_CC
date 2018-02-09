
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
    
    
    Sprite* pFlag = Sprite::create(localPath("flag.png"));
    CMVisibleRect::setPosition(pFlag, 480, 0,kBorderNone,kBorderTop);
    this->addChild(pFlag);
    
    Sprite* pLight = Sprite::create(localPath("light.png"));
    CMVisibleRect::setPosition(pLight, 480, 0,kBorderNone,kBorderTop);
    this->addChild(pLight);
    
    Button* pBackBtn = Button::create("content/common/button/btn_back.png");
    pBackBtn->addTouchEventListener([=](Ref* ref,Widget::TouchEventType type){
        if (type==Widget::TouchEventType::ENDED) {
            pBackBtn->setEnabled(false);
            _dismiss();
        }
    });
    this->addChild(pBackBtn);
    CMVisibleRect::setPosition(pBackBtn, 10, 10,kBorderLeft,kBorderTop);
    
    Button* pRestoreBtn = Button::create(localPath("restore.png"));
    pRestoreBtn->addTouchEventListener([=](Ref* ref,Widget::TouchEventType type){
        if (type==Widget::TouchEventType::ENDED) {
            pRestoreBtn->setEnabled(false);
            
            ShopAdapter::getInstance()->requestRestore();
            pRestoreBtn->setEnabled(true);
        }
    });
    this->addChild(pRestoreBtn);
    CMVisibleRect::setPosition(pRestoreBtn, 10, -50,kBorderRight,kBorderBottom);
    
    int count = ConfigManager::getInstance()->getIapDataVector().size();
    for (int i = 0; i<count; i++) {
        IAPConfigData data = ConfigManager::getInstance()->getIapData(i);
        IAPItem* pItem = IAPItem::create();
        if (i==0) {
            pItem->initWithName(localPath(data.iconName), localPath("buy_big.png"));
        }else{
            pItem->initWithName(localPath(data.iconName), localPath("buy_small.png"));
        }
        this->addChild(pItem);
        //        pItem->setPosition(Vec2(320, 270*(count-i)));
        CMVisibleRect::setPosition(pItem, gItemPos[i]);
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
