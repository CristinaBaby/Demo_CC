
#include "ClassicChooseScene.h"
#include "SceneManager.h"

ClassicChooseScene::ClassicChooseScene()
{
    m_bSelected = false;
}

ClassicChooseScene::~ClassicChooseScene()
{
    
}
bool ClassicChooseScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localPath("bg1.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    for(int i = 0;i<5;i++){
        LockItem* pItem = LockItem::create();
        bool locked = false;
        std::string lockName = "";
        if (!IAPManager::getInstance()->getItemIsBought(2)){
            if (i>=2){
                if (ConfigManager::getInstance()->getVideoUnLocked("classic_flavor", i)){
                    lockName = "";
                }else{
                    lockName = "content/common/video.png";
                    locked = true;
                }
            }
        }
        
        pItem->initWithName(localPath("cookie"+std::to_string(i)+".png"), lockName,locked);
        this->addChildToContentLayer(pItem);
        pItem->setScale(0.3);
        CMVisibleRect::setPositionAdapted(pItem, 130+338*(i%2), 690-355*(i/2));
        if(i == 4){
            CMVisibleRect::setPositionAdapted(pItem, 130+338/2, 690-355/2);
        }
        if (locked) {
            Sprite* pLock = pItem->getLockSprite();
            if (pLock) {
                pLock->setTag(TAG_REWARD_LOCK);
                LockUIManager::getInstance()->registerLock("classic_flavor", i, pLock->getParent());
                LockUIManager::getInstance()->setLockPos(Vec2(50, -50));
            }
        }
        pItem->onItemClicked = CC_CALLBACK_1(ClassicChooseScene::_onItemCallback, this);
        pItem->setItemTouchEnable(false);
        pItem->runAction(Sequence::create(DelayTime::create(0.5),
                                          EaseElasticInOut::create(ScaleTo::create(1, 1)),
                                          ActionHelper::getJellyAction(),
                                         CallFunc::create([=]()
                                                          {
                                                              if (i>=2 && !locked) {
                                                                  ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dec.plist");
                                                                  pItem->addChild(pParticle,20);
                                                                  pParticle->setPosition(Vec2::ZERO);
                                                              }
                                                              pItem->setItemTouchEnable();
                                                          }), NULL));
        
        Sprite* pPlate = Sprite::create(localPath("plate"+std::to_string(i)+".png"));
        pItem->addChild(pPlate,-10);
        pItem->setTag(i);
        m_Items.pushBack(pItem);
    }
    AudioHelp::getInstance()->playEffect("vo_choose_flavor.mp3");
    m_pGameUI->showNormalLayout();
    return true;
}

void ClassicChooseScene::onEnter()
{
    ExtensionScene::onEnter();
    Analytics::getInstance()->sendScreenEvent(Flurry_EVENT_MAKE_CLASSIC);
}

void ClassicChooseScene::onShopItemBuy(cocos2d::Ref *pRef)
{
    if (IAPManager::getInstance()->getItemIsBought(2)) {
        for_each(m_Items.begin(), m_Items.end(), [=](LockItem* pItem){
            pItem->unlock();
        });
    }
}

void ClassicChooseScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
}

void ClassicChooseScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void ClassicChooseScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
}

void ClassicChooseScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void ClassicChooseScene::_initData()
{
    setExPath("content/make/classic/choose/");
}


void ClassicChooseScene::_onItemCallback(LockItem* pItem)
{
    if(m_bSelected){
        return;
    }
    int tag = pItem->getTag();
    if (pItem->isLocked()) {
        if(!ConfigManager::getInstance()->getVideoUnLocked("classic_flavor", tag)){
            ShopMiniLayer* pLayer = ShopMiniLayer::create(false);
            this->addChildToUILayer(pLayer);
            pLayer->setLocalZOrder(100);
            
            pLayer->shopGetFree = [=](){
                LockUIManager::getInstance()->onRewardSuccess = [=](){
                    AudioHelp::getInstance()->playEffect("basic/start.mp3");
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/goodParticle.plist");
                    pItem->addChild(pParticle,100);
                    pParticle->setPosition(Vec2::ZERO);
                    pItem->unlock();
                };
                
                RewardManager::getInstance()->showRewardAds("classic_flavor", tag);
                pLayer->removeFromParent();
            };
            return;
        }
    }
    m_bSelected = true;
    
    AudioHelp::getInstance()->playSelectedEffect();
    AudioHelp::getInstance()->playBackGroundMusic("music_home.mp3");
    GameDataManager::getInstance()->m_nFlavorIndex = tag;
    pItem->runAction(Sequence::create(ActionHelper::getJellyAction(),
                                      DelayTime::create(1),
                                      MoveTo::create(1,Vec2(visibleSize.width/2,visibleSize.height/2)),
                                      CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("all_star.mp3");
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/win.plist");
        pItem->addChild(pParticle);
        pParticle->setPosition(Vec2::ZERO);
    }),
                                      DelayTime::create(2),
                                      CallFunc::create([=](){
        SceneManager::replaceTheScene<ClassicMixScene>();
    }), NULL));
    for(auto pFlavor:m_Items){
        if (pFlavor!=pItem) {
            if (pFlavor->getPositionX()<visibleSize.width/2) {
                pFlavor->runAction(EaseElasticIn::create(MoveBy::create(1, Vec2(-visibleSize.width, 0)), 0.6));
            }else{
                pFlavor->runAction(EaseElasticIn::create(MoveBy::create(1, Vec2(visibleSize.width, 0)), 0.6));
            }
        }
    }
}