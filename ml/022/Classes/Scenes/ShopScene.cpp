
#include "ShopScene.h"
#include "SceneManager.h"

ShopScene::ShopScene()
{
    m_InnerPosition = Vec2::ZERO;
    m_bIsScrolling = false;
}

ShopScene::~ShopScene()
{
    
}
bool ShopScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    
    //    bg
    Sprite* pBg = Sprite::create(localPath("bg.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    Sprite* pLogo = Sprite::create(localPath("logo.png"));
    CMVisibleRect::setPositionAdapted(pLogo, 320, 880,kBorderNone,kBorderTop);
    this->addChildToUILayer(pLogo);
    
    Sprite* pLogoText = Sprite::create(localPath("text_shop.png"));
    CMVisibleRect::setPositionAdapted(pLogoText, 320, 900,kBorderNone,kBorderTop);
    this->addChildToUILayer(pLogoText);
    
    Button* pBackBtn = Button::create(localPath("btn_back.png"));
    pBackBtn->addTouchEventListener([=](Ref* ref,Widget::TouchEventType type){
        if (type==Widget::TouchEventType::ENDED) {
            pBackBtn->setEnabled(false);
            SceneManager::popScene();
        }
    });
    this->addChildToUILayer(pBackBtn);
    CMVisibleRect::setPositionAdapted(pBackBtn, 75, 900,kBorderLeft,kBorderTop);
    
    
    m_pScrollView = ExpandScrollView::create();
    m_pScrollView->setContentSize(Size(visibleSize.width, visibleSize.height-110));
    m_pScrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    CMVisibleRect::setPositionAdapted(m_pScrollView, 0, 0,kBorderNone,kBorderBottom);
    this->addChildToContentLayer(m_pScrollView);
    
    m_pScrollView->setTouchEnabled(true);
    m_pScrollView->setClippingEnabled(true);
    int count = ConfigManager::getInstance()->getIapDataVector().size();
    for (int i = 0; i<count; i++) {
        IAPConfigData data = ConfigManager::getInstance()->getIapData(i);
        IAPItem* pItem = IAPItem::create();
        pItem->initWithName(localPath(data.iconName), localPath("btn_buy.png"));
        m_pScrollView->getInnerContainer()->addChild(pItem);
        pItem->setPosition(Vec2(320, 270*(count-i)));
        pItem->setTag(i);
        pItem->onItemClicked = CC_CALLBACK_1(ShopScene::onItemClicked, this);
        m_ItemVector.pushBack(pItem);
    }
    m_pScrollView->setInnerContainerSize(Size(640, 200+270*count));
    _showScrollBottom();
    
    return true;
}

void ShopScene::onEnter()
{
    ExtensionScene::onEnter();
    
    if (!UserDefault::getInstance()->getBoolForKey("removeAds")){
        AdsManager::getInstance()->removeAds(kTypeBannerAds);
        
    }
}
#pragma mark - initData
void ShopScene::_initData()
{
    
    setExPath("content/shop/");
}

void ShopScene::_showScrollBottom()
{
    m_pScrollView->scrollToBottom(3, true);
    this->runAction(Sequence::create(DelayTime::create(2.1),
                                     CallFunc::create([=]()
                                                      {
                                                          this->_showScrollBottomBack();
                                                      }),
                                     NULL));
}

void ShopScene::_showScrollBottomBack()
{
    m_pScrollView->scrollToTop(1, true);
}

void ShopScene::onItemClicked(int index)
{
    if (m_pScrollView->isScrolled()) {
        return;
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ShopAdapter::getInstance()->requestBuyItem(ConfigManager::getInstance()->getIapData(index).iapId);
#else
    ShopAdapter::getInstance()->requestBuyItem(index+1);
#endif
}
