
#include "RateUsManager.h"
#include "cocos2d.h"
#include "GameDataAdapter.h"
#include "Global.h"
#include "extensions/cocos-ext.h"
#include "SSCInternalLibManager.h"
#include "AudioHelp.h"

USING_NS_CC;
using namespace cocos2d::ui;


void RateUsManager::showRateUs(bool check)
{
    if(UserDefault::getInstance()->getBoolForKey("rated",false) && check){
        if(onRateusCallback){
            onRateusCallback(0);
        }
        return;
    }
    Node* dialogNode = Node::create();
    LayerColor* pLayer = LayerColor::create(Color4B(0,0,0,160));
    dialogNode->addChild(pLayer);
    pLayer->setPosition(Vec2(-visibleSize.width/2, -visibleSize.height/2));
    
    Sprite* bg = Sprite::create("dialog/rate_frame.png");
    dialogNode->addChild(bg);
    
    Sprite* pText = Sprite::create("dialog/rate_text.png");
    dialogNode->addChild(pText);
    
    auto pSureBtn = Button::create("dialog/btn_rate.png");
    pSureBtn->setPosition(Vec2(370, 80));
    bg->addChild(pSureBtn);
    pSureBtn->addTouchEventListener([=](Ref* r, Widget::TouchEventType t){
        if(t != Widget::TouchEventType::ENDED)
            return;
        AudioHelp::getInstance()->playSelectedEffect();
        if(onRateusCallback){
            onRateusCallback(1);
        }
        UserDefault::getInstance()->setBoolForKey("rated", true);
        SSCInternalLibManager::getInstance()->rateUs();
        onRateusCallback = nullptr;
        dialogNode->removeFromParent();
    });
    
    auto cancelButton = Button::create("dialog/btn_why.png");
    bg->addChild(cancelButton);
    cancelButton->setPosition(Vec2(160, 80));
    cancelButton->addTouchEventListener([=](Ref* r, Widget::TouchEventType t){
        if(t != Widget::TouchEventType::ENDED)
            return;
        
        AudioHelp::getInstance()->playSelectedEffect();
        RateUsManager::getInstance()->showRateUsWhy();
        dialogNode->removeFromParent();
    });
    
    
    Director::getInstance()->getRunningScene()->addChild(dialogNode, INT_MAX);
    dialogNode->setPosition(Director::getInstance()->getVisibleOrigin() + Director::getInstance()->getVisibleSize() * 0.5);
    auto touchLs = EventListenerTouchOneByOne::create();
    touchLs->setSwallowTouches(true);
    touchLs->onTouchBegan = [](Touch* t, Event* e)
    {
        return true;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchLs, dialogNode);
    AudioHelp::getInstance()->playEffect("popup_show.mp3");
}


void RateUsManager::showRateUsWhy()
{
    Node* dialogNode = Node::create();
    LayerColor* pLayer = LayerColor::create(Color4B(0,0,0,160));
    dialogNode->addChild(pLayer);
    pLayer->setPosition(Vec2(-visibleSize.width/2, -visibleSize.height/2));
    
    Sprite* bg = Sprite::create("dialog/rate_frame.png");
    dialogNode->addChild(bg);
    
    Sprite* pText = Sprite::create("dialog/text.png");
    dialogNode->addChild(pText);
    
    auto pSureBtn = Button::create("dialog/btn_rate.png");
    pSureBtn->setPosition(Vec2(370, 80));
    bg->addChild(pSureBtn);
    pSureBtn->addTouchEventListener([=](Ref* r, Widget::TouchEventType t){
        if(t != Widget::TouchEventType::ENDED)
            return;
        AudioHelp::getInstance()->playSelectedEffect();
        if(onRateusCallback){
            onRateusCallback(1);
        }
        UserDefault::getInstance()->setBoolForKey("rated", true);
        SSCInternalLibManager::getInstance()->rateUs();
        onRateusCallback = nullptr;
        dialogNode->removeFromParent();
    });
    
    auto cancelButton = Button::create("dialog/btn_notnow.png");
    bg->addChild(cancelButton);
    cancelButton->setPosition(Vec2(160, 80));
    cancelButton->addTouchEventListener([=](Ref* r, Widget::TouchEventType t){
        if(t != Widget::TouchEventType::ENDED)
            return;
        AudioHelp::getInstance()->playSelectedEffect();
        
        if(onRateusCallback){
            onRateusCallback(0);
        }
        onRateusCallback = nullptr;
        dialogNode->removeFromParent();
    });
    
    
    Director::getInstance()->getRunningScene()->addChild(dialogNode, INT_MAX);
    dialogNode->setPosition(Director::getInstance()->getVisibleOrigin() + Director::getInstance()->getVisibleSize() * 0.5);
    auto touchLs = EventListenerTouchOneByOne::create();
    touchLs->setSwallowTouches(true);
    touchLs->onTouchBegan = [](Touch* t, Event* e)
    {
        return true;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchLs, dialogNode);
    AudioHelp::getInstance()->playEffect("popup_show.mp3");
}