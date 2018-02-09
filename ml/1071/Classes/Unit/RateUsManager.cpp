
#include "RateUsManager.h"
#include "cocos2d.h"
#include "GameDataAdapter.h"
#include "Global.h"
#include "extensions/cocos-ext.h"
#include "SSCInternalLibManager.h"

USING_NS_CC;
using namespace cocos2d::ui;


void RateUsManager::showRateUs()
{
    onRateusCallback = nullptr;
    Node* dialogNode = Node::create();
    LayerColor* pLayer = LayerColor::create(Color4B(0,0,0,160));
    dialogNode->addChild(pLayer);
    pLayer->setPosition(Vec2(-visibleSize.width/2, -visibleSize.height/2));
    
    Sprite* bg = Sprite::create("dialog/rateus.png");
    dialogNode->addChild(bg);
    auto pSureBtn = Button::create("dialog/btn_sure.png");
    pSureBtn->setPosition(Vec2(310, 80));
    bg->addChild(pSureBtn);
    pSureBtn->addTouchEventListener([=](Ref* r, Widget::TouchEventType t){
        if(t != Widget::TouchEventType::ENDED)
            return;
        if(onRateusCallback){
            onRateusCallback(1);
        }
        SSCInternalLibManager::getInstance()->rateUs();
        dialogNode->removeFromParent();
    });
    
    auto cancelButton = Button::create("dialog/btn_notnow.png");
    bg->addChild(cancelButton);
    cancelButton->setPosition(Vec2(140, 80));
    cancelButton->addTouchEventListener([=](Ref* r, Widget::TouchEventType t){
        if(t != Widget::TouchEventType::ENDED)
            return;
        
        if(onRateusCallback){
            onRateusCallback(0);
        }
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
}