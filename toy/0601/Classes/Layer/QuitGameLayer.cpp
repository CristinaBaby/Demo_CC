//
//  QuitGameLayer.cpp
//  PaintDress
//
//  Created by maxiang on 7/21/15.
//
//

#include "QuitGameLayer.h"
#include "../Model/AppManager.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

#define RES_BG              "ui09_popup_box.png"
#define RES_BUTTON_YES      "ui09_btn_yes.png"
#define RES_BUTTON_NO       "ui09_btn_no.png"

#define FONT_NAME           "Helvetica_Rounded_LT_Black.ttf"
#define FONT_SZIE           30
#define TEXT_QUIT_GAME      "Are you sure you want\nto exit the game?"


bool QuitGameLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 210))) {
        return false;
    }
    
    bool rcode = false;
    do {
        //bg
        auto bg = Sprite::create(RES_BG);
        CC_BREAK_IF(!bg);
        bg->setPosition(getContentSize()/2);
        addChild(bg);
        
        //yes button
        auto yesButton = Button::create(RES_BUTTON_YES);
        CC_BREAK_IF(!yesButton);
        yesButton->setPosition(Vec2(391, 93));
        yesButton->addTouchEventListener([this](Ref*, Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                AppManager::getInstance()->playButtonEffect();
                
                this->yesAction();
            }
        });
        bg->addChild(yesButton);
        
        //no button
        auto noButton = Button::create(RES_BUTTON_NO);
        CC_BREAK_IF(!noButton);
        noButton->setPosition(Vec2(141, 93));
        noButton->addTouchEventListener([this](Ref*, Widget::TouchEventType type){
            
            if (type == ui::Widget::TouchEventType::ENDED)
            {
                AppManager::getInstance()->playButtonEffect();
                
                this->noAction();
            }
        });
        bg->addChild(noButton);
        
        //text label
        auto textLabel = Label::createWithTTF(TEXT_QUIT_GAME, FONT_NAME, FONT_SZIE);
        CC_BREAK_IF(!textLabel);
        textLabel->setPosition(Vec2(267, 280));
        textLabel->setTextColor(Color4B::WHITE);
        textLabel->setAlignment(TextHAlignment::CENTER);
        bg->addChild(textLabel);
        
        //touch event listen
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(true);
        touchEventListener->onTouchBegan = [](Touch* touch, Event* event) {
            
            return true;
        };
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        
        rcode = true;
    } while (0);
    
    return rcode;
}


void QuitGameLayer::setYesActionCallback(const std::function<void()>& callback)
{
    _yesCallback = callback;
}

void QuitGameLayer::setNoActionCallback(const std::function<void()>& callback)
{
    _noCallback = callback;
}


#pragma mark- Action methods

void QuitGameLayer::yesAction()
{
    if (_yesCallback) {
        _yesCallback();
    }
    
    removeFromParent();
}

void QuitGameLayer::noAction()
{
    if (_noCallback) {
        _noCallback();
    }
    
    removeFromParent();
}

