//
//  AdsLoadingLayer.cpp
//  SpotDifferences
//
//  Created by maxiang on 6/12/15.
//
//

#include "AdsLoadingLayer.h"

#define IMAGE_BG_PATH          "UI/UI-AdsLoading/animalhouse/loading_bg.jpg"
#define IMAGE_LOGO             "UI/UI-AdsLoading/animalhouse/animalhouse.png"
#define IMAGE_LODING           "UI/UI-AdsLoading/animalhouse/loading.png"
#define IMAGE_DOT              "UI/UI-AdsLoading/animalhouse/dot.png"
#define IMAGE_DOG_1              "UI/UI-AdsLoading/animalhouse/loading_dog_1.png"
#define IMAGE_DOG              "UI/UI-AdsLoading/animalhouse/loading_dog_"

#define RES_SKY_CASTLE_BG         "UI/UI-AdsLoading/skycastle/bg.jpg"
#define RES_SKY_CASTLE_LOGO       "UI/UI-AdsLoading/skycastle/castle.png"
#define RES_SKY_CASTLE_TITLE      "UI/UI-AdsLoading/skycastle/sky_castle.png"
#define RES_SKY_FLAG              "UI/UI-AdsLoading/skycastle/sky_castle_1.png"
#define RES_SKY_ANIMATION         "UI/UI-AdsLoading/skycastle/sky_castle_"

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
    _visibleSize = Director::getInstance()->getVisibleSize();
    this->setAnchorPoint(Vec2::ZERO);
    this->setPosition(Vec2::ZERO);
    this->setContentSize(_visibleSize);
    
    Sprite* bgSprite = Sprite::create(IMAGE_BG_PATH);
    bgSprite->setPosition(Vec2(_visibleSize.width / 2, _visibleSize.height / 2));
    this->addChild(bgSprite);
    
    Sprite* logoSprite = Sprite::create(IMAGE_LOGO);
    logoSprite->setPosition(Vec2(_visibleSize.width / 2, _visibleSize.height / 2 - _visibleSize.width / 10));
    this->addChild(logoSprite);
    
    Sprite* lodingSprite = Sprite::create(IMAGE_LODING);
    lodingSprite->setPosition(Vec2(_visibleSize.width / 2, _visibleSize.height / 2 - _visibleSize.width * 2 / 10));
    this->addChild(lodingSprite);
    
    //dot
    auto dot1 = Sprite::create(IMAGE_DOT);
    addChild(dot1);
    dot1->setPosition(Vec2(lodingSprite->getPosition().x + lodingSprite->getContentSize().width/2 + 5, lodingSprite->getPosition().y - 8));
    
    auto dot2 = Sprite::create(IMAGE_DOT);
    addChild(dot2);
    dot2->setPosition(Vec2(lodingSprite->getPosition().x + lodingSprite->getContentSize().width/2 + dot2->getContentSize().width + 7, lodingSprite->getPosition().y - 8));
    
    auto dot3 = Sprite::create(IMAGE_DOT);
    addChild(dot3);
    dot3->setPosition(Vec2(lodingSprite->getPosition().x + lodingSprite->getContentSize().width/2 + dot2->getContentSize().width*2 + 9, lodingSprite->getPosition().y - 8));
    dot3->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=]{
        dot2->setVisible(true), dot3->setVisible(false);
    }), DelayTime::create(ANIMATION_DURATION), CallFunc::create([=]{
        dot3->setVisible(true);
    }), DelayTime::create(ANIMATION_DURATION), CallFunc::create([=]{
        dot2->setVisible(false), dot3->setVisible(false);
    }), DelayTime::create(0.2), NULL)));
    
    //dog
    auto dogSprite = Sprite::create(IMAGE_DOG_1);
    dogSprite->setPosition(Vec2(_visibleSize.width / 2 - 20, _visibleSize.height / 2 + 80));
    this->addChild(dogSprite);
    
    
    //创建帧动画序列，名词形式
    auto animation = Animation::create();
    for (int i=1; i<4; i++)
    {
        char szName[100] = {0};
        sprintf(szName, "%s%d.png", IMAGE_DOG,i);
        animation->addSpriteFrameWithFile(szName);
    }
    //设置帧动画属性
    animation->setDelayPerUnit(2.0f / 6);      //每一帧停留的时间，2秒时间完成6幅图片显示，切记要写成2.0f形式！
    animation->setRestoreOriginalFrame(true);       //播放完后回到第一帧
    
    auto animate = Animate::create(animation);
    dogSprite->runAction(RepeatForever::create(animate));
}

void AdsLoadingLayer::loadSkyCastleStudio()
{
    //bg
    auto bg = Sprite::create(RES_SKY_CASTLE_BG);
    bg->setPosition(Vec2(getContentSize().width/2, getContentSize().height/2));
    addChild(bg);
    
    //logo
    auto logo = Sprite::create(RES_SKY_CASTLE_LOGO);
    logo->setPosition(Vec2(getContentSize().width/2, getContentSize().height/2 + 60));
    addChild(logo);
    
    //
    auto title = Sprite::create(RES_SKY_CASTLE_TITLE);
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

