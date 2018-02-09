
#include "JuiceChooseFlavorScene.h"
#include "SceneManager.h"

JuiceChooseFlavorScene::JuiceChooseFlavorScene()
{
    m_bSelected =false;
    
}

JuiceChooseFlavorScene::~JuiceChooseFlavorScene()
{
    
}
bool JuiceChooseFlavorScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create("content/make/common/bg1.jpg");
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("present_in.mp3");
        AudioHelp::getInstance()->playEffect("vo_choose_juice.mp3");
    }), NULL));
    
    _showFlavors();
    m_pGameUI->showNormalLayout();
    return true;
}

void JuiceChooseFlavorScene::onEnter()
{
    ExtensionScene::onEnter();
}

void JuiceChooseFlavorScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void JuiceChooseFlavorScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
}

void JuiceChooseFlavorScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void JuiceChooseFlavorScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
}

void JuiceChooseFlavorScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void JuiceChooseFlavorScene::_initData()
{
    setExPath("content/make/juice/");
}

void JuiceChooseFlavorScene::_showFlavors()
{
    int flavorCount = 8;
    for (int i = 0; i<flavorCount; i++) {
        LockItem* pItem = LockItem::create();
        bool locked = false;
        std::string lockName = "";
        if (!IAPManager::getInstance()->getItemIsBought(2)){
            if (i%2==1){
                if (ConfigManager::getInstance()->getVideoUnLocked("juice_flavor", i)){
                    lockName = "";
                }else{
                    lockName = "content/common/video.png";
                    locked = true;
                }
            }
        }
        
        pItem->initWithName(localPath("icon_juice"+std::to_string(i)+".png"), lockName,locked);
        this->addChildToContentLayer(pItem);
        pItem->setScale(0.3);
        CMVisibleRect::setPositionAdapted(pItem, Vec2(128+135*(i%4)+visibleSize.width, 530-210*(i/4)));
        
        if (locked) {
            Sprite* pLock = pItem->getLockSprite();
            if (pLock) {
                pLock->setTag(TAG_REWARD_LOCK);
                LockUIManager::getInstance()->registerLock("juice_flavor", i, pLock->getParent());
                LockUIManager::getInstance()->setLockPos(Vec2(50, -50));
                pLock->setScale(2);
            }
        }
        pItem->onItemClicked = CC_CALLBACK_1(JuiceChooseFlavorScene::_onItemCallback, this);
        pItem->setItemTouchEnable(false);
        pItem->runAction(Sequence::create(DelayTime::create(0.5),
                                          Spawn::create(JumpBy::create(1, Vec2(-visibleSize.width, 0), 200, 1),
                                                        ScaleTo::create(1, 0.7), NULL),
                                          ScaleTo::create(0.13, 0.7,0.62),
                                          ScaleTo::create(0.11, 0.62, 0.7),
                                          ScaleTo::create(0.10, 0.7, 0.66),
                                          ScaleTo::create(0.09, 0.68, 0.7),
                                          ScaleTo::create(0.08, 0.7, 0.69),
                                          ScaleTo::create(0.07, 0.69, 0.7),
                                          ScaleTo::create(0.07, 0.7),
                                          CallFunc::create([=]()
                                                           {
                                                               if (i%2!=0 && !locked) {
                                                                   ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dec.plist");
                                                                   pItem->addChild(pParticle,20);
                                                                   pParticle->setPosition(Vec2::ZERO);
                                                               }
                                                               pItem->setItemTouchEnable();
                                                           }), NULL));
        
        pItem->setTag(i);
        m_Items.pushBack(pItem);
    }
}
void JuiceChooseFlavorScene::_onItemCallback(LockItem* pItem)
{
    
    if(m_bSelected){
        return;
    }
    int tag = pItem->getTag();
    if (pItem->isLocked()) {
        if(!ConfigManager::getInstance()->getVideoUnLocked("juice_flavor", tag)){
            ShopMiniLayer* pLayer = ShopMiniLayer::create(false);
            this->addChildToUILayer(pLayer);
            pLayer->setLocalZOrder(100);
            
            pLayer->shopDismissed = [=](){
                if (IAPManager::getInstance()->getItemIsBought(2)) {
                    for_each(m_Items.begin(), m_Items.end(), [=](LockItem* pItem){
                        pItem->unlock();
                    });
                }
            };
            pLayer->shopGetFree = [=](){
                LockUIManager::getInstance()->onRewardSuccess = [=](){
                    AudioHelp::getInstance()->playEffect("bling.mp3");
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/goodParticle.plist");
                    pItem->addChild(pParticle,100);
                    pParticle->setPosition(Vec2::ZERO);
                    pItem->unlock();
                };
                
                RewardManager::getInstance()->showRewardAds("juice_flavor", tag);
                pLayer->removeFromParent();
            };
            return;
        }
    }
    m_bSelected = true;
    AudioHelp::getInstance()->playEffect("click_button.mp3");
//    AudioHelp::getInstance()->_playEffectGoodChooice();
    GameDataManager::getInstance()->m_nFlavorIndex = tag;
    pItem->setLocalZOrder(10);
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
        SceneManager::replaceTheScene<JuiceCutFruitScene>();
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
    AudioHelp::getInstance()->playEffect("done.mp3");
    AudioHelp::getInstance()->_playEffectLaugh();
    AudioHelp::getInstance()->playEffect("magic_effect.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
}