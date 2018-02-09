//
//  SaveLayer.cpp
//  BabyPlay
//
//  Created by luoxp on 3/18/15.
//
//

#include "SaveLayer.h"
#include "STSystemFunction.h"
#include "AppConfigs.h"
#include "UICpp.h"
#include "UITools.h"
#include "CBAppManager.h"
#include "GameScene.h"
#include "SugarManager.hpp"
#include "PackManager.h"


SaveLayer* SaveLayer::create(Image *image, GameScene * game)
{
    SaveLayer *pRet = new SaveLayer();
    if (pRet && pRet->init(image, game))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool SaveLayer::init(Image *image, GameScene * game){
    
    if(Layer::init())
    {
        auto _root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("save.json"));
        addChild(_root);
        
        quickButton("back", _root, CC_CALLBACK_2(SaveLayer::onButton, this));
        quickButton("save", _root, CC_CALLBACK_2(SaveLayer::onButton, this));
        
        auto paper = (ImageView*)Helper::seekWidgetByName(_root, "Image_8");
        
        auto texture = new Texture2D();
        texture->initWithImage(image);
        texture->autorelease();
        auto picture = Sprite::createWithTexture(texture,Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
        
        float scaleX = (paper->getContentSize().width - 12) / picture->getContentSize().width;
        float scaleY = (paper->getContentSize().height - 14) / picture->getContentSize().height;
        
        float scale = scaleX > scaleY ? scaleY : scaleX;
        picture->setScale(scale * 0.92);
        picture->setPosition(paper->getContentSize()/2);
        paper->addChild(picture, 1);
        
        _pictureImage = image;
        _game = game;
        
        return true;
    }
    
    return false;
}

void SaveLayer::onButton(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    string name = ((Node*)pSender)->getName();
    
    xAudio->playDefaultSound();
    
    if (name.compare("save") == 0) {
        STSystemFunction function = STSystemFunction();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        function.saveLocalImage(_pictureImage);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string writePath = function.getSdCardPath()+"/Totemism/";
        
        if(!FileUtils::sharedFileUtils()->isFileExist(writePath))
        {
            xPack->createDirectory(writePath.c_str());
            log("MX: android writeable path: %s", writePath.c_str());
        }
        
        struct timeval now;
        gettimeofday(&now, nullptr);
        std::string _time = StringUtils::format("%ld", (now.tv_sec * 1000 + now.tv_usec / 1000));
        std::string fullPath = writePath + _time + ".jpg";
        
        log("MX: %s", fullPath.c_str());
        
        bool isSaveSuccess = _pictureImage->saveToFile(fullPath.c_str());
        if(isSaveSuccess)
            function.makeToast("Save successfully!");
        else
            function.makeToast("Save failed!");
        function.refreshDCIM(fullPath);
#endif
    }
    
    removeFromParent();
}

