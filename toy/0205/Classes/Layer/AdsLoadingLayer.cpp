//
//  AdsLoadingLayer.cpp
//  SpotDifferences
//
//  Created by maxiang on 6/12/15.
//
//

#include "AdsLoadingLayer.h"
#include "UICpp.h"

#define RES_TOYBOX_BG          "ads/toybox/bg.jpg"
#define RES_TOYBOX_BEAR        "ads/toybox/bear.png"
#define RES_TOYBOX_LOADING     "ads/toybox/loading.png"
#define RES_TOYBOX_BIG_STAR    "ads/toybox/star_2.png"
#define RES_TOYBOX_SMALL_STAR  "ads/toybox/star_1.png"
#define RES_TOYBOX_DOT         "ads/toybox/dian.png"

#define ANIMATION_DURATION      0.4

bool AdsLoadingLayer::init()
{
    if (!AdLoadingLayerBase::init()) {
        return false;
    }

    bool rcode = false;
    
    do {
        loadToyboxStudio();

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

void AdsLoadingLayer::loadToyboxStudio()
{
    //bg
    auto bg = Sprite::create(RES_TOYBOX_BG);
    bg->setPosition(Vec2(getContentSize().width/2, getContentSize().height/2));
    addChild(bg);
    
    //
    auto bear = Sprite::create(RES_TOYBOX_BEAR);
    bear->setPosition(Vec2(getContentSize().width/2, getContentSize().height/2 + 40));
    addChild(bear);
    
    //
    auto loading = Sprite::create(RES_TOYBOX_LOADING);
    loading->setPosition(Vec2(getContentSize().width/2, getContentSize().height/2 - 140));
    addChild(loading);
    
    //animation
    auto star1 = Sprite::create(RES_TOYBOX_BIG_STAR);
    addChild(star1);
    star1->setPosition(Vec2(getContentSize().width/2 - 80, getContentSize().height/2 + 100));
    star1->runAction(RepeatForever::create(Sequence::create(FadeIn::create(ANIMATION_DURATION), FadeTo::create(0.5, 100), DelayTime::create(0.8), NULL)));
    
    auto star2 = Sprite::create(RES_TOYBOX_SMALL_STAR);
    addChild(star2);
    star2->setPosition(Vec2(getContentSize().width/2 - 110, getContentSize().height/2 + 80));
    star2->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.4), FadeIn::create(ANIMATION_DURATION), FadeTo::create(0.5, 100), DelayTime::create(0.4), NULL)));
    
    auto star3 = Sprite::create(RES_TOYBOX_SMALL_STAR);
    addChild(star3);
    star3->setPosition(Vec2(getContentSize().width/2 - 77, getContentSize().height/2 + 40));
    star3->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.9), FadeIn::create(ANIMATION_DURATION), FadeTo::create(0.5, 100), NULL)));
    
    //dot
    auto dot1 = Sprite::create(RES_TOYBOX_DOT);
    addChild(dot1);
    dot1->setPosition(Vec2(loading->getPosition().x + loading->getContentSize().width/2 + 5, loading->getPosition().y - 5));

    auto dot2 = Sprite::create(RES_TOYBOX_DOT);
    addChild(dot2);
    dot2->setPosition(Vec2(loading->getPosition().x + loading->getContentSize().width/2 + dot2->getContentSize().width + 7, loading->getPosition().y - 5));
    
    auto dot3 = Sprite::create(RES_TOYBOX_DOT);
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

void AdsLoadingLayer::onEnter(){
    AdLoadingLayerBase::onEnter();
    if (xAdsEx->isShowBanner()) {
        xAds->setVisiable(kTypeBannerAds, false);
        xAdsEx->setBannerShow(true);
    }
}

void AdsLoadingLayer::onExit(){
    AdLoadingLayerBase::onExit();
    if (xAdsEx->isShowBanner()) {
        xAds->setVisiable(kTypeBannerAds, true);
        xAdsEx->setBannerShow(true);
    }
}


