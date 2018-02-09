//
//  AdsLoadingLayer.cpp
//  SpotDifferences
//
//  Created by maxiang on 6/12/15.
//
//

#include "AdsLoadingLayer.h"
#include "../Model/STVisibleRect.h"

#define RES_ANIMAL_BG          "UI/UI-AdsLoading/ah/loading_bg.jpg"
#define RES_ANIMAL_DOG1        "UI/UI-AdsLoading/ah/loading_dog_1.png"
#define RES_ANIMAL_DOG2        "UI/UI-AdsLoading/ah/loading_dog_2.png"
#define RES_ANIMAL_DOG3        "UI/UI-AdsLoading/ah/loading_dog_3.png"
#define RES_ANIMAL_TITLE       "UI/UI-AdsLoading/ah/ah.png"
#define RES_ANIMAL_LOADING     "UI/UI-AdsLoading/ah/loading.png"
#define RES_ANIMAL_DOT         "UI/UI-AdsLoading/ah/dot.png"

#define RES_SC_BG              "UI/UI-AdsLoading/sc/bg.jpg"
#define RES_SC_LOGO            "UI/UI-AdsLoading/sc/castle.png"
#define RES_SC_TITLE           "UI/UI-AdsLoading/sc/sc.png"
#define RES_SKY_FLAG           "UI/UI-AdsLoading/sc/sc_1.png"
#define RES_SKY_ANIMATION      "UI/UI-AdsLoading/sc/sc_"

#define ANIMATION_DURATION      0.4

bool AdsLoadingLayer::init()
{
    if (!AdLoadingLayerBase::init()) {
        return false;
    }
    
    bool rcode = false;
    
    do {

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        loadSCStudio();
#else
        loadAHStudio();
#endif
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

void AdsLoadingLayer::loadSCStudio()
{
    //bg
    auto bg = Sprite::create(RES_SC_BG);
    bg->setPosition(Vec2(getContentSize().width/2, getContentSize().height/2));
    addChild(bg);
    
    //logo
    auto logo = Sprite::create(RES_SC_LOGO);
    logo->setPosition(Vec2(getContentSize().width/2, getContentSize().height/2 + 60));
    addChild(logo);
    
    //
    auto title = Sprite::create(RES_SC_TITLE);
    title->setPosition(Vec2(getContentSize().width/2, getContentSize().height/2 - 120));
    addChild(title);
    
    //animation
    auto animation = Animation::create();
    for(int i = 1; i < 6; ++i)
    {
        char animationImageName[100];
        sprintf(animationImageName, "%s%d.png", RES_SKY_ANIMATION, i);
        animation->addSpriteFrameWithFile(animationImageName);
    }
    animation->setDelayPerUnit(0.12);
    animation->setRestoreOriginalFrame(true);
    
    auto action = Animate::create(animation);
    
    auto flag = Sprite::create(RES_SKY_FLAG);
    flag->setPosition(Vec2(getContentSize().width/2 + 13.8, getContentSize().height/2 + 237));
    flag->runAction(RepeatForever::create(Sequence::create(action, action->reverse(), NULL)));
    addChild(flag);
}

void AdsLoadingLayer::loadAHStudio()
{
    //bg
    auto bg = Sprite::create(RES_ANIMAL_BG);
    bg->setPosition(Vec2(getContentSize()/2));
    addChild(bg);
    
    auto animation = Animation::create();
    animation->addSpriteFrameWithFile(RES_ANIMAL_DOG1);
    animation->addSpriteFrameWithFile(RES_ANIMAL_DOG2);
    animation->addSpriteFrameWithFile(RES_ANIMAL_DOG3);
    animation->setDelayPerUnit(0.1);
    auto action = Animate::create(animation);

    //
    auto dog = Sprite::create(RES_ANIMAL_DOG1);
    namespaceST::STVisibleRect::setPosition(dog, 460.0, 402.0);
    addChild(dog);
    dog->runAction(RepeatForever::create(Sequence::create(action, action->reverse(), NULL)));
    
    //
    auto title = Sprite::create(RES_ANIMAL_TITLE);
    namespaceST::STVisibleRect::setPosition(title, 480.0, 255.0);
    addChild(title);
    
    //
    auto loading = Sprite::create(RES_ANIMAL_LOADING);
    namespaceST::STVisibleRect::setPosition(loading, 460.0, 171.0);
    addChild(loading);

    //dot
    auto dot1 = Sprite::create(RES_ANIMAL_DOT);
    addChild(dot1);
    dot1->setPosition(Vec2(loading->getPosition().x + loading->getContentSize().width/2 + 5, loading->getPosition().y - 5));

    auto dot2 = Sprite::create(RES_ANIMAL_DOT);
    addChild(dot2);
    dot2->setPosition(Vec2(loading->getPosition().x + loading->getContentSize().width/2 + dot2->getContentSize().width + 7, loading->getPosition().y - 5));
    
    auto dot3 = Sprite::create(RES_ANIMAL_DOT);
    addChild(dot3);
    dot3->setPosition(Vec2(loading->getPosition().x + loading->getContentSize().width/2 + dot2->getContentSize().width*2 + 9, loading->getPosition().y - 5));
    dot3->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=]{
        dot2->setVisible(true), dot3->setVisible(false);
    }), DelayTime::create(ANIMATION_DURATION), CallFunc::create([=]{
        dot3->setVisible(true);
    }), DelayTime::create(ANIMATION_DURATION), CallFunc::create([=]{
        dot2->setVisible(false), dot3->setVisible(false);
    }), DelayTime::create(0.2), NULL)));
}




