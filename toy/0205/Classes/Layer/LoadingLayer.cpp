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
        
        _loadingBg = Sprite::create("loading/loading.png");
        _loadingBg->setPosition(bgPostion);
        addChild(_loadingBg);
        
        _prompt = ImageView::create("loading/prompt.png");
        _prompt->setPosition(Vec2(Director::getInstance()->getWinSize().width * 0.5, Director::getInstance()->getWinSize().height * 0.59));
        addChild(_prompt);
        
        _dot_1 = Sprite::create("loading/point_4.png");
        _dot_2 = Sprite::create("loading/point_5.png");
        _dot_3 = Sprite::create("loading/point_6.png");
        _dot_4 = Sprite::create("loading/point_7.png");
        _dot_1->setPosition(_loadingBg->getPosition() + Vec2(102, 0));
        _dot_2->setPosition(_loadingBg->getPosition() + Vec2(102, 0));
        _dot_3->setPosition(_loadingBg->getPosition() + Vec2(102, 0));
        _dot_4->setPosition(_loadingBg->getPosition() + Vec2(102, 0));
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
        
        //_root->setTouchEnabled(true);

        return true;
    }

    return false;
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
    
    ImageView* table = ImageView::create("loading/guide_taptostart.png");
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
