//
//  CBContinueLayer.cpp
//  ColorBook
//
//  Created by maxiang on 5/7/15.
//
//

#include "CBContinueLayer.h"
#include <stdio.h>
#include "../Model/CBUtils.h"
#include "../Model/CBAppManager.h"
#include "../Scene/CBGameScene.h"
#include "../Model/CBPackManager.h"

USING_NS_CC;
CB_BEGIN_NAMESPACE

#define TEXT_CONTINE            "You've already paint a bit on\nthis picture, do you wish to\ncontinue your previous work?\nOr start from beginning?"
#define TEXT_FONT_NAME          "Helvetica_Rounded_LT_Black.ttf"
#define TEXT_FONT_SIZE          25

#define RES_BG                  "ui05_popup_box.png"

#define RES_CONTINE_BUTTON      "ui05_popup_continue.png"
#define RES_CONTINE_BUTTON_H    "ui05_popup_continue_h.png"

#define RES_RESET_BUTTON        "ui05_popup_reset.png"
#define RES_RESET_BUTTON_H      "ui05_popup_reset_h.png"

ContinueLayer* ContinueLayer::create(const int packIndex, const int pictureIndex)
{
    ContinueLayer *layer = new (std::nothrow)(ContinueLayer);
    if (layer && layer->init(packIndex, pictureIndex))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool ContinueLayer::init(const int packIndex, const int pictureIndex)
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 200))) {
        return false;
    }
    
    bool rcode = false;
    do {
        
        _packIndex = packIndex;
        _pictureIndex = pictureIndex;
        
        //background
        Sprite *background = Sprite::create(RES_BG);
        background->setPosition(Utils::getInstance()->getScreenCenter());
        addChild(background);
        
        //text
        auto label = Label::createWithTTF(TEXT_CONTINE, TEXT_FONT_NAME, TEXT_FONT_SIZE);
        CC_BREAK_IF(!label);
        label->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
        label->setPosition(Vec2(95, 290));
        label->setTextColor(Color4B(152, 52, 0, 255));
        background->addChild(label);
        
        //continue button
        auto continueButton = Sprite::create(RES_CONTINE_BUTTON);
        CC_BREAK_IF(!continueButton);
        continueButton->setPosition(Vec2(396, 80));
        background->addChild(continueButton);
        
        //reset button
        auto resetButton = Sprite::create(RES_RESET_BUTTON);
        CC_BREAK_IF(!resetButton);
        resetButton->setPosition(Vec2(190, 80));
        background->addChild(resetButton);

        //touch event listen
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(true);
        touchEventListener->onTouchBegan = [background, continueButton, resetButton, this](Touch* touch, Event* event) {
            
            Point point = background->convertToNodeSpace(touch->getLocation());
            if (continueButton->getBoundingBox().containsPoint(point))
            {
                continueButton->setTexture(RES_CONTINE_BUTTON_H);
            }
            else if (resetButton->getBoundingBox().containsPoint(point))
            {
                resetButton->setTexture(RES_RESET_BUTTON_H);
            }
            
            return true;
        };
        
        touchEventListener->onTouchMoved = [background, continueButton, resetButton, this](Touch* touch, Event* event) {
            
            Point point = background->convertToNodeSpace(touch->getLocation());
            
            continueButton->setTexture(RES_CONTINE_BUTTON);
            resetButton->setTexture(RES_RESET_BUTTON);

            if (continueButton->getBoundingBox().containsPoint(point))
            {
                continueButton->setTexture(RES_CONTINE_BUTTON_H);
            }
            else if (resetButton->getBoundingBox().containsPoint(point))
            {
                resetButton->setTexture(RES_RESET_BUTTON_H);
            }
        };
        
        touchEventListener->onTouchEnded = [background, continueButton, resetButton, this](Touch* touch, Event* event) {
            
            Point point = background->convertToNodeSpace(touch->getLocation());
            if (continueButton->getBoundingBox().containsPoint(point))
            {
                continueButton->setTexture(RES_CONTINE_BUTTON);
                continueAction();
            }
            else if (resetButton->getBoundingBox().containsPoint(point))
            {
                resetButton->setTexture(RES_RESET_BUTTON);
                resetAction();
            }
        };
        
        touchEventListener->onTouchCancelled = [background, continueButton, resetButton, this](Touch* touch, Event* event) {
            
            continueButton->setTexture(RES_CONTINE_BUTTON);
            resetButton->setTexture(RES_RESET_BUTTON);
        };
        
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        
        AppManager::getInstance()->showBannerAd();

        rcode = true;
    } while (0);
    
    return rcode;
}

void ContinueLayer::continueAction()
{
    auto scene = GameScene::createScene(_packIndex, _pictureIndex);
    
    Director::getInstance()->replaceScene(scene);
}

void ContinueLayer::resetAction()
{
    std::string historyPath = PackManager::getInstance()->getPackHistoryPicturePath(_packIndex+1, _pictureIndex, true);
    bool isExsit = FileUtils::getInstance()->isFileExist(historyPath);
    if (isExsit) {
        remove(historyPath.c_str());
    }
    
    auto scene = GameScene::createScene(_packIndex, _pictureIndex);
    Director::getInstance()->replaceScene(scene);
}

CB_END_NAMESPACE