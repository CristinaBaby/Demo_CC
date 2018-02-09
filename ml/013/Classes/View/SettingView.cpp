//
//  SettingView.cpp
//  Make
//
//  Created by QAMAC01 on 15-4-14.
//
//

#include "SettingView.h"
#include "AudioController.h"
#include "Config.h"
bool SettingView::init()
{
    if(!Layer::init())
    {
        return false;
    }


    auto winSize = Director::getInstance()->getWinSize();
    auto *layerColor = CCLayerColor::create();
    layerColor -> setColor(Color3B(0, 0, 0));
    layerColor -> setOpacity(150);
    layerColor -> setContentSize(Size(9999,9999));
    layerColor -> setPosition(-320, -480);
    addChild(layerColor,0);

    EventListenerTouchOneByOne* lis = EventListenerTouchOneByOne::create();
    lis -> setSwallowTouches(true);
    lis -> onTouchBegan = [](Touch* touch, Event* event)
    {
    
        auto target = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        
        if (rect.containsPoint(locationInNode))
        {
                return true;
        }
        return false;

    };
    
    _eventDispatcher -> addEventListenerWithSceneGraphPriority(lis, this);

    
    this -> initBtn();
    
    
    
    return true;
}

void SettingView::initBtn()
{
    Sprite* bg = Sprite::create("button/setting_bg.png");
    bg -> setPosition(320, 480);
    this -> addChild(bg);
    
    ui::Button* soundBtn = AudioController::getInstance() -> initWithSoundBtn();
    this -> addChild(soundBtn,2);
    soundBtn -> setPosition(Vec2(420, 400));
    
    ui::Button* musicBtn = AudioController::getInstance() -> initWithMusicBtn();
    musicBtn -> setPosition(Vec2(420, 470));
    this -> addChild(musicBtn,2);
    
    
    auto closeBtn = ui::Button::create("button/btn_close.png");
    closeBtn -> setPosition(Vec2(533, 589));
    closeBtn -> addTouchEventListener(CC_CALLBACK_2(SettingView::touchEvent, this));
    addChild(closeBtn, 20, kBtnBack);

}

void SettingView::touchEvent(Ref *obj,ui::Widget::TouchEventType type)
{
    ui::Button *btn = dynamic_cast<ui::Button *>(obj);
    
    
    
    switch (type)
    {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            
            break;
        case cocos2d::ui::Widget::TouchEventType::MOVED:
            
            break;
            
        case ui::Widget::TouchEventType::ENDED:
            if (btn -> getTag() == kBtnBack)
            {
                this -> removeFromParent();
            }
            
            break;
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
            
            break;

    }

}
