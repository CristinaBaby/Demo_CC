//
//  AdsLoadingLayer.cpp
//  SpotDifferences
//
//  Created by maxiang on 6/12/15.
//
//

#include "AdsLoadingLayer.h"
#include "../utilities/UICpp.h"

#define INTERVEL            5
#define ANIMATION_INTERVEL  0.12

using namespace cocostudio;

bool AdsLoadingLayer::init()
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    if (!AdLoadingLayerBase::init()) {
        return false;
    }
#else
    if (!Layer::init()) {
        return false;
    }
#endif
    
    bool rcode = false;
    
    do {
        //bg
        auto bg = Sprite::create("adsloading.jpg");
        bg->setPosition(Vec2(getContentSize().width/2, getContentSize().height/2));
        addChild(bg);
        
        ArmatureDataManager::getInstance()->addArmatureFileInfo("062514.csb");
        
        Armature *armature = Armature::create("062514");
        armature->setPosition(Vec2(getContentSize()/2));
        auto ani = armature->getAnimation();
        
        ani->playWithIndex(0);
        addChild(armature);
        
        //touch event listen
        auto touchEventListener = EventListenerTouchOneByOne::create();
        touchEventListener->setSwallowTouches(true);
        touchEventListener->onTouchBegan = [](Touch* touch, Event* event) {
            return true;
        };
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
        
        rcode = true;
    } while (0);
    
    return rcode;
}
