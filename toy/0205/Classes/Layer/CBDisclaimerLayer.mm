//
//  CBDisclaimerLayer.cpp
//  ColorBook
//
//  Created by maxiang on 4/30/15.
//
//

#include "CBDisclaimerLayer.h"
#include "ui/cocosGUI.h"
#include "../Model/CBAppManager.h"
#import <UIKit/UIKit.h>

USING_NS_CC;
using namespace ui;


#define RES_BG                  "disclaimer_bg_landscape.png"
#define RES_BUTTON_PLAY         "btn_play_landscape.png"
#define RES_BUTTON_TERMS_USE    "btn_terms_landscape.png"
#define RES_BUTTON_UNLOCK       "btn_unlock_landscape.png"

bool DisclaimerLayer::init()
{
    Color4B color = {47, 173, 144, 255};
    if (!LayerColor::initWithColor(color))
        return false;
    
    do {
        float width  = Director::getInstance()->getWinSize().width;
        float height = Director::getInstance()->getWinSize().height;
        
        auto bgSprite = Sprite::create(RES_BG);
        bgSprite->setPosition(cocos2d::Vec2(width/2, height/2));
        bgSprite->setScale(0.8);
        addChild(bgSprite);
        
        //play
        auto playButton = Button::create(RES_BUTTON_PLAY);
        CC_BREAK_IF(!playButton);
        playButton->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                this->playAction();
            }
        });
        playButton->setPosition(Vec2(268, 95));
        bgSprite->addChild(playButton);
        
        //unlock all
        auto unlockButton = Button::create(RES_BUTTON_UNLOCK);
        CC_BREAK_IF(!unlockButton);
        unlockButton->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                this->unlockAllAction();
            }
        });
        unlockButton->setPosition(Vec2(764, 95));
        bgSprite->addChild(unlockButton);

        //terms of use
        auto termsButton = Button::create(RES_BUTTON_TERMS_USE);
        CC_BREAK_IF(!termsButton);
        termsButton->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                this->termsOfUseAction();
            }
        });
        termsButton->setPosition(Vec2(508, 222));
        bgSprite->addChild(termsButton);

        //touch event listen
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(true);
        touchEventListener->onTouchBegan = [](Touch* touch, Event* event) {
            
            return true;
        };
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        
        return true;
    } while (0);
    
    return false;
}

void DisclaimerLayer::playAction()
{
    removeFromParent();
    
    UserDefault::getInstance()->setBoolForKey(UserDefaultKey_ViewDisclaimer, true);
}

void DisclaimerLayer::unlockAllAction()
{
    //TBD
//    auto scene = PackScene::createScene();
//    Director::getInstance()->replaceScene(scene);
//    
//    auto layer = dynamic_cast<PackScene*>(scene->getChildren().at(0));
//    if (layer)
//    {
//        layer->turnToPage(1);
//    }
    
    UserDefault::getInstance()->setBoolForKey(UserDefaultKey_ViewDisclaimer, true);
}

void DisclaimerLayer::termsOfUseAction()
{
    ParentalGateLayer *gate = ParentalGateLayer::create();
    gate->setDelegate(this, 1);
    //gate->showInLayer(this);
}

void DisclaimerLayer::parentalGatePass(int tag)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <UIKit/UIKit.h>
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"http://www.toymakermedia.com/terms.html"]];
#endif
//    
//    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
//    removeFromParent();
}

