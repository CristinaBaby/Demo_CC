
#include "EggChooseModelScene.h"
#include "SceneManager.h"

EggChooseModelScene::EggChooseModelScene()
{
    
}

EggChooseModelScene::~EggChooseModelScene()
{
    
}
bool EggChooseModelScene::init()
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
        
        AudioHelp::getInstance()->playEffect("vo_choose_egg_mold.mp3");
        _showShapes();
    }), NULL));
    
    m_pGameUI->showNormalLayout();
    return true;
}

void EggChooseModelScene::onEnter()
{
    ExtensionScene::onEnter();
}

void EggChooseModelScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void EggChooseModelScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
}

void EggChooseModelScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void EggChooseModelScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
}

void EggChooseModelScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void EggChooseModelScene::_initData()
{
    setExPath("content/make/egg/choose/");
}

void EggChooseModelScene::_showShapes()
{
    Sprite* pPlate = Sprite::create(localPath("plate.png"));
    this->addChildToContentLayer(pPlate);
    CMVisibleRect::setPosition(pPlate, 320+visibleSize.width, 420);
    
    for (int i = 0; i<6; i++) {
        LockItem* pItem = LockItem::create();
        bool bLock = false;
        if (!IAPManager::getInstance()->getItemIsBought(2)){
            if (i%2==1 && !GameDataManager::getInstance()->getVideoUnlocked("egg_shape", i)) {
                bLock = true;
                LockUIManager::getInstance()->registerLock("egg_shape", i, pItem);
            }
        }
        pItem->initWithName(localPath(std::to_string(i+1)+".png"), "content/common/video.png",bLock);
        Sprite* pLock = pItem->getLockSprite();
        if (pLock) {
            pLock->setTag(TAG_REWARD_LOCK);
        }
        pItem->setPosition(Vec2(150+160*(i%3), 270-150*(i/3)));
        pItem->onItemClicked = CC_CALLBACK_1(EggChooseModelScene::_onShapeCallback, this);
        pPlate->addChild(pItem);
        m_Items.pushBack(pItem);
        pItem->setTag(i);
    }
    m_bCanTouch = false;
    pPlate->runAction(Sequence::create(DelayTime::create(1),
                                       MoveBy::create(1, Vec2(-visibleSize.width, 0)),
                                       CallFunc::create([=](){
        m_bCanTouch = true;
    }), NULL));
}

void EggChooseModelScene::_onShapeCallback(LockItem* pItem)
{
    if (!m_bCanTouch) {
        return;
    }
    int tag = pItem->getTag();
    if (pItem->isLocked()) {
        if(!ConfigManager::getInstance()->getVideoUnLocked("egg_shape", tag)){
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
                
                RewardManager::getInstance()->showRewardAds("egg_shape", tag);
                pLayer->removeFromParent();
            };
            return;
        }
    }
    GameDataManager::getInstance()->m_nShapeIndex = tag;
    AudioHelp::getInstance()->playEffect("soda_cup.mp3");
    AudioHelp::getInstance()->_playEffectLaugh();
    pItem->runAction(Sequence::create(ActionHelper::getJellyAction(),
                                      CallFunc::create([=](){
        SceneManager::replaceTheScene<EggFryScene>();
    }), NULL));
}

