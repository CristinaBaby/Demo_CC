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
#define TEXT_FONT_NAME          "HELVETICA_NEUE.ttf"
#define TEXT_FONT_SIZE          32

#define RES_BG                  "ui05_save_box.png"
#define RES_CONTINE_BUTTON      "ui05_btn_continue.png"
#define RES_RESET_BUTTON        "ui05_btn_restart.png"

#define OFFSET  (80)

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
        label->setPosition(Vec2(background->getContentSize().width/2, 250));
        label->setTextColor(Color4B(152, 52, 0, 255));
        background->addChild(label);
        
        //continue button
        auto continueButton = ui::Button::create(RES_CONTINE_BUTTON);
        CC_BREAK_IF(!continueButton);
        continueButton->setTag(contineTag);
        continueButton->setPosition(Vec2(260+OFFSET, 80));
        background->addChild(continueButton);
        
        //reset button
        auto resetButton = ui::Button::create(RES_RESET_BUTTON);
        CC_BREAK_IF(!resetButton);
        resetButton->setTag(resetTag);
        resetButton->setPosition(Vec2(80+OFFSET, 80));
        background->addChild(resetButton);

        //touch event listen
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(true);
        touchEventListener->onTouchBegan = [this](Touch* touch, Event* event) {
            return true;
        };
        
        continueButton->addTouchEventListener(CC_CALLBACK_2(ContinueLayer::touchDown, this));
        resetButton->addTouchEventListener(CC_CALLBACK_2(ContinueLayer::touchDown, this));
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        
        AppManager::getInstance()->showBannerAd();

        rcode = true;
    } while (0);
    
    return rcode;
}

void ContinueLayer::touchDown(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type){
    if(ui::Widget::TouchEventType::ENDED != type) return;
    
    auto widget = (Widget*)pSender;
    int tag = widget->getTag();
    
    if (tag == contineTag) {
        continueAction();
    }
    else{
        resetAction();
    }
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