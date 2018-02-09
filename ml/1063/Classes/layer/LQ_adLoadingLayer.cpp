//
//  LQ_adLoadingLayer.cpp
//  Cupcake
//
//  Created by luotianqiang1 on 15/6/15.
//
//

#include "LQ_adLoadingLayer.h"
#include "KeyBackEvent.h"
#include<iostream>
#include "cocostudio/cocostudio.h"
#include "ChangeLoading.h"

bool LQ_adLoadingLayer::isChangeLayer = true;
using namespace std;
bool LQ_adLoadingLayer::init(){
    if(AdLoadingLayerBase::init()) {
        _loadingMinTime = 0;
        _loadingMaxTime = 5;
        return true;
    }
    return false;
}

void LQ_adLoadingLayer::onEnterTransitionDidFinish(){
    AdLoadingLayerBase::onEnterTransitionDidFinish();
    removeAllChildren();
    this->runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create([this](){
        if(!isChangeLayer)
        {
//            _loadingMinTime = 0;
//            _loadingMaxTime = 5;
            auto bg = Sprite::create("loading/loading.jpg");
            bg-> setPosition(getContentSize()*.5);
            addChild(bg);
            cocostudio::ArmatureDataManager::getInstance() -> addArmatureFileInfo("loading/loading/loading.ExportJson");
            cocostudio::Armature *armature = cocostudio::Armature::create("loading");
        auto size = Director::getInstance()->getVisibleSize();
            armature->setPosition(size*.5-Size(0,100));
            armature->getAnimation()->playWithIndex(0);
            this->addChild(armature);
        }
        else {
//            _loadingMinTime = 3.5;
//            _loadingMaxTime = 5;
            addChild(ChangeLoading::create());
        }
        isChangeLayer = true;
    }),nullptr));
   
    KeyBackEvent::getInstance()->addBackEvent([](){return true;}, this);
}
