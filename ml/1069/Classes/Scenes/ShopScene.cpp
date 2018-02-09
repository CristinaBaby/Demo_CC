
#include "ShopScene.h"
#include "SceneManager.h"

static Vec2 gItemPos[]={
    Vec2(480,310),
    Vec2(180,370),
    Vec2(810,350),
};
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
    Sprite* pBg = Sprite::create(localPath("bg.png"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    Sprite* pFlag = Sprite::create(localPath("flag.png"));
    CMVisibleRect::setPosition(pFlag, 480, 0,kBorderNone,kBorderTop);
    this->addChildToUILayer(pFlag);
    
    Sprite* pLight = Sprite::create(localPath("light.png"));
    CMVisibleRect::setPosition(pLight, 480, 0,kBorderNone,kBorderTop);
    this->addChildToContentLayer(pLight);
    
    Button* pBackBtn = Button::create("content/common/button/btn_back.png");
    pBackBtn->addTouchEventListener([=](Ref* ref,Widget::TouchEventType type){
        if (type==Widget::TouchEventType::ENDED) {
            pBackBtn->setEnabled(false);
            SceneManager::popScene();
        }
    });
    this->addChildToUILayer(pBackBtn);
    CMVisibleRect::setPosition(pBackBtn, 10, 10,kBorderLeft,kBorderTop);
    
    Button* pRestoreBtn = Button::create(localPath("restore.png"));
    pRestoreBtn->addTouchEventListener([=](Ref* ref,Widget::TouchEventType type){
        if (type==Widget::TouchEventType::ENDED) {
            pRestoreBtn->setEnabled(false);
            
            ShopAdapter::getInstance()->requestRestore();
            pRestoreBtn->setEnabled(true);
        }
    });
    this->addChildToUILayer(pRestoreBtn);
    CMVisibleRect::setPosition(pRestoreBtn, 10, -50,kBorderRight,kBorderBottom);
    
//    m_pScrollView = ExpandScrollView::create();
//    m_pScrollView->setContentSize(Size(visibleSize.width, visibleSize.height-110));
//    m_pScrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
//    CMVisibleRect::setPositionAdapted(m_pScrollView, 0, 0,kBorderNone,kBorderBottom);
//    this->addChildToContentLayer(m_pScrollView);
//    
//    m_pScrollView->setTouchEnabled(true);
//    m_pScrollView->setClippingEnabled(true);
    int count = ConfigManager::getInstance()->getIapDataVector().size();
    for (int i = 0; i<count; i++) {
        IAPConfigData data = ConfigManager::getInstance()->getIapData(i);
        IAPItem* pItem = IAPItem::create();
        if (i==0) {
            pItem->initWithName(localPath(data.iconName), localPath("buy_big.png"));
        }else{
            pItem->initWithName(localPath(data.iconName), localPath("buy_small.png"));
        }
//        m_pScrollView->getInnerContainer()->addChild(pItem);
        this->addChildToContentLayer(pItem);
//        pItem->setPosition(Vec2(320, 270*(count-i)));
        CMVisibleRect::setPosition(pItem, gItemPos[i]);
        pItem->setTag(i);
        pItem->onItemClicked = CC_CALLBACK_1(ShopScene::onItemClicked, this);
        m_ItemVector.pushBack(pItem);
    }
//    m_pScrollView->setInnerContainerSize(Size(640, 200+270*count));
//    _showScrollBottom();

    return true;
}

void ShopScene::onEnter()
{
    ExtensionScene::onEnter();
    
//    if (!UserDefault::getInstance()->getBoolForKey("removeAds")){
//        AdsManager::getInstance()->removeAds(kTypeBannerAds);
//        
//    }
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
//    if (m_pScrollView->isScrolled()) {
//        return;
//    }
    
    ShopAdapter::getInstance()->requestBuyItem(ConfigManager::getInstance()->getIapData(index).iapId);
}
