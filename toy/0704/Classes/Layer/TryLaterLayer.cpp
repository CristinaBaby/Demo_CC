//
//  TryLaterLayer.cpp
//  ColorBook0818
//
//  Created by huangwen on 15/9/21.
//
//

#include "TryLaterLayer.h"
#include "GameScene.h"
#include "UITools.h"
#include "UICpp.h"
#include "PackManager.h"
#include "GameScene.h"

TryLaterLayer* TryLaterLayer::create(){
    TryLaterLayer* layer = new(std::nothrow) TryLaterLayer();
    if (layer && layer->init()) {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool TryLaterLayer::init(){
    if(Layer::init()){
        setName("TryLaterLayer");
        
        auto _root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("try_later.json"));
        addChild(_root);
        
        quickButton("try", _root, CC_CALLBACK_2(TryLaterLayer::onButton, this));
        
        return true;
    }
    return false;
}

void TryLaterLayer::onButton(cocos2d::Ref *pSender, ui::Widget::TouchEventType type){
    if(ui::Widget::TouchEventType::ENDED != type) return;
    
    std::string name = ((ui::Widget*)pSender)->getName();
    
    xAudio->playDefaultSound();
    
    if (name.compare("try") == 0) {
    }
    else if(name.compare("play now") == 0) {
    }
    
    removeFromParent();
}
