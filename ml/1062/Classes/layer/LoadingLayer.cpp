//
//  LoadingLayer.cpp
//  PizzaMaker
//
//  Created by luotianqiang1 on 3/17/15.
//
//

#include "LoadingLayer.h"
#include "CMVisibleRect.h"
#include "HomeLayer.h"

LoadingLayer::~LoadingLayer(){
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("loading/loading.plist");
}

bool LoadingLayer::init() {
    if(BaseStepLayer::init()) {
        
        auto bg = Sprite::create("loading/loading.png");
        bg-> setPosition(getContentSize()*.5);
        addChild(bg);
        
        cocostudio::ArmatureDataManager::getInstance() -> addArmatureFileInfo("loading/loading/loading.ExportJson");
        cocostudio::Armature *armature = cocostudio::Armature::create("loading");
        armature->setPosition(CMVisibleRect::getPosition(480, 200));
        armature->getAnimation()->playWithIndex(0);
        this->addChild(armature);
        
        this -> runAction(Sequence::create(DelayTime::create(0.5)
                                           ,CallFunc::create([=] {
            SoundPlayer::getInstance() ->playEffect("loading/loading.mp3");
        }),DelayTime::create(4), CallFunc::create(CC_CALLBACK_0(LoadingLayer::enCallBack, this)), nullptr));
        
        return true;
    }
    return false;
}

void LoadingLayer::enCallBack(){

    Director::getInstance()->replaceScene(HomeLayer::createScene());
    STSystemFunction _cfs;
    _cfs.showNewsBlast(NewsModeLaunch);
    SoundPlayer::getInstance()->playBackGroundMusic("sound/Kids Fun.mp3");
}

void LoadingLayer::onEnterTransitionDidFinish() {
    BaseStepLayer::onEnterTransitionDidFinish();
}