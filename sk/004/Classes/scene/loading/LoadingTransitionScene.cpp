//
//  LoadingTransitionScene.cpp
//  BHUG1062
//
//  Created by liji on 16/5/4.
//
//
#include "LoadingTransitionScene.h"
#include "AdsManager.h"
#include "ActionHelper.h"
#include "IAPAapter.h"

bool LoadingTransitionScene::init()
{
    if (!StudioLayer::init())
    {
        return false;
    }

    this->setBackground("loading/768x1136.jpg");
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    IAPAapter a;
//    int index = a.getAndroidPlatformCode();
//    if (index == 32)
    {
        this->setBackground("loading/kloadingp.jpg");
        cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("loading/NewAnimation.ExportJson");
        cocostudio::Armature *armature = cocostudio::Armature::create("NewAnimation");
        CMVisibleRect::setPosition(armature, 320, 312);
        armature->setScale(0.7f);
        armature->getAnimation()->playWithIndex(0);
        this->addChild(armature);
    }
#endif
//
//    auto star = Sprite::create("res/loading/star.png");
//    star->setPosition(CMVisibleRect::getPosition(480, 320));
//    this->addToContentLayer(star,1);
//    star->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.8, 1.05),
//                                                           ScaleTo::create(0.8, 0.98),
//                                                           NULL)));
//    
//    
//    auto logo = Sprite::create("res/loading/hollywood.png");
//    logo->setPosition(CMVisibleRect::getPosition(480,320));
//    this->addToContentLayer(logo,4);
//    
//    auto logoShine = Sprite::create("res/loading/star0.png");
//    logoShine->setPosition(CMVisibleRect::getPosition(480,320));
//    this->addToContentLayer(logoShine,5);
//
//    vector<Vec2> pos =
//    {
//        CMVisibleRect::getPosition(158, 424),
//        CMVisibleRect::getPosition(783, 139),
//        CMVisibleRect::getPosition(766, 466),
//        CMVisibleRect::getPosition(95, 134),
//    };
//
//    for (int i=0; i<4;i++)
//    {
//        Sprite* star = Sprite::create(StringUtils::format("res/loading/star%d.png",i+1));
//        this->addChild(star);
//        star->setPosition(pos.at(i));
//        star->setVisible(false);
//        _stars.pushBack(star);
//    }
//    
//    this->schedule(schedule_selector(LoadingTransitionScene::loadingAnimation), 0.3f);
    
    
    auto lis = EventListenerTouchOneByOne::create();
    lis->setSwallowTouches(true);
    lis->onTouchBegan=[](Touch* t,Event* e)
    {
    
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(lis, this);
    
    return true;
}


void LoadingTransitionScene::loadingAnimation(float dt)
{
    static int animationIndex = 0;
    if (animationIndex<4)
    {
        _stars.at(animationIndex)->setVisible(true);
    }
    if (animationIndex == 4)
    {
        for (auto star : _stars)
        {
            star->runAction(Blink::create(0.6, 1));
        }
    }
    if (animationIndex == 6)
    {
        _stars.at(0)->runAction(Blink::create(0.6, 1));
        _stars.at(1)->runAction(Blink::create(0.6, 1));
    }
    if (animationIndex == 8)
    {
        _stars.at(2)->runAction(Blink::create(0.6, 1));
        _stars.at(3)->runAction(Blink::create(0.6, 1));
    }
    if (animationIndex == 10)
    {
        for (auto star : _stars)
        {
            star->stopAllActions();
            star->setVisible(false);
        }
    }
    animationIndex++;
    if (animationIndex>10)
    {
        animationIndex=0;
    }
    
}

void LoadingTransitionScene::onEnter()
{
    StudioLayer::onEnter();
    this->runAction(Sequence::create(DelayTime::create(221.5),
                                     CallFunc::create([=]
                                                      {
                                                          if (_transitionScene)
                                                          {
                                                              _transitionScene();
                                                              this->removeFromParentAndCleanup(true);
                                                          }
                                                      }),
                                     NULL));
    
//    AdsManager::getInstance()->setVisiable(kTypeBannerAds, false);

}

void LoadingTransitionScene::onExit()
{
    StudioLayer::onExit();
//    AdsManager::getInstance()->setVisiable(kTypeBannerAds, true);

}


