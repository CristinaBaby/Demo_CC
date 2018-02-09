
#include "ShopLayer.h"
#include "SceneManager.h"

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
    pRootNode->setPosition(Vec2(0, visibleSize.height));
    pRootNode->runAction(EaseBackOut::create(MoveBy::create(2, Vec2(0, -visibleSize.height))));
    
    //    bg
    Sprite* pBg = Sprite::create(localPath("shop.png"));
    CMVisibleRect::setPosition(pBg, pShopLayer->getChildByName("shop")->getPosition());
    pRootNode->addChild(pBg);
    
    Sprite* pLogo = Sprite::create(localPath("shop_logo.png"));
    CMVisibleRect::setPosition(pLogo, pShopLayer->getChildByName("shop_logo")->getPosition());
    pRootNode->addChild(pLogo);
    
    Button* pBackBtn = Button::create(localPath("btn_close.png"));
    pBackBtn->addTouchEventListener([=](Ref* ref,Widget::TouchEventType type){
        if (type==Widget::TouchEventType::ENDED) {
            pBackBtn->setEnabled(false);
            _dismiss();
        }
    });
    pRootNode->addChild(pBackBtn);
    CMVisibleRect::setPosition(pBackBtn, pShopLayer->getChildByName("btn_close")->getPosition());
    
    Button* pRestoreBtn = Button::create(localPath("btn_restore.png"));
    pRestoreBtn->addTouchEventListener([=](Ref* ref,Widget::TouchEventType type){
        if (type==Widget::TouchEventType::ENDED) {
            pRestoreBtn->setEnabled(false);
            
            ShopAdapter::getInstance()->requestRestore();
            pRestoreBtn->setEnabled(true);
        }
    });
    pRootNode->addChild(pRestoreBtn);
    CMVisibleRect::setPosition(pRestoreBtn, pShopLayer->getChildByName("btn_restore")->getPosition());
    
    int count = ConfigManager::getInstance()->getIapDataVector().size();
    //    for (int i = 0; i<count; i++) {
    //        Button* pBuyBtn = Button::create(localPath("btn_buy.png"));
    //        pRootNode->addChild(pBuyBtn);
    //        CMVisibleRect::setPosition(pBuyBtn, pShopLayer->getChildByName("btn_buy"+to_string(i))->getPosition());
    //        pBuyBtn->setTag(i);
    //
    //        pBuyBtn->addTouchEventListener([=](Ref* ref,Widget::TouchEventType type){
    //            if (type==Widget::TouchEventType::ENDED) {
    //                pBuyBtn->setEnabled(false);
    //                int index = pBuyBtn->getTag();
    //                ShopAdapter::getInstance()->requestBuyItem(ConfigManager::getInstance()->getIapData(index).iapId);
    //                pBuyBtn->setEnabled(true);
    //            }
    //        });
    //
    //        pBuyBtn->setScale(0.3);
    //        pBuyBtn->setVisible(false);
    //        pBuyBtn->runAction(Sequence::create(DelayTime::create(1),
    //                                            CallFunc::create([=](){
    //            pBuyBtn->setVisible(true);
    //        }),
    //                                            ScaleTo::create(0.3, 1), NULL));
    //    }
    
    for (int i = 0; i<count; i++) {
        IAPConfigData data = ConfigManager::getInstance()->getIapData(i);
        IAPItem* pItem = IAPItem::create();
        pItem->initWithName(localPath(data.iconName), "");
        pRootNode->addChild(pItem);
        pItem->setBuyPosition(Vec2(0,-200));
        std::string name = StringHelp::split(data.iconName, ".").at(0);
        Node* pCocosNode = pShopLayer->getChildByName(name);
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
    pBackBtn->setScale(0,1);
    pRestoreBtn->runAction(Sequence::create(DelayTime::create(1),
                                            CallFunc::create([=](){
        pRestoreBtn->setVisible(true);
    }),
                                            ScaleTo::create(0.5, 1), NULL));
    pBackBtn->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=](){
        pBackBtn->setVisible(true);
    }),
                                         ScaleTo::create(0.5, 1), NULL));
    
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
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ShopAdapter::getInstance()->requestBuyItem(ConfigManager::getInstance()->getIapData(index).iapId);
#else
    ShopAdapter::getInstance()->requestBuyItem(index+1);
#endif
}
