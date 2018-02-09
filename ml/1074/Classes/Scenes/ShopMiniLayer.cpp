
#include "ShopMiniLayer.h"
#include "SceneManager.h"

ShopMiniLayer::ShopMiniLayer()
{
    m_InnerPosition = Vec2::ZERO;
    m_bIsScrolling = false;
    shopDismissed = nullptr;
    m_bShowBanner = true;
    m_nPercent = 0;
    shopGetFree = nullptr;
}

ShopMiniLayer::~ShopMiniLayer()
{
    
}
ShopMiniLayer* ShopMiniLayer::create(bool dec)
{ 
    ShopMiniLayer *pRet = new(std::nothrow) ShopMiniLayer(); 
    if (pRet && pRet->init(dec))
    { 
        pRet->autorelease(); 
        return pRet; 
    } 
    else 
    { 
        delete pRet; 
        pRet = NULL; 
        return NULL; 
    }
}
bool ShopMiniLayer::init(bool dec)
{
    if ( !Layer::init() )
    {
        return false;
    }
    _initData();
    LayerColor* color = LayerColor::create(Color4B(0, 0, 0, 160));
    this->addChild(color);
    
    Node* pShopMiniLayer = CSLoader::createNode("MiniShopLayer.csb");
    
    Node* pRootNode = Node::create();
    this->addChild(pRootNode);
    pRootNode->setPosition(Vec2(0, visibleSize.height));
    pRootNode->runAction(EaseBackOut::create(MoveBy::create(2, Vec2(0, -visibleSize.height))));
    
    //    bg
    Sprite* pBg = Sprite::create(localPath("mini/mini_frame.png"));
    CMVisibleRect::setPosition(pBg, pShopMiniLayer->getChildByName("mini_frame")->getPosition());
    pRootNode->addChild(pBg,-2);
    
    Sprite* pBg2 = Sprite::create(localPath("mini/mini_frame1.png"));
    CMVisibleRect::setPosition(pBg2, pShopMiniLayer->getChildByName("mini_frame1")->getPosition());
    pRootNode->addChild(pBg2);
    
    Button* pBackBtn = Button::create(localPath("btn_close.png"));
    pBackBtn->addTouchEventListener([=](Ref* ref,Widget::TouchEventType type){
        if (type==Widget::TouchEventType::ENDED) {
            pBackBtn->setEnabled(false);
            _dismiss();
        }
    });
    pRootNode->addChild(pBackBtn);
    CMVisibleRect::setPosition(pBackBtn, pShopMiniLayer->getChildByName("btn_close")->getPosition());
    
    Button* pRestoreBtn = Button::create(localPath("mini/minishop.png"));
    pRestoreBtn->addTouchEventListener([=](Ref* ref,Widget::TouchEventType type){
        if (type==Widget::TouchEventType::ENDED) {
            ShopLayer* pLayer = ShopLayer::create();
            this->addChild(pLayer);
            pLayer->setLocalZOrder(100);
        }
    });
    pRootNode->addChild(pRestoreBtn);
    CMVisibleRect::setPosition(pRestoreBtn, pShopMiniLayer->getChildByName("minishop")->getPosition());
    
    Button* pRewardBtn = Button::create(localPath("mini/btn_free.png"));
    pRewardBtn->addTouchEventListener([=](Ref* ref,Widget::TouchEventType type){
        if (type==Widget::TouchEventType::ENDED) {
            if(shopGetFree){
                shopGetFree();
            }
        }
    });
    pRootNode->addChild(pRewardBtn,-1);
    CMVisibleRect::setPosition(pRewardBtn, pShopMiniLayer->getChildByName("btn_free")->getPosition());
    
    int count = 2;
    
    for (int i = 0; i<count; i++) {
        int k = i;
        if (!dec && i == 1) {
            k++;
        }
        IAPConfigData data = ConfigManager::getInstance()->getIapData(k);
        IAPItem* pItem = IAPItem::create();
        pItem->initWithName(localPath("mini/mini_"+data.iconName), "");
        pRootNode->addChild(pItem);
        pItem->setBuyPosition(Vec2(0,-200));
        std::string name = StringHelp::split(data.iconName, ".").at(0);
        Node* pCocosNode = pShopMiniLayer->getChildByName("mini_"+name);
        CMVisibleRect::setPosition(pItem, pCocosNode->getPosition());
        pItem->setTag(k);
        pItem->onItemClicked = CC_CALLBACK_1(ShopMiniLayer::onItemClicked, this);
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

void ShopMiniLayer::onEnter()
{
    Layer::onEnter();
    
    //    if (!UserDefault::getInstance()->getBoolForKey("removeAds")){
    //        AdsManager::getInstance()->removeAds(kTypeBannerAds);
    //    }
}

void ShopMiniLayer::onExit()
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
void ShopMiniLayer::_initData()
{
    
    setExPath("content/shop/");
}

void ShopMiniLayer::_dismiss()
{
    if (shopDismissed) {
        shopDismissed();
    }
    this->removeFromParent();
}
void ShopMiniLayer::onItemClicked(int index)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ShopAdapter::getInstance()->requestBuyItem(ConfigManager::getInstance()->getIapData(index).iapId);
#else
    ShopAdapter::getInstance()->requestBuyItem(index+1);
#endif
}
