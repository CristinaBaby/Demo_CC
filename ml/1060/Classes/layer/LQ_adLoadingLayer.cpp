//
//  LQ_adLoadingLayer.cpp
//  Cupcake
//
//  Created by luotianqiang1 on 15/6/15.
//
//

#include "LQ_adLoadingLayer.h"
#include "CMVisibleRect.h"
#include "IAPManager.h"
#include "KeyBackEvent.h"
#include<iostream>
#include "cocostudio/cocostudio.h"
#include "ChangeLayer.h"

bool LQ_adLoadingLayer::isChangeLayer = true;
using namespace std;
bool LQ_adLoadingLayer::init(){
    if(AdLoadingLayerBase::init()) {
        if(!isChangeLayer)
        {  _loadingMinTime = 1;
            _loadingMaxTime = 5;
            auto bg = Sprite::create("loading/loading.png");
            bg-> setPosition(getContentSize()*.5);
            addChild(bg);
            cocostudio::ArmatureDataManager::getInstance() -> addArmatureFileInfo("loading/loading/loading.ExportJson");
            cocostudio::Armature *armature = cocostudio::Armature::create("loading");
            armature->setPosition(CMVisibleRect::getPosition(480, 200));
            armature->getAnimation()->playWithIndex(0);
            this->addChild(armature);
        } else {
            _loadingMinTime = 7;
            _loadingMaxTime = 9;
            addChild(ChangeLayer::create());
        }
        isChangeLayer = true;
        return true;
    }
    return false;
}

void LQ_adLoadingLayer::onEnterTransitionDidFinish(){
    AdLoadingLayerBase::onEnterTransitionDidFinish();
    KeyBackEvent::getInstance()->addBackEvent([](){return true;}, this);
}
