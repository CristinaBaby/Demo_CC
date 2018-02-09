
#include "ChooseLayer.h"
#include "CMVisibleRect.h"
#include "Global.h"
#include "AudioHelp.h"
#include "IAPManager.h"
#include "RewardManager.h"
#include "LockUIManager.h"
#include "ConfigManager.h"

ChooseLayer::ChooseLayer()
{
    onItemCallback = nullptr;
    m_bSelected =false;
}

ChooseLayer::~ChooseLayer()
{
    
}

bool ChooseLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    LayerColor* pColor = LayerColor::create(Color4B(0, 0, 0, 160));
    this->addChild(pColor);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch* touch,Event*){
        return true;
    };
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void ChooseLayer::showChooseBlender()
{
    AudioHelp::getInstance()->playEffect("pop_up.mp3");
    for (int i = 0; i<2; i++) {
        LockItem* item = LockItem::create();
        stringstream ostr;
        ostr<<"content/common/tip/btn_blender"<<i<<".png";
        
        bool locked = false;
        std::string lockName = "";
        if (!IAPManager::getInstance()->getItemIsBought(1)){
            if (i==1){
                if (ConfigManager::getInstance()->getVideoUnLocked("blender", i)){
                    lockName = "";
                }else{
                    lockName = "content/common/video.png";
                    locked = true;
                }
            }
        }
        
        item->initWithName(ostr.str(), lockName,locked);
        this->addChild(item);
        item->setScale(1.2);
        if (locked) {
            Sprite* pLock = item->getLockSprite();
            if (pLock) {
                pLock->setTag(TAG_REWARD_LOCK);
                LockUIManager::getInstance()->registerLock("blender", i, pLock->getParent());
                LockUIManager::getInstance()->setLockPos(Vec2(50, -50));
            }
        }
        item->onItemClicked = CC_CALLBACK_1(ChooseLayer::_onItemCallback, this);
        CMVisibleRect::setPosition(item, 200+240*i+visibleSize.width, 480);
        item->setItemTouchEnable(false);
        item->runAction(Sequence::create(DelayTime::create(0.5+0.3*i),
                                         EaseBounceOut::create(MoveBy::create(1, Vec2(-visibleSize.width, 0))),
                                         CallFunc::create([=]()
                                                          {
                                                              item->setItemTouchEnable();
                                                          }), NULL));
        item->setTag(i);
    }
}

void ChooseLayer::_onItemCallback(LockItem* item)
{
    if (m_bSelected) {
        return;
    }
    int tag = item->getTag();
    if (item->getChildByTag(TAG_REWARD_LOCK))  {
        LockUIManager::getInstance()->onRewardSuccess = [=](){
            AudioHelp::getInstance()->playEffect("basic/start.mp3");
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/goodParticle.plist");
            item->addChild(pParticle,100);
            pParticle->setPosition(Vec2::ZERO);
        };
        RewardManager::getInstance()->showRewardAds("blender", tag);
        return;
    }
    AudioHelp::getInstance()->playSelectedEffect();
    m_bSelected = true;
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/dec.plist");
    item->addChild(pParticle);
    pParticle->setPosition(Vec2::ZERO);
    this->runAction(Sequence::create(DelayTime::create(0.8),
                                     CallFunc::create([=]()
                                                      {
                                                          if (onItemCallback) {
                                                              onItemCallback(item->getTag());
                                                          }
                                                          this->removeFromParent();
                                                      }), NULL));
}