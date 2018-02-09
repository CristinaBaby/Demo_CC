//
//  ContinueLayer.cpp
//  ColorBook0818
//
//  Created by huangwen on 15/9/21.
//
//

#include "ContinueLayer.h"
#include "GameScene.h"
#include "UITools.h"
#include "UICpp.h"
#include "CBPackManager.h"

ContinueLayer* ContinueLayer::create(int packIndex, int pictureIndex){
    ContinueLayer* layer = new(std::nothrow) ContinueLayer();
    if (layer && layer->init(packIndex, pictureIndex)) {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool ContinueLayer::init(int packIndex, int pictureIndex){
    if(Layer::init()){
        setName("continueLayer");

        _packIndex = packIndex;
        _pictureIndex = pictureIndex;
        
        auto _root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("popup.json"));
        addChild(_root);
        
        quickButton("reset", _root, CC_CALLBACK_2(ContinueLayer::TouchDown, this));
        quickButton("continue", _root, CC_CALLBACK_2(ContinueLayer::TouchDown, this));
        
        xAds->setVisiable(kTypeBannerAds, true);
        xAdsEx->setBannerShow(true);
        
        return true;
    }
    return false;
}

void ContinueLayer::TouchDown(cocos2d::Ref *pSender, ui::Widget::TouchEventType type){
    if(ui::Widget::TouchEventType::ENDED != type) return;
    
    std::string name = ((ui::Widget*)pSender)->getName();
    
    xAudio->playDefaultSound();
    xAds->setVisiable(kTypeBannerAds, false);
    xAdsEx->setBannerShow(false);
    if (name.compare("continue") == 0) {
        continueAction();
    }
    else if(name.compare("reset") == 0) {
        resetAction();
    }
    
}
void ContinueLayer::continueAction(){
    auto scene = GameScene::create(_packIndex, _pictureIndex);
    Director::getInstance()->replaceScene(scene);
}

void ContinueLayer::resetAction(){
    std::string filePath = PackManager::getInstance()->getPackHistoryPicturePath(_packIndex, _pictureIndex, true);
    bool isExit = FileUtils::getInstance()->isFileExist(filePath);
    if (isExit) {
        std::remove(filePath.c_str());
    }
    
    std::string logoPath = PackManager::getInstance()->getPackHistoryLogoPath(_packIndex, _pictureIndex, true);
    bool isexit = FileUtils::getInstance()->isFileExist(filePath);
    if (isexit) {
        std::remove(logoPath.c_str());
    }
    
    auto scene = GameScene::create(_packIndex, _pictureIndex);
    Director::getInstance()->replaceScene(scene);
}