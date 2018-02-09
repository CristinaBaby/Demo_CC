//
//  LoadingLayer.cpp
//  ColorBook0818
//
//  Created by huangwen on 15/9/14.
//
// 

#include "LoadingLayer.h"
#include "UICpp.h"
#include "ArmatureManager.h"
#include "TalkManager.hpp"
#include "PageViewEx.hpp"


#define DELAY_TIME      (0.5)

LoadingLayer* LoadingLayer::create(){
    LoadingLayer* loadLayer = new (std::nothrow) LoadingLayer();
    if (loadLayer && loadLayer->init())
    {
        loadLayer->autorelease();
        return loadLayer;
    }
    CC_SAFE_DELETE(loadLayer);
    return nullptr;
}

bool LoadingLayer::init(){
    if (Layer::init()) {
        _root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("loading.json"));
        addChild(_root);
        _root->cocos2d::Node::visit();
        auto popup = (ImageView*)Helper::seekWidgetByName(_root, "center");
        
        
        _dot = Sprite::create("loading/page_number_1.png");
        _dot->setPosition(popup->getPosition() - Vec2(20,popup->getContentSize().height/4 + 40));
        addChild(_dot,10);
        
        auto pageView = PageViewEx::create();
        popup->addChild(pageView);
        pageView->setPosition(Vec2(popup->getContentSize()/2) + Vec2(-20,-10));
        pageView->setContentSize(popup->getContentSize() - Size(125,240));
        //pageView->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        //pageView->setBackGroundColor(Color3B::BLACK);
        
        pageView->setAnchorPoint(Point::ANCHOR_MIDDLE);
        pageView->removeAllPages();
        for (int i = 1 ; i < 4; i++) {
            string s = "loading/loading_" + to_string(i) + ".png";
            Layout* layout = Layout::create();
            layout->setContentSize(popup->getContentSize());
            layout->setBackGroundColor(Color3B::BLUE);
            //layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
            layout->cocos2d::Node::setPosition(popup->getContentSize()/2);
            
            ImageView* imageView = ImageView::create(s);
            imageView->setContentSize(popup->getContentSize());
            imageView->setPosition(Vec2(layout->getContentSize()/2 ) + Vec2(110,10));
            
            layout->addChild(imageView);
            pageView->insertPage(layout, i);
        }
        pageView->addEventListener(CC_CALLBACK_2(LoadingLayer::pageViewEvent, this));
        
        return true;
    }
    
    return false;
}

void LoadingLayer::pageViewEvent(Ref *pSender, PageView::EventType type)
{
    switch (type)
    {
        case PageView::EventType::TURNING:
        {
            PageView* pageView = dynamic_cast<PageView*>(pSender);
            
            
            int i = (int)pageView->getCurPageIndex() + 1;
            string d = "loading/page_number_" + to_string(i) + ".png";
            auto dot = Sprite::create(d);
            _dot->setTexture(dot->getTexture());
            
        }
            break;
            
        default:
            break;
    }
}

ActionInterval* LoadingLayer::setVisibleTrue(float time, Sprite * sp){
    auto action = Sequence::create(DelayTime::create(time), CallFunc::create([sp](){
        sp->setVisible(true);
    }),NULL);
    return action;
}

ActionInterval* LoadingLayer::setVisibleFalse(float time){
    auto action = Sequence::create(DelayTime::create(time), CallFunc::create([&](){
        for (auto dot : _dots) {
            dot->setVisible(false);
        }
    }),NULL);
    return action;
}

void LoadingLayer::setVisibleFalse(){
    
    _root->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) return;
        xTalk->initIntro(this->_armature);
        this->removeAllChildrenWithCleanup(true);
    });
    
    this->stopAction(_action);
    
    ImageView* table = ImageView::create("loading/tap_to_start.png");
    table->setPosition(Vec2(Director::getInstance()->getVisibleSize().width/2, 85));
    table->setTouchEnabled(true);
    table->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) return;
        xTalk->initIntro(this->_armature);
        this->removeAllChildrenWithCleanup(true);
    });
    addChild(table);
    table->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5f, 1.1), ScaleTo::create(0.5f, 1.0), NULL)));
}
