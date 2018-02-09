
#include "ChooseScene.h"
#include "SceneManager.h"

ChooseScene::ChooseScene()
{
    m_bChoosen = false;
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
    
    //    bg
    Sprite* pBg = Sprite::create(localPath("choose_bg.jpg"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_choose_page_0.mp3");
    }),
                                     DelayTime::create(4),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_choose_page_1.mp3");
        
    }), NULL));
    for (int i = 2; i>=0; i--) {
        HomeCart* pCart = HomeCart::create();
        pCart->loadCartType(i+1);
        this->addChildToContentLayer(pCart);
        pCart->setLocalZOrder(1);
        pCart->runCart();
        pCart->setTag(i);
        pCart->setTouchEnable(false);
        pCart->onCartClickCallback = CC_CALLBACK_1(ChooseScene::_onCartCallback, this);
        CMVisibleRect::setPositionAdapted(pCart, 500+visibleSize.width, 240+230*i);
        
        int k = i;
        if(i==0){
            k = 1;
        }else if(i==1){
            k = 2;
        }else if(i==2){
            k = 0;
        }
        pCart->runAction(Sequence::create(DelayTime::create(0.5+1.5*k),
                                          EaseBackOut::create(MoveBy::create(3, Vec2(-visibleSize.width, 0))),
                                          CallFunc::create([=](){
            pCart->stopRunCart();
            pCart->setTouchEnable(true);
        }), NULL));
        
        m_pCarVector.push_back(pCart);
        
        Button* pButton = Button::create(localPath("hand"+std::to_string(i+1)+".png"));
        this->addChildToContentLayer(pButton);
        pButton->setLocalZOrder(0);
        CMVisibleRect::setPositionAdapted(pButton, 100-visibleSize.width, 240+230*i);
        pButton->setTag(i);
        pButton->setTouchEnabled(false);
        pButton->addTouchEventListener(CC_CALLBACK_2(ChooseScene::_onItemButtonCallback, this));
        pButton->runAction(Sequence::create(DelayTime::create(2.5+1.5*i),
                                            MoveBy::create(2, Vec2(visibleSize.width,0)),
                                            CallFunc::create([=](){
            pButton->setTouchEnabled(true);
            pButton->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.5, Vec2(50, 0)),
                                                                      MoveBy::create(0.5, Vec2(-10, 0)),
                                                                      MoveBy::create(0.5, Vec2(10, 0)),
                                                                      MoveBy::create(0.5, Vec2(-50, 0)),
                                                                      MoveBy::create(0.5, Vec2(50, 0)),
                                                                      MoveBy::create(0.5, Vec2(-10, 0)),
                                                                      MoveBy::create(0.5, Vec2(10, 0)),
                                                                      MoveBy::create(0.5, Vec2(-50, 0)),
                                                                      DelayTime::create(1), NULL)));
        }), NULL));
        
        Sprite* pLock = Sprite::create("content/common/video.png");
        pCart->addChild(pLock,10,TAG_REWARD_LOCK);
        
        if (ConfigManager::getInstance()->getVideoUnLocked("pack", i)){
            pLock->setVisible(false);
        }else{
            LockUIManager::getInstance()->registerLock("pack", i, pCart);
        }
    }
    
    AudioHelp::getInstance()->playBackGroundMusic("background.mp3");
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
    setExPath("content/start/");
}


void ChooseScene::_onItemButtonCallback(Ref* ref,Widget::TouchEventType type)
{
    if(type != Widget::TouchEventType::ENDED){
        return;
    }
    Button* pButton = (Button*)ref;
    _onItemCallback(pButton->getTag());
}

void ChooseScene::_onCartCallback(HomeCart* cart)
{
    _onItemCallback(cart->getTag());
    
}

void ChooseScene::_onItemCallback(int tag)
{
    if(m_bChoosen) {
        return;
    }
    m_bChoosen = true;
    std::string name = "pack"+std::to_string(tag);
    if(!ConfigManager::getInstance()->getVideoUnLocked("pack", tag)){
        ShopMiniLayer* pLayer = ShopMiniLayer::create(false);
        this->addChildToUILayer(pLayer);
        pLayer->setLocalZOrder(100);
        pLayer->shopGetFree = [=](){
            RewardManager::getInstance()->showRewardAds("pack", tag);
            pLayer->removeFromParent();
        };
        m_bChoosen = false;
        return;
    }
    GameDataManager::getInstance()->m_nPackage = tag;
    if (!UserDefault::getInstance()->getBoolForKey("removeAds")){
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeInterstitialAds);
        
    }
    Node* pNode = m_pCarVector.at(2-tag);
    pNode->runAction(Spawn::create(MoveBy::create(1, Vec2(-200, 50)),
                                   ScaleTo::create(1,1.2), NULL));
    MenuLayer::reset();
    AudioHelp::getInstance()->playBackGroundMusic("background_make.mp3");
    switch (tag) {
        case 0://cottoncandy
        {
            AudioHelp::getInstance()->playEffect("vo_choose_cottoncandy.mp3");
            this->runAction(Sequence::create(DelayTime::create(2.5),
                                             CallFunc::create([=](){
                SceneManager::replaceTheScene<CottonDisplayIngridentScene>();
            }), NULL));
        }
            break;
        case 1://funnel cake
        {
            AudioHelp::getInstance()->playEffect("vo_choose_funnelcake.mp3");
            this->runAction(Sequence::create(DelayTime::create(2.5),
                                             CallFunc::create([=](){
                SceneManager::replaceTheScene<FunnelDisplayIngridentScene>();
            }), NULL));
        }
            break;
        case 2://snowcont
        {
            AudioHelp::getInstance()->playEffect("vo_choose_snowcone.mp3");
            this->runAction(Sequence::create(DelayTime::create(2.5),
                                             CallFunc::create([=](){
                SceneManager::replaceTheScene<SnowconeDisplayIngridentScene>();
            }), NULL));
        }
            break;
            
        default:
            break;
    }

}