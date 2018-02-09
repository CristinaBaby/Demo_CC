//
//  CBSavingLayer.cpp
//  ColorBook
//
//  Created by maxiang on 4/28/15.
//
//

#include "CBSavingLayer.h"
#include "../Model/CBUtils.h"
#include "../Model/CBAppManager.h"

USING_NS_CC;
CB_BEGIN_NAMESPACE

SavingLayer::~SavingLayer()
{
    Director::getInstance()->getTextureCache()->removeTextureForKey("ui05_saving_box.png");
    Director::getInstance()->getTextureCache()->removeTextureForKey("ui05_saving_progress.png");
}

bool SavingLayer::init()
{
    if (! LayerColor::initWithColor(Color4B(0, 0, 0, 200)))
    {
        return false;
    }
    
    bool rcode = false;
    do {
        //background
        auto bgSprite = Sprite::create("ui05_saving_box.png");
        CC_BREAK_IF(!bgSprite);
        bgSprite->setPosition(Vec2(Utils::getInstance()->getScreenCenter()));
        addChild(bgSprite);
        
        //text
        auto label = Label::createWithTTF("Saving...", "Helvetica_Rounded_LT_Black.ttf", 40);
        CC_BREAK_IF(!label);
        label->setPosition(Vec2(bgSprite->getContentSize().width/2 + 60, 160));
        label->setTextColor(Color4B(152, 52, 0, 255));
        bgSprite->addChild(label);
        
        //init progress background
        Sprite *progressbg = Sprite::create("ui05_saving_progress.png");
        CC_BREAK_IF(!progressbg);
        progressbg->setPosition(Vec2(label->getPosition().x , 100));
        bgSprite->addChild(progressbg);
        
//        //init progress
//        Sprite *sprite = Sprite::create("ui06_loading-sroll.png");
//        auto progress = ProgressTimer::create(sprite);
//        CC_BREAK_IF(!progress);
//        progress->setPosition(progressbg->getPosition());
//        progress->setType(ProgressTimer::Type::BAR);
//        progress->setMidpoint(Vec2(0, 1));
//        progress->setBarChangeRate(Vec2(1, 0));
//        progress->setPercentage(0);
//        addChild(progress);
        
        
        //touch events
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(true);
        touchEventListener->onTouchBegan = CC_CALLBACK_2(SavingLayer::onTouchBegan, this);
        touchEventListener->onTouchMoved = CC_CALLBACK_2(SavingLayer::onTouchMoved, this);
        touchEventListener->onTouchEnded = CC_CALLBACK_2(SavingLayer::onTouchEnded, this);
        touchEventListener->onTouchCancelled = CC_CALLBACK_2(SavingLayer::onTouchCancelled, this);
        
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        
        //fake a saving process
        scheduleOnce(schedule_selector(SavingLayer::finishSave), 2);
        
        AppManager::getInstance()->showBannerAd();

//        //progress
//        ProgressTo *progressTo = ProgressTo::create(1, 100);
//        progress->runAction(Sequence::create(progressTo, CallFunc::create(CC_CALLBACK_0(SavingLayer::finishSave, this)), NULL));
        
        rcode = true;
    } while (0);
    
    return rcode;
}


#pragma mark- Action methods

void SavingLayer::setFinishCallback(const std::function<void()>& callback)
{
    _finishCallback = callback;
}

void SavingLayer::finishSave(float time)
{
    if (_finishCallback) {
        _finishCallback();
    }
    
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
    removeFromParentAndCleanup(true);
}

#pragma mark- Touch events

bool SavingLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
    return true;
}

void SavingLayer::onTouchMoved(Touch *touch, Event *unused_event)
{

}

void SavingLayer::onTouchEnded(Touch *touch, Event *unused_event)
{

}

void SavingLayer::onTouchCancelled(Touch *touch, Event *unused_event)
{
    
}



CB_END_NAMESPACE