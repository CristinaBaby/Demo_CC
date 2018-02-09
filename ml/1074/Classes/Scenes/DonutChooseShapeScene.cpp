
#include "DonutChooseShapeScene.h"
#include "SceneManager.h"

DonutChooseShapeScene::DonutChooseShapeScene()
{
    m_bSelected =false;
}

DonutChooseShapeScene::~DonutChooseShapeScene()
{
    
}
bool DonutChooseShapeScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localRootPath("common/bg.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    static Vec2 sPos[] = {
        Vec2(320, 370),
        Vec2(170, 600),
        Vec2(470, 600),
    };
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("mold_in.mp3");
    }),
                                     DelayTime::create(0.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("kids_laughing3.mp3");
    }),
                                     DelayTime::create(0.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_donut_molds.mp3");
    }), NULL));
    for (int i = 0; i<3; i++) {
        LockItem* pItem = LockItem::create();
        bool locked = false;
        std::string lockName = "";
        if (!IAPManager::getInstance()->getItemIsBought(2)){
            if (i>0){
                if (ConfigManager::getInstance()->getVideoUnLocked("donut_shape", i)){
                    lockName = "";
                }else{
                    lockName = "content/common/video.png";
                    locked = true;
                }
            }
        }
        
        pItem->initWithName(localPath("mould/mould"+std::to_string(i+1)+".png"), lockName,locked);
        this->addChildToContentLayer(pItem);
        pItem->setScale(0.3);
        CMVisibleRect::setPositionAdapted(pItem, sPos[i]+Vec2(visibleSize.width, 0));
        
        if (locked) {
            Sprite* pLock = pItem->getLockSprite();
            if (pLock) {
                pLock->setTag(TAG_REWARD_LOCK);
                LockUIManager::getInstance()->registerLock("donut_shape", i, pLock->getParent());
                LockUIManager::getInstance()->setLockPos(Vec2(50, -50));
                pLock->setScale(2);
            }
        }
        pItem->onItemClicked = CC_CALLBACK_1(DonutChooseShapeScene::_onItemCallback, this);
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
                                                               if (i>=2 && !locked) {
                                                                   ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dec.plist");
                                                                   pItem->addChild(pParticle,20);
                                                                   pParticle->setPosition(Vec2::ZERO);
                                                               }
                                                               pItem->setItemTouchEnable();
                                                           }), NULL));
        
        Sprite* pPlate = Sprite::create(localPath("mould/mould"+std::to_string(i+1)+"_shadow.png"));
        pItem->addChild(pPlate,-10);
        pItem->setTag(i);
        m_Items.pushBack(pItem);
    }
    m_pGameUI->showNormalLayout();
    return true;
}

void DonutChooseShapeScene::onEnter()
{
    ExtensionScene::onEnter();
}

void DonutChooseShapeScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void DonutChooseShapeScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
}

void DonutChooseShapeScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void DonutChooseShapeScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
}

void DonutChooseShapeScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void DonutChooseShapeScene::_initData()
{
    setExPath("content/make/donut/make2/");
}

void DonutChooseShapeScene::_onItemCallback(LockItem* pItem)
{
    if(m_bSelected){
        return;
    }
    int tag = pItem->getTag();
    if (pItem->isLocked()) {
        if(!ConfigManager::getInstance()->getVideoUnLocked("donut_shape", tag)){
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
                
                RewardManager::getInstance()->showRewardAds("donut_shape", tag);
                pLayer->removeFromParent();
            };
            return;
        }
    }
    m_bSelected = true;
    
    AudioHelp::getInstance()->playEffect("mold_in.mp3");
    AudioHelp::getInstance()->_playEffectGoodChooice();
    GameDataManager::getInstance()->m_nShapeIndex = tag;
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
        SceneManager::replaceTheScene<DonutPourModelScene>();
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
    AudioHelp::getInstance()->_playEffectNiceWork();
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setLocalZOrder(10);
    pParticle->setPosition(visibleSize*0.5);
}