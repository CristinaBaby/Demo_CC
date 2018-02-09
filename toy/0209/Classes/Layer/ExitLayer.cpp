//
//  ExitLayer.cpp
//  ColorBook0818
//
//  Created by huangwen on 15/9/21.
//
//

#include "ExitLayer.h"
#include "GameScene.h"
#include "UITools.h"
#include "UICpp.h"

bool ExitLayer::init(){
    if(Layer::init()){
        
        setName("exitLayer");
        
        auto _root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("popup.json"));
        addChild(_root);
        
        auto yes = quickButton("yes", _root, CC_CALLBACK_2(ExitLayer::TouchDown, this));
        auto no = quickButton("no", _root, CC_CALLBACK_2(ExitLayer::TouchDown, this));
        
        xAds->showAds(kTypeBannerAds);
        
        return true;
    }
    return false;
}

void ExitLayer::TouchDown(cocos2d::Ref *pSender, ui::Widget::TouchEventType type){
    if(ui::Widget::TouchEventType::ENDED != type) return;
    
    std::string name = ((ui::Widget*)pSender)->getName();
    
    xAudio->playDefaultSound();
    xAds->removeAds(kTypeBannerAds);
    
    if(name.compare("yes") == 0) {
        Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#endif
    }
    else if (name.compare("no") == 0) {
        removeFromParent();
    }
}