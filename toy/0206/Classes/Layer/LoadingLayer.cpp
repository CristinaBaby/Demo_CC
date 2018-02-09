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
        Vec2 bgPostion = Helper::seekWidgetByName(_root, "loading")->getWorldPosition();
        
        auto popup = Sprite::create("loading/popup_1_1.png");
        popup->setPosition(Vec2(Director::getInstance()->getWinSize().width * 0.5, Director::getInstance()->getWinSize().height * 0.59));
        addChild(popup);
        
        _loadingBg = Sprite::create("loading/loading.png");
        _loadingBg->setPosition(bgPostion + Vec2(-50 , 165));
        addChild(_loadingBg);
        
        _dot = Sprite::create("loading/page_number_1.png");
        _dot->setPosition(_loadingBg->getPosition() + Vec2(50, 70));
        addChild(_dot,10);
        
        _dot_1 = Sprite::create("loading/point_4.png");
        _dot_2 = Sprite::create("loading/point_5.png");
        _dot_3 = Sprite::create("loading/point_6.png");
        _dot_4 = Sprite::create("loading/point_7.png");
        _dot_1->setPosition(_loadingBg->getPosition() + Vec2(218, -2));
        _dot_2->setPosition(_loadingBg->getPosition() + Vec2(218, -2));
        _dot_3->setPosition(_loadingBg->getPosition() + Vec2(218, -2));
        _dot_4->setPosition(_loadingBg->getPosition() + Vec2(218, -2));
        addChild(_dot_1);
        addChild(_dot_2);
        addChild(_dot_3);
        addChild(_dot_4);
        
        _dot_2->setVisible(false);
        _dot_3->setVisible(false);
        _dot_4->setVisible(false);
        
        _dots.push_back(_dot_2);
        _dots.push_back(_dot_3);
        _dots.push_back(_dot_4);
        
        _action = RepeatForever::create(Sequence::create(setVisibleTrue(DELAY_TIME, _dot_2), setVisibleTrue(DELAY_TIME, _dot_3), setVisibleTrue(DELAY_TIME, _dot_4), setVisibleFalse(DELAY_TIME),NULL));
        runAction(_action);
        
        auto pageView = PageViewEx::create();
        popup->addChild(pageView);
        pageView->setPosition(popup->getContentSize()/2);
        pageView->setContentSize(popup->getContentSize() - Size(55,0));
        pageView->setAnchorPoint(Point::ANCHOR_MIDDLE);
        pageView->removeAllPages();
        for (int i = 1 ; i < 4; i++) {
            string s = "loading/hand_" + to_string(i) + "_" + to_string(i) + ".png";
            string h = "loading/hand_" + to_string(i) + ".png";
            Layout* layout = Layout::create();
            layout->setContentSize(popup->getContentSize());
            layout->setBackGroundColor(Color3B::BLUE);
            //layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
            layout->cocos2d::Node::setPosition(popup->getContentSize()/2);
            
            ImageView* imageView = ImageView::create(s);
            imageView->setContentSize(popup->getContentSize());
            imageView->setPosition(Vec2(layout->getContentSize()/2) + Vec2(218,130));
            
            ImageView* hand = ImageView::create(h);
            hand->setContentSize(popup->getContentSize());
            hand->setPosition(Vec2(layout->getContentSize()/2) + Vec2(40,110));
            
            layout->addChild(imageView);
            layout->addChild(hand);
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
    _dot_1->setVisible(false);
    _loadingBg->setVisible(false);
    
    for (auto dot : _dots) {
        dot->setVisible(false);
    }
    
    ImageView* table = ImageView::create("loading/tap_to_start.png");
    table->setPosition(_loadingBg->getPosition() + Vec2(45, -30));
    table->setTouchEnabled(true);
    table->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) return;
        xTalk->initIntro(this->_armature);
        this->removeAllChildrenWithCleanup(true);
    });
    addChild(table);
    table->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5f, 1.1), ScaleTo::create(0.5f, 1.0), NULL)));
}
