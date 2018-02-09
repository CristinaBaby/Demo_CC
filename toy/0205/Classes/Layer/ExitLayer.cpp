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
        
        auto yes = quickButton("reset", _root, CC_CALLBACK_2(ExitLayer::TouchDown, this));
        auto no = quickButton("continue", _root, CC_CALLBACK_2(ExitLayer::TouchDown, this));
        
        yes->loadTextureNormal("popup/yes.png");
        no->loadTextureNormal("popup/no.png");
        
        auto text = (ImageView*)Helper::seekWidgetByName(_root, "text");
        text->loadTexture("popup/text_1.png");
        
        xAds->setVisiable(kTypeBannerAds, true);
        xAdsEx->setBannerShow(true);
        
        return true;
    }
    return false;
}

void ExitLayer::TouchDown(cocos2d::Ref *pSender, ui::Widget::TouchEventType type){
    if(ui::Widget::TouchEventType::ENDED != type) return;
    
    std::string name = ((ui::Widget*)pSender)->getName();
    
    xAudio->playDefaultSound();
    xAds->setVisiable(kTypeBannerAds, false);
    xAdsEx->setBannerShow(false);
    
    if(name.compare("reset") == 0) {
        Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#endif
    }
    else if (name.compare("continue") == 0) {
        removeFromParent();
    }
}