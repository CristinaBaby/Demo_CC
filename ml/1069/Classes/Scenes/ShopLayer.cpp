
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
    
    Node* pShopLayer = CSLoader::createNode("ShopLayer.csb");
    
    Node* pRootNode = Node::create();
    this->addChild(pRootNode);
//    pRootNode->setPosition(Vec2(0,visibleSize.height));
//    pRootNode->runAction(Sequence::create(DelayTime::create(0.5),
//                                          EaseBackOut::create(MoveBy::create(1, Vec2(0,-visibleSize.height))), NULL));
    
    
    Sprite* pBg2 = Sprite::create(localPath("frame1.png"));
    CMVisibleRect::setPosition(pBg2, pShopLayer->getChildByName("frame1")->getPosition()+Vec2(0, visibleSize.height));
    pRootNode->addChild(pBg2);
    
    //    bg
    Sprite* pBg = Sprite::create(localPath("frame0.png"));
    CMVisibleRect::setPosition(pBg, pShopLayer->getChildByName("frame0")->getPosition());
    pRootNode->addChild(pBg);
    
    Sprite* pLogo = Sprite::create(localPath("shop_logo.png"));
    CMVisibleRect::setPosition(pLogo, pShopLayer->getChildByName("shop_logo")->getPosition());
    pRootNode->addChild(pLogo,11);
    
    
    Button* pBackBtn = Button::create(localPath("no.png"));
    pBackBtn->addTouchEventListener([=](Ref* ref,Widget::TouchEventType type){
        if (type==Widget::TouchEventType::ENDED) {
            pBackBtn->setEnabled(false);
            _dismiss();
        }
    });
    this->addChild(pBackBtn);
    CMVisibleRect::setPosition(pBackBtn, pShopLayer->getChildByName("no")->getPosition());
    
    Button* pRestoreBtn = Button::create(localPath("restore.png"));
    pRestoreBtn->addTouchEventListener([=](Ref* ref,Widget::TouchEventType type){
        if (type==Widget::TouchEventType::ENDED) {
            pRestoreBtn->setEnabled(false);
            
            ShopAdapter::getInstance()->requestRestore();
            pRestoreBtn->setEnabled(true);
        }
    });
    pRootNode->addChild(pRestoreBtn);
    CMVisibleRect::setPosition(pRestoreBtn, pShopLayer->getChildByName("restore")->getPosition());
    
    Button* pAdsBtn = Button::create(localPath("noads.png"));
    pAdsBtn->addTouchEventListener([=](Ref* ref,Widget::TouchEventType type){
        if (type==Widget::TouchEventType::ENDED) {
            pAdsBtn->setEnabled(false);
            ShopAdapter::getInstance()->requestBuyItem(ConfigManager::getInstance()->getIapData(3).iapId);
            pAdsBtn->setEnabled(true);
        }
    });
    pRootNode->addChild(pAdsBtn);
    CMVisibleRect::setPosition(pAdsBtn, pShopLayer->getChildByName("ads")->getPosition());
    
    
    int count = ConfigManager::getInstance()->getIapDataVector().size();
    for (int i = 0; i<count-1; i++) {
        IAPConfigData data = ConfigManager::getInstance()->getIapData(i);
        IAPItem* pItem = IAPItem::create();
        pItem->initWithName(localPath(data.iconName), "");
        pRootNode->addChild(pItem);
        pItem->setBuyPosition(Vec2(220,0));
        std::string name = StringHelp::split(data.iconName, ".").at(0);
        Node* pCocosNode = pShopLayer->getChildByName(name);
        pItem->getContentSprite()->setAnchorPoint(pCocosNode->getAnchorPoint());
        CMVisibleRect::setPosition(pItem, pCocosNode->getPosition());
        pItem->setTag(i);
        pItem->onItemClicked = CC_CALLBACK_1(ShopLayer::onItemClicked, this);
        m_ItemVector.pushBack(pItem);
        pItem->setScale(0.3);
        pItem->setVisible(false);
        pItem->runAction(Sequence::create(DelayTime::create(1),
                                          CallFunc::create([=](){
            pItem->setVisible(true);
        }),
                                          ScaleTo::create(0.3, 1), NULL));
    }
    
    pRestoreBtn->setVisible(false);
    pBackBtn->setVisible(false);
    pRestoreBtn->setScale(0,1);
    pBg2->runAction(Sequence::create(DelayTime::create(0.5),
                                     EaseBackOut::create(MoveBy::create(1, Vec2(0,-visibleSize.height))),
                                     CallFunc::create([=]{
        pBackBtn->setVisible(true);
        pRestoreBtn->setVisible(true);
        pRestoreBtn->runAction(ScaleTo::create(0.5, 1));
    }), NULL));
    
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
    
//    if (!UserDefault::getInstance()->getBoolForKey("removeAds")){
//        AdsManager::getInstance()->removeAds(kTypeBannerAds);
//    }
}

void ShopLayer::onExit()
{
//    if (m_bShowBanner) {
//        if (!UserDefault::getInstance()->getBoolForKey("removeAds")){
//            AdsManager::getInstance()->showAds(kTypeBannerAds);
//            
//        }
//    }
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
