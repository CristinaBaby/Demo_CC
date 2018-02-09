//
//  CBDownloadFaildLayer.cpp
//  ColorBook
//
//  Created by maxiang on 5/11/15.
//
//

#include "CBDownloadFaildLayer.h"
#include "../Model/CBUtils.h"
#include "../Model/CBAppManager.h"

USING_NS_CC;
CB_BEGIN_NAMESPACE

#define RES_BG                  "ui05_download_box.png"

#define TEXT_DOWNLOAD_FAILED    "Download failed,\ntry again?"
#define TEXT_FONT_NAME          "Helvetica_Rounded_LT_Black.ttf"
#define TEXT_SIZE               30

#define RES_BUTTON_NO           "ui05_btn_no.png"
#define RES_BUTTON_NO_H         "ui05_btn_no_h.png"

#define RES_BUTTON_YES          "ui05_btn_yes.png"
#define RES_BUTTON_YES_H        "ui05_btn_yes_h.png"

bool DownloadFaildLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 200))) {
        return false;
    }
    
    bool rcode = false;
    do {
        
        //background
        Sprite *background = Sprite::create(RES_BG);
        background->setPosition(Utils::getInstance()->getScreenCenter());
        addChild(background);
        
        //text
        _textLabel = Label::createWithTTF(TEXT_DOWNLOAD_FAILED, TEXT_FONT_NAME, TEXT_SIZE);
        CC_BREAK_IF(!_textLabel);
        _textLabel->setPosition(Vec2(250, 230));
        _textLabel->setTextColor(Color4B(152, 52, 0, 255));
        _textLabel->setAlignment(TextHAlignment::CENTER);
        background->addChild(_textLabel);
        
        //save picture bg
        auto no = Sprite::create(RES_BUTTON_NO);
        CC_BREAK_IF(!no);
        no->setPosition(Vec2(170, 80));
        background->addChild(no);
        
        auto yes = Sprite::create(RES_BUTTON_YES);
        CC_BREAK_IF(!yes);
        yes->setPosition(Vec2(340, 80));
        background->addChild(yes);
        
        //touch event listen
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(true);
        touchEventListener->onTouchBegan = [no, yes, background](Touch* touch, Event* event) {
            
            Point point = background->convertToNodeSpace(touch->getLocation());
            
            if (no->getBoundingBox().containsPoint(point))
            {
                no->setTexture(RES_BUTTON_NO_H);
            }
            else if(yes->getBoundingBox().containsPoint(point))
            {
                yes->setTexture(RES_BUTTON_YES_H);
            }
        
            return true;
        };
        
        touchEventListener->onTouchMoved = [no, yes, background](Touch* touch, Event* event) {
            
            Point point = background->convertToNodeSpace(touch->getLocation());
            
            no->setTexture(RES_BUTTON_NO);
            yes->setTexture(RES_BUTTON_YES);

            if (no->getBoundingBox().containsPoint(point))
            {
                no->setTexture(RES_BUTTON_NO_H);
            }
            else if(yes->getBoundingBox().containsPoint(point))
            {
                yes->setTexture(RES_BUTTON_YES_H);
            }
        };
        
        touchEventListener->onTouchEnded = [no, yes, background, this](Touch* touch, Event* event) {
            
            Point point = background->convertToNodeSpace(touch->getLocation());

            if (no->getBoundingBox().containsPoint(point))
            {
                no->setTexture(RES_BUTTON_NO);
                noAction();
            }
            else if(yes->getBoundingBox().containsPoint(point))
            {
                yes->setTexture(RES_BUTTON_YES);
                yesAction();
            }
        };
        
        touchEventListener->onTouchCancelled = [no, yes](Touch* touch, Event* event) {
            
            no->setTexture(RES_BUTTON_NO);
            yes->setTexture(RES_BUTTON_YES);
        };
        
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        
        AppManager::getInstance()->showBannerAd();
        
        rcode = true;
    } while (0);
    
    return rcode;
}

void DownloadFaildLayer::setYesActionCallback(const std::function<void()>& callback)
{
    _yesCallback = callback;
}

void DownloadFaildLayer::setNoActionCallback(const std::function<void()>& callback)
{
    _noCallback = callback;
}

#pragma mark- Action methods

void DownloadFaildLayer::yesAction()
{
    if (_yesCallback) {
        _yesCallback();
    }
    
    removeFromParent();
}

void DownloadFaildLayer::noAction()
{
    if (_noCallback) {
        _noCallback();
    }
    
    removeFromParent();
}


CB_END_NAMESPACE