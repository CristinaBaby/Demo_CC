
#include "ChoosePackageLayer.h"

ChoosePackageLayer::ChoosePackageLayer()
{
    m_bSelected = false;
    onChooseCallback = nullptr;
}

ChoosePackageLayer::~ChoosePackageLayer()
{
    
}
bool ChoosePackageLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    LayerColor* color = LayerColor::create(Color4B(0, 0, 0, 160));
    this->addChild(color);
    
    Sprite* pBanner = Sprite::create("content/choose/choose.png");
    this->addChild(pBanner);
    CMVisibleRect::setPositionAdapted(pBanner, 320, 1600);
    
    for (int i = 0; i<2; i++) {
        LockItem* pItem = LockItem::create();
        bool locked = false;
        std::string lockName = "";
        if (!IAPManager::getInstance()->getItemIsBought(2)){
            if (i>0){
                if (ConfigManager::getInstance()->getVideoUnLocked("pack", i)){
                    lockName = "";
                }else{
                    lockName = "content/common/video.png";
                    locked = true;
                }
            }
        }
        
        pItem->initWithName("content/choose/make"+std::to_string(i)+"_0.png", lockName,locked);
        pBanner->addChild(pItem);
//        pItem->setScale(0.5);
//        CMVisibleRect::setPositionAdapted(pItem, 140+280*(i%2), 200);
        pItem->setPosition(Vec2(180+280*(i%2), 200));
        if (locked) {
            Sprite* pLock = pItem->getLockSprite();
            if (pLock) {
                pLock->setTag(TAG_REWARD_LOCK);
                LockUIManager::getInstance()->registerLock("pack", i, pLock->getParent());
                LockUIManager::getInstance()->setLockPos(Vec2(50, -50));
            }
        }
        pItem->onItemClicked = CC_CALLBACK_1(ChoosePackageLayer::_onItemCallback, this);
        pItem->setItemTouchEnable(false);
        pItem->runAction(Sequence::create(DelayTime::create(0.5+0.3*i),
//                                          EaseElasticInOut::create(ScaleTo::create(0.5, 1)),
                                          CallFunc::create([=]()
                                                           {
                                                               if (i>0 && !locked) {
                                                                   ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dec.plist");
                                                                   pItem->addChild(pParticle,20);
                                                                   pParticle->setPosition(Vec2::ZERO);
                                                               }
                                                               pItem->setItemTouchEnable();
                                                           }), NULL));
        pItem->setTag(i);
        m_Items.pushBack(pItem);
    }
    m_bSelected = true;
    AudioHelp::getInstance()->playEffect("vo_choose_page.mp3");
    pBanner->runAction(Sequence::create(DelayTime::create(0.5),
                                        EaseElasticOut::create(MoveBy::create(1, Vec2(0, -1000)),1.0f),
                                        CallFunc::create([=](){
        m_bSelected = false;
    }),
                                        DelayTime::create(1),
                                        CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_choose_page1.mp3");
    }), NULL));
    
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *pTouch, Event *pEvent){
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void ChoosePackageLayer::onEnter()
{
    Layer::onEnter();
}

void ChoosePackageLayer::_onItemCallback(LockItem* pItem)
{
    if(m_bSelected){
        return;
    }
    int tag = pItem->getTag();
    if(pItem->isLocked()){
        if(!ConfigManager::getInstance()->getVideoUnLocked("pack", tag)){
            ShopMiniLayer* pLayer = ShopMiniLayer::create(false);
            this->addChild(pLayer);
            pLayer->setLocalZOrder(100);
            pLayer->shopGetFree = [=](){
                LockUIManager::getInstance()->onRewardSuccess = [=](){
                    AudioHelp::getInstance()->playEffect("basic/start.mp3");
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/goodParticle.plist");
                    pItem->addChild(pParticle,100);
                    pParticle->setPosition(Vec2::ZERO);
                    pItem->unlock();
                };
                RewardManager::getInstance()->showRewardAds("pack", tag);
                pLayer->removeFromParent();
            };
            
            pLayer->shopDismissed = [=](){                
                if (IAPManager::getInstance()->getItemIsBought(2)) {
                    for_each(m_Items.begin(), m_Items.end(), [=](LockItem* pItem){
                        pItem->unlock();
                    });
                }
            };
            return;
        }
    }
    m_bSelected = true;
    AudioHelp::getInstance()->playSelectedEffect();
    if (!UserDefault::getInstance()->getBoolForKey("removeAds")){
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeInterstitialAds);
        
    }
    
    GameDataManager::getInstance()->m_nPackage = tag;
    pItem->runAction(Sequence::create(ActionHelper::getJellyAction(),
                                      CallFunc::create([=](){
        pItem->getContentSprite()->setTexture("content/choose/make"+std::to_string(pItem->getTag())+"_1.png");
    }),
                                      DelayTime::create(1),
                                      CallFunc::create([=](){
        if(onChooseCallback){
            onChooseCallback(pItem->getTag());
        }
    }), NULL));
}
