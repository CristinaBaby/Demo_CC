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
#include "CBPackManager.h"


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
        setName("saveLayer");
        
        auto _root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("save.json"));
        addChild(_root);
        
        quickButton("close", _root, CC_CALLBACK_2(SaveLayer::onButton, this));
        quickButton("save", _root, CC_CALLBACK_2(SaveLayer::onButton, this));
        
        auto paper = (ImageView*)Helper::seekWidgetByName(_root, "paper-3");
        auto paper_over = (ImageView*)Helper::seekWidgetByName(_root, "paper");
        paper_over->setLocalZOrder(1);
        
        auto texture = new Texture2D();
        texture->initWithImage(image);
        texture->autorelease();
        auto picture = Sprite::createWithTexture(texture);
        float scaleX = paper->getContentSize().width / picture->getContentSize().width;
        float scaleY = paper->getContentSize().height / picture->getContentSize().height;
        
        float scale = scaleX > scaleY ? scaleY : scaleX;
        picture->setScale(scale * 0.95);
        picture->setPosition(paper->getContentSize()/2);
        paper->addChild(picture, 1);
        
        _pictureImage = image;
        _game = game;
        
//        _particle = ParticleSystemQuad::create("particle/finish.plist");
//        _particle->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2, Director::getInstance()->getWinSize().height));
//        _particle->setPosVar(Vec2(Director::getInstance()->getWinSize().width / 2, 0));
//        addChild(_particle);
//        
//        _bangbang = ParticleSystemQuad::create("particle/bangbang.plist");
//        _bangbang->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2, Director::getInstance()->getWinSize().height));
//        _bangbang->setPosVar(Vec2(Director::getInstance()->getWinSize().width / 2, 0));
//        addChild(_bangbang,1);
//        
//        _candy = ParticleSystemQuad::create("particle/candy.plist");
//        _candy->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2, Director::getInstance()->getWinSize().height));
//        _candy->setPosVar(Vec2(Director::getInstance()->getWinSize().width / 2, 0));
//        addChild(_candy,2);
        
        SugarManager::getInstance()->start();
        //scheduleOnce(schedule_selector(SaveLayer::stopParticle), 5);
        return true;
    }
    
    return false;
}

void SaveLayer::onExit()
{
    SugarManager::getInstance()->stop();
    Layer::onExit();
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
        std::string writePath = function.getSdCardPath()+"/ColorBook/";
        
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
        xApp->requestFullScreenAd(_game, 10);
    }
    SugarManager::getInstance()->stop();
    removeFromParent();
}

void SaveLayer::createParticle(float dt){
//    auto particle = ParticleSystemQuad::create("particle/finish.plist");
//    particle->setPosition(Vec2(Director::getInstance()->getWinSize().width * quickRandom(2, 8) / 10, Director::getInstance()->getWinSize().height * quickRandom(2, 7) / 10));
//    addChild(particle);
//    
//    auto bangbang = ParticleSystemQuad::create("particle/bangbang.plist");
//    bangbang->setPosition(Vec2(Director::getInstance()->getWinSize().width * quickRandom(2, 8) / 10, Director::getInstance()->getWinSize().height * quickRandom(2, 7) / 10));
//    addChild(bangbang,1);
}

void SaveLayer::stopParticle(float dt){
//    _particle->stopSystem();
//    _particle->setVisible(false);
//    
//    _bangbang->stopSystem();
//    _bangbang->setVisible(false);
//    
//    _candy->stopSystem();
//    _candy->setVisible(false);
}


