
#include "ChooseModelLayer.h"
#include "CMVisibleRect.h"
#include "Global.h"
#include "AudioHelp.h"
//#include "IAPManager.h"
#include "RewardManager.h"
#include "LockUIManager.h"
#include "ConfigManager.h"
#include "ActionHelper.h"
#include "ShopMiniLayer.h"

ChooseModelLayer::ChooseModelLayer()
{
    onItemCallback = nullptr;
    m_bSelected =false;
}

ChooseModelLayer::~ChooseModelLayer()
{
    
}

bool ChooseModelLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    LayerColor* pColor = LayerColor::create(Color4B(0, 0, 0, 160));
    this->addChild(pColor);
    
    m_pRoot = Node::create();
    this->addChild(m_pRoot);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch* touch,Event*){
        return true;
    };
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void ChooseModelLayer::showChooseModel()
{
    static std::string gsModels[] = {
        "round",
        "heart",
        "cat",
        "star",
        "ginger",
        "tree",
    };
    Sprite* pBanner = Sprite::create("content/make/classic/make3/menu/choose_shape.png");
    m_pRoot->addChild(pBanner);
    CMVisibleRect::setPosition(pBanner, 320, 480);
    m_pRoot->setPosition(Vec2(-500, -visibleSize.height/2));
    m_pRoot->setScale(0.3);
    
    AudioHelp::getInstance()->playEffect("pop_up.mp3");
    
    m_pRoot->runAction(Sequence::create(Spawn::create(MoveTo::create(2, Vec2::ZERO),
                                                      ScaleTo::create(2.5, 1.1),
                                                      RotateBy::create(2, 360), NULL),
                                        ActionHelper::getJellyAction(), NULL));
    for (int i = 0; i<6; i++) {
        LockItem* item = LockItem::create();
        stringstream ostr;
        ostr<<"content/make/classic/make3/menu/shape_"<<gsModels[i]<<".png";
        
        bool locked = false;
        std::string lockName = "";
        if (!IAPManager::getInstance()->getItemIsBought(2)){
            if (i%2==1){
                if (ConfigManager::getInstance()->getVideoUnLocked("classic_shape", i)){
                    lockName = "";
                }else{
                    lockName = "content/common/video.png";
                    locked = true;
                }
            }
        }
        
        item->initWithName(ostr.str(), lockName,locked);
        m_pRoot->addChild(item);
        item->setScale(0.5);
        CMVisibleRect::setPositionAdapted(item, 190+224*(i%2), 720-220*(i/2));
        if (locked) {
            Sprite* pLock = item->getLockSprite();
            if (pLock) {
                pLock->setTag(TAG_REWARD_LOCK);
                LockUIManager::getInstance()->registerLock("classic_shape", i, pLock->getParent());
                LockUIManager::getInstance()->setLockPos(Vec2(50, -50));
            }
        }
        item->onItemClicked = CC_CALLBACK_1(ChooseModelLayer::_onItemCallback, this);
        item->setItemTouchEnable(false);
        item->runAction(Sequence::create(DelayTime::create(0.5+0.3*i),
//                                         EaseElasticInOut::create(ScaleTo::create(0.5, 1)),
                                         ActionHelper::getJellyAction(),
                                         CallFunc::create([=]()
                                                          {
                                                              if (i%2 == 1 && !locked) {
                                                                  ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dec.plist");
                                                                  item->addChild(pParticle,20);
                                                                  pParticle->setPosition(Vec2::ZERO);
                                                              }
                                                              item->setItemTouchEnable();
                                                          }), NULL));
        item->setTag(i);
        m_Items.pushBack(item);
    }
}

void ChooseModelLayer::_onItemCallback(LockItem* item)
{
    if (m_bSelected) {
        return;
    }
    int tag = item->getTag();
    if (item->isLocked())  {
        if(!ConfigManager::getInstance()->getVideoUnLocked("classic_shape", tag)){
            ShopMiniLayer* pLayer = ShopMiniLayer::create(false);
            this->addChild(pLayer);
            pLayer->setLocalZOrder(100);
            
            pLayer->shopGetFree = [=](){
                LockUIManager::getInstance()->onRewardSuccess = [=](){
                    AudioHelp::getInstance()->playEffect("basic/start.mp3");
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/goodParticle.plist");
                    item->addChild(pParticle,100);
                    pParticle->setPosition(Vec2::ZERO);
                    item->unlock();
                };
                
                RewardManager::getInstance()->showRewardAds("classic_shape", tag);
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
    
    AudioHelp::getInstance()->playSelectedEffect();
    m_bSelected = true;
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dec.plist");
    item->addChild(pParticle);
    pParticle->setPosition(Vec2::ZERO);
    
    Sprite* pOk = Sprite::create("content/make/classic/make3/menu/btn_tick.png");
    item->addChild(pOk);
    
    m_pRoot->runAction(Sequence::create(ActionHelper::getJellyAction(),
                                        Spawn::create(MoveBy::create(0.5, Vec2(-visibleSize.width, -visibleSize.height/2)),
                                                      ScaleTo::create(0.5, 0.3), NULL),
                                        CallFunc::create([=]()
                                                         {
                                                             if (onItemCallback) {
                                                                 onItemCallback(item->getTag());
                                                             }
                                                             this->removeFromParent();
                                                         }), NULL));
}